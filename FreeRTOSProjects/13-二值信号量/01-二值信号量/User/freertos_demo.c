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
TaskHandle_t  start_task_handler;
void start_task(void *pvParameters);

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t  task1_handler;
void task1(void * pvParameters );


/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
void task2( void * pvParameters );

/* 二值信号量句柄 - 用于引用创建的二值信号量 */
QueueHandle_t semphore_handle;  /* 注意：信号量句柄实际上就是队列句柄 */

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
    /* 动态创建二值信号量
     * 创建成功：返回非NULL句柄
     * 创建失败：返回NULL（通常是因为内存不足）
     * 注意：新创建的二值信号量初始为0，必须先释放才能获取
     */
    semphore_handle = xSemaphoreCreateBinary();
    
    /* 检查信号量是否创建成功 */
    if(semphore_handle != NULL)
    {
        printf("二值信号量创建成功！！！\r\n");
    }
    
    /* 创建启动任务 */
    xTaskCreate((TaskFunction_t         )   start_task,
                (char *                 )   "start_task",
                (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   START_TASK_PRIO,
                (TaskHandle_t *         )   &start_task_handler );
    
    /* 启动任务调度器 - 开始多任务调度 */
    vTaskStartScheduler();
}

/**
 * 启动任务：用于创建其他应用任务
 */
void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* 进入临界区，防止创建任务过程中被中断 */
    
    /* 创建任务1：用于释放信号量 */
    xTaskCreate((TaskFunction_t         )   task1,
                (char *                 )   "task1",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIO,
                (TaskHandle_t *         )   &task1_handler );
    
    /* 创建任务2：用于获取信号量 */            
    xTaskCreate((TaskFunction_t         )   task2,
                (char *                 )   "task2",
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIO,
                (TaskHandle_t *         )   &task2_handler );
    
    /* 删除启动任务自身 */
    vTaskDelete(NULL);
    
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/**
 * 任务一：释放二值信号量
 * 功能：检测按键，当按键按下时释放信号量
 */
void task1( void * pvParameters )
{
    uint8_t key = 0;
    BaseType_t err;  /* 用于存储函数返回值 */
    
    while(1) 
    {
        /* 扫描按键（假设key_scan是按键扫描函数）
         * 参数0表示非阻塞模式
         */
        key = key_scan(0);
        
        /* 检测到KEY0被按下 */
        if(key == KEY0_PRES)
        {
            /* 确保信号量句柄有效 */
            if(semphore_handle != NULL)
            {
                /* 释放信号量
                 * 功能：将信号量计数值从0变为1
                 * 注意：不能在中断中使用此函数，应使用xSemaphoreGiveFromISR()
                 * 返回值：pdPASS(成功) 或 errQUEUE_FULL(失败)
                 */
                err = xSemaphoreGive(semphore_handle);
                
                /* 检查释放结果 */
                if(err == pdPASS)
                {
                    printf("信号量释放成功！！\r\n");
                }
                else  /* 释放失败通常是因为信号量已经处于释放状态 */
                {
                    printf("信号量释放失败！！\r\n");
                }
            }
        }
        /* 延时10ms，避免按键抖动检测和CPU占用过高 */
        vTaskDelay(10);
    }
}

/**
 * 任务二：获取二值信号量
 * 功能：等待信号量，获取成功后打印信息
 */
void task2( void * pvParameters )
{
    uint32_t i = 0;          /* 用于计数超时次数 */
    BaseType_t err;          /* 用于存储函数返回值 */
    
    while(1)
    {
        /* 获取信号量，使用portMAX_DELAY无限等待
         * 参数1：信号量句柄
         * 参数2：阻塞时间 - portMAX_DELAY表示一直等待直到获取到信号量
         * 工作原理：
         *   - 如果信号量可用（计数值>0），立即获取成功，计数值减1
         *   - 如果信号量不可用（计数值=0），任务进入阻塞状态
         *   - 当其他任务释放信号量时，此任务被唤醒并获取信号量
         * 返回值：pdTRUE(成功) 或 pdFALSE(超时失败)
         */
        err = xSemaphoreTake(semphore_handle, portMAX_DELAY);
        
        /* 检查获取结果 */
        if(err == pdTRUE)
        {
            /* 成功获取到信号量 */
            printf("获取信号量成功\r\n");
        }
        else  /* pdFALSE - 超时（本例中使用portMAX_DELAY，理论上不会进入此分支） */
        {
            printf("已超时%d\r\n", ++i);
        }
    }
}
