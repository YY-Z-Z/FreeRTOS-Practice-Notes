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
#include "event_groups.h"
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

EventGroupHandle_t  eventgroup_handle;
#define EVENTBIT_0  (1 << 0)
#define EVENTBIT_1  (1 << 1)
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
    taskENTER_CRITICAL();               /* 进入临界区 */
    eventgroup_handle = xEventGroupCreate();
    if(eventgroup_handle != NULL)
    {
        printf("事件标志组创建成功！！\r\n");
    }
    
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

/* 任务一，释放二值信号量 */
void task1( void * pvParameters )
{
    uint8_t key = 0;
    while(1) 
    {
        key = key_scan(0);
        if(key == KEY0_PRES)
        {
            xEventGroupSetBits( eventgroup_handle, EVENTBIT_0); /* 将事件标志组的bit0位置1 */
        }else if(key == KEY1_PRES)
        {
            xEventGroupSetBits( eventgroup_handle, EVENTBIT_1); /* 将事件标志组的bit1位置1 */
        }
        vTaskDelay(10);
    }
}

/* 任务二，获取二值信号量 */
void task2( void * pvParameters )
{
    EventBits_t event_bit = 0;
    while(1)
    {
        event_bit = xEventGroupWaitBits( eventgroup_handle,         /* 事件标志组句柄 */
                                         EVENTBIT_0 | EVENTBIT_1,   /* 等待事件标志组的bit0和bit1位 */
                                         pdTRUE,                    /* 成功等待到事件标志位后，清除事件标志组中的bit0和bit1位 */
                                         pdTRUE,                    /* 等待事件标志组的bit0和bit1位都置1,就成立 */
                                         portMAX_DELAY );           /* 死等 */
        printf("等待到的事件标志位值为：%#x\r\n",event_bit);
    }
}
