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

/* 任务一，发送任务通知值 */
void task1( void * pvParameters )
{
    uint8_t key = 0;
    
    while(1) 
    {
        key = key_scan(0);
        if((key != 0) && (task2_handler != NULL))
        {
            printf("任务通知模拟消息邮箱发送，发送的键值为：%d\r\n",key);
            xTaskNotify( task2_handler, key, eSetValueWithOverwrite );
        }
        vTaskDelay(10);
    }
}

/* 任务二，接收任务通知值 */
void task2( void * pvParameters )
{
    uint32_t noyify_val = 0;
    while(1)
    {
        xTaskNotifyWait( 0, 0xFFFFFFFF, &noyify_val, portMAX_DELAY );
        switch(noyify_val)
        {
            case KEY0_PRES:
            {
                printf("接收到的通知值为：%d\r\n",noyify_val);
                LED0_TOGGLE();
                break;
            }
            case KEY1_PRES:
            {
                printf("接收到的通知值为：%d\r\n",noyify_val);
                LED1_TOGGLE();
                break;
            }
            default : break;
        }
    }
}
