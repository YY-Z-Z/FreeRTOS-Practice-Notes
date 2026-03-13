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

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO         4
#define TASK3_STACK_SIZE   128
TaskHandle_t    task3_handler;
void task3( void * pvParameters );

QueueHandle_t key_queue;        /* 小数据句柄 */
QueueHandle_t big_date_queue;   /* 大数据句柄 */
char buff[100] = {"我是一个大数组，大大的数组 124214 uhsidhaksjhdklsadhsaklj"};
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
    /* 队列的创建 */
    key_queue = xQueueCreate( 2, sizeof(uint8_t) );
    if(key_queue != NULL)
    {
        printf("key_queue队列创建成功！！\r\n");
    }else printf("key_queue队列创建失败！！\r\n");
    
    big_date_queue = xQueueCreate( 1, sizeof(char *) );
    if(big_date_queue != NULL)
    {
        printf("big_date_queue队列创建成功！！\r\n");
    }else printf("big_date_queue队列创建失败！！\r\n");
    
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
    taskENTER_CRITICAL();               /* 进入临界区 */
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
               
    xTaskCreate((TaskFunction_t         )   task3,
                (char *                 )   "task3",
                (configSTACK_DEPTH_TYPE )   TASK3_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK3_PRIO,
                (TaskHandle_t *         )   &task3_handler );                
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，实现入队 */
void task1( void * pvParameters )
{
    uint8_t key = 0;
    char * buf;
    BaseType_t   err = 0;
    buf = &buff[0]; /* buf = &buff[0] */
    while(1) 
    {
        key = key_scan(0);
        if(key == KEY0_PRES || key == KEY1_PRES)
        {
            err = xQueueSend( key_queue, &key, portMAX_DELAY );
            if(err != pdTRUE)
            {
                printf("key_queue队列发送失败\r\n");
            }
        }else if(key == WKUP_PRES)
        {
            err = xQueueSend( big_date_queue, &buf, portMAX_DELAY );
            if(err != pdTRUE)
            {
                printf("key_queue队列发送失败\r\n");
            }
        }
        vTaskDelay(10);
    }
}

/* 任务二，小数据出队 */
void task2( void * pvParameters )
{
    uint8_t key = 0;
    BaseType_t err = 0;
    while(1)
    {
        err = xQueueReceive( key_queue,&key,portMAX_DELAY);
        if(err != pdTRUE)
        {
            printf("key_queue队列读取失败\r\n");
        }else 
        {
            printf("key_queue读取队列成功，数据：%d\r\n",key);
        }
    }
}

/* 任务三，大数据出队 */
void task3( void * pvParameters )
{
    char * buf;
    BaseType_t err = 0;
    while(1)
    {
        err = xQueueReceive( big_date_queue,&buf,portMAX_DELAY);
        if(err != pdTRUE)
        {
            printf("big_date_queue队列读取失败\r\n");
        }else 
        {
            printf("数据：%s\r\n",buf);
        }
    }
}

