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
TaskHandle_t    start_task_handler;
StackType_t     start_task_stack[START_TASK_STACK_SIZE];
StaticTask_t    start_task_tcb;
void start_task( void * pvParameters );

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
StackType_t     task1_stack[TASK1_STACK_SIZE];
StaticTask_t    task1_tcb;
void task1( void * pvParameters );

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
StackType_t     task2_stack[TASK2_STACK_SIZE];
StaticTask_t    task2_tcb;
void task2( void * pvParameters );

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO         4
#define TASK3_STACK_SIZE   128
TaskHandle_t    task3_handler;
StackType_t     task3_stack[TASK3_STACK_SIZE];
StaticTask_t    task3_tcb;
void task3( void * pvParameters );
/******************************************************************************************************/
/* 空闲任务配置 */
StaticTask_t idle_task_tcb;
StackType_t  idle_task_stack[configMINIMAL_STACK_SIZE];

/* 软件定时器任务配置 */
StaticTask_t timer_task_tcb;
StackType_t  timer_task_stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务内存分配 */
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
    * ppxIdleTaskTCBBuffer = &idle_task_tcb;
    * ppxIdleTaskStackBuffer = idle_task_stack;
    * pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


/* 软件定时器内存分配 */
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    * ppxTimerTaskTCBBuffer = &timer_task_tcb;
    * ppxTimerTaskStackBuffer = timer_task_stack;
    * pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
    
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
    start_task_handler = xTaskCreateStatic( (TaskFunction_t )   start_task,
                                            (char *         )   "start_task", 
                                            (uint32_t       )   START_TASK_STACK_SIZE,
                                            (void *         )   NULL,
                                            (UBaseType_t    )   START_TASK_PRIO,
                                            (StackType_t *  )   start_task_stack,
                                            (StaticTask_t * )   &start_task_tcb );
    vTaskStartScheduler();
}


void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* 进入临界区 */
    
    task1_handler = xTaskCreateStatic(  (TaskFunction_t )   task1,
                                        (char *         )   "task1", 
                                        (uint32_t       )   TASK1_STACK_SIZE,
                                        (void *         )   NULL,
                                        (UBaseType_t    )   TASK1_PRIO,
                                        (StackType_t *  )   task1_stack,
                                        (StaticTask_t * )   &task1_tcb );
                        
    task2_handler = xTaskCreateStatic(  (TaskFunction_t )   task2,
                                        (char *         )   "task2", 
                                        (uint32_t       )   TASK2_STACK_SIZE,
                                        (void *         )   NULL,
                                        (UBaseType_t    )   TASK2_PRIO,
                                        (StackType_t *  )   task2_stack,
                                        (StaticTask_t * )   &task2_tcb );
                        
    task3_handler = xTaskCreateStatic(  (TaskFunction_t )   task3,
                                        (char *         )   "task3", 
                                        (uint32_t       )   TASK3_STACK_SIZE,
                                        (void *         )   NULL,
                                        (UBaseType_t    )   TASK3_PRIO,
                                        (StackType_t *  )   task3_stack,
                                        (StaticTask_t * )   &task3_tcb );
    
    vTaskDelete(start_task_handler);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，实现LED0每500ms翻转一次 */
void task1( void * pvParameters )
{
    while(1)
    {
        printf("task1正在运行！！！\r\n");
        LED0_TOGGLE();
        vTaskDelay(500);
    }
}

/* 任务二，实现LED1每500ms翻转一次 */
void task2( void * pvParameters )
{
    while(1)
    {
        printf("task2正在运行！！！\r\n");
        LED1_TOGGLE();
        vTaskDelay(500);
    }
}

/* 任务三，判断按键KEY0，按下KEY0删除task1 */
void task3( void * pvParameters )
{
    uint8_t key = 0;
    while(1)
    {
        printf("task3正在运行！！！\r\n");
        key = key_scan(0);
        if(key == KEY0_PRES)
        {
            if(task1_handler != NULL)
            {
                printf("删除task1任务\r\n");
                vTaskDelete(task1_handler);
                task1_handler = NULL;
            }

        }
        vTaskDelay(10);
    }
}


