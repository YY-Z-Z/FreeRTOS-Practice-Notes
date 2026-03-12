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

void timer1_callback( TimerHandle_t pxTimer );
void timer2_callback( TimerHandle_t pxTimer );

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

TimerHandle_t timer1_handle = 0;    /* 单次定时器 */
TimerHandle_t timer2_handle = 0;    /* 周期定时器 */

void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* 进入临界区 */
    
    /* 单次定时器 */
    timer1_handle = xTimerCreate( "timer1", 
                                    500,
                                    pdFALSE,
                                    (void *)1,
                                    timer1_callback );
                            
    /* 周期定时器 */
    timer2_handle = xTimerCreate( "timer2", 
                                    2000,
                                    pdTRUE,
                                    (void *)2,
                                    timer2_callback );
                                    
    xTaskCreate((TaskFunction_t         )   task1,
                (char *                 )   "task1",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIO,
                (TaskHandle_t *         )   &task1_handler );
                                             
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，按键扫描并控制软件定时器 */
void task1( void * pvParameters )
{
    uint8_t key = 0;
    while(1) 
    {
        key = key_scan(0);
        if(key == KEY0_PRES)
        {
            xTimerStart(timer1_handle,portMAX_DELAY);
            xTimerStart(timer2_handle,portMAX_DELAY);
        }else if(key == KEY1_PRES)
        {
            xTimerStop(timer1_handle,portMAX_DELAY);
            xTimerStop(timer2_handle,portMAX_DELAY);
        }
        vTaskDelay(10);
    }
}

/* timer1的超时回调函数 */
void timer1_callback( TimerHandle_t pxTimer )
{
    static uint32_t timer = 0;
    printf("timer1的运行次数：%d\r\n",++timer);
}

/* timer2的超时回调函数 */
void timer2_callback( TimerHandle_t pxTimer )
{
    static uint32_t timer = 0;
    printf("timer2的运行次数：%d\r\n",++timer);
}


