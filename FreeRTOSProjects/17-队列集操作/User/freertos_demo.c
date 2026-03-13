#include "freertos_demo.h"
#include "../Drivers/SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./MALLOC/malloc.h"
/*FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   128
TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
void task1( void * pvParameters );


/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
void task2( void * pvParameters );

QueueSetHandle_t queueset_handle;
QueueHandle_t    queue_handle;
QueueHandle_t    semphr_handle;
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
    xTaskCreate((TaskFunction_t         )   start_task,
                (char *                 )   "start_task",
                (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   START_TASK_PRIO,
                (TaskHandle_t *         )   &start_task_handler );
    vTaskStartScheduler();
}


void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();                               /* 进入临界区 */
    queueset_handle = xQueueCreateSet( 2 );             /* 创建队列集，可以存放2个队列 */
    if(queueset_handle != NULL)
    {
        printf("队列集创建成功！！\r\n");
    }
    
    queue_handle = xQueueCreate( 1, sizeof(uint8_t) );  /* 创建队列 */ 
    semphr_handle = xSemaphoreCreateBinary();           /* 创建二值信号量 */
    
    xQueueAddToSet( queue_handle,queueset_handle);
    xQueueAddToSet( semphr_handle,queueset_handle);
    
    xTaskCreate((TaskFunction_t         )   task1,
                (char *                 )   "task1",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIO,
                (TaskHandle_t *         )   &task1_handler );
                
    xTaskCreate((TaskFunction_t         )   task2,
                (char *                 )   "task2",
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIO,
                (TaskHandle_t *         )   &task2_handler );
                             
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，实现队列发送以及信号量释放 */
void task1( void * pvParameters )
{
    uint8_t key = 0;
    BaseType_t err = 0;
    while(1) 
    {
        key = key_scan(0);
        if(key == KEY0_PRES)
        {
            err = xQueueSend( queue_handle, &key, portMAX_DELAY );
            if(err == pdPASS)
            {
                printf("往队列queue_handle写入数据成功！！\r\n");
            }
        }else if(key == KEY1_PRES)
        {
            err = xSemaphoreGive(semphr_handle);
            if(err == pdPASS)
            {
                printf("释放信号量成功！！\r\n");
            }
        }
        vTaskDelay(10);
    }
}

/* 任务二，获取队列集的消息 */
void task2( void * pvParameters )
{
    QueueSetMemberHandle_t member_handle;
    uint8_t key;
    while(1)
    {
        member_handle = xQueueSelectFromSet( queueset_handle,portMAX_DELAY);
        if(member_handle == queue_handle)
        {
            xQueueReceive( member_handle,&key,portMAX_DELAY);
            printf("获取到的队列数据为：%d\r\n",key);
        }else if(member_handle == semphr_handle)
        {
            xSemaphoreTake( member_handle, portMAX_DELAY );
            printf("获取信号量成功！！\r\n");
        }
    }
}

