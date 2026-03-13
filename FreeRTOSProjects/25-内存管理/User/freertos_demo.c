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

    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，申请内存以及释放内存，并显示空闲内存大小 */
void task1( void * pvParameters )
{
    uint8_t key = 0, t = 0;
    uint8_t * buf = NULL;
    while(1) 
    {
        key = key_scan(0);
        if(key == KEY0_PRES)
        {
            buf = pvPortMalloc(30);                 /* 申请内存 */
            if(buf != NULL)
            {
                printf("申请内存成功！\r\n");
            }else printf("申请内存失败\r\n");
        }else if(key == KEY1_PRES)
        {
            if(buf != NULL)
            {
                vPortFree(buf);                     /* 释放内存 */
                printf("释放内存！！\r\n");
            }                
        }
        if(t++ > 50)
        {
            t = 0;
            printf("剩余的空闲内存大小为：%d\r\n",xPortGetFreeHeapSize());
        }
        vTaskDelay(10);
    }
}
