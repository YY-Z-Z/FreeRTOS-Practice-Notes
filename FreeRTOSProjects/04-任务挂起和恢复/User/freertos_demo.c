#include "freertos_demo.h"
#include "../Drivers/SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
/*FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"

/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   128
TaskHandle_t start_task_handler;
void start_task( void * pvParameters );

/* TASK1 任务 配置 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
void task1( void * pvParameters );

/* TASK2 任务 配置 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t task2_handler;
void task2( void * pvParameters );

/* TASK3 任务 配置 */
#define TASK3_PRIO         4
#define TASK3_STACK_SIZE   128
TaskHandle_t task3_handler;
void task3( void * pvParameters );

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
                
    xTaskCreate((TaskFunction_t         )   task3,
                (char *                 )   "task3",
                (configSTACK_DEPTH_TYPE )   TASK3_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK3_PRIO,
                (TaskHandle_t *         )   &task3_handler );
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，实现LED0每500ms翻转一次 */
void task1( void * pvParameters )
{
	uint32_t task1_num=0;
    while(1)
    {
			printf("task1_num:%d\r\n",++task1_num);
        LED0_TOGGLE();
        vTaskDelay(500);
    }
}

/* 任务二，实现LED1每500ms翻转一次 */
void task2( void * pvParameters )
{
	uint32_t task2_num=0;
    while(1)
    {
        printf("task2_num:%d\r\n",++task2_num);
        LED1_TOGGLE();
        vTaskDelay(500);
    }
}

/* 任务三 */
void task3( void * pvParameters )
{
    uint8_t key = 0;
    while(1)
    {
        key = key_scan(0);
        if(key == KEY0_PRES)
        {
					printf("挂起task1\r\n");
					vTaskSuspend(task1_handler);
        }else if(key==KEY1_PRES)
				{
					printf("在任务中恢复task1\r\n");
					vTaskResume(task1_handler);
				}
        vTaskDelay(10);
    }
}


