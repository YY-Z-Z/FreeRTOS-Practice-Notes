#include "freertos_demo.h"
#include "../Drivers/SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
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
TaskHandle_t    task2_handler;
void task2( void * pvParameters );

List_t TestKist; //定义测试列表
ListItem_t ListItem1; //定义测试列表项
ListItem_t ListItem2;
ListItem_t ListItem3;

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

/* 任务一 */
void task1( void * pvParameters )
{
	uint32_t task1_num=0;
	while(1)
	{
		taskENTER_CRITICAL();               /* 进入临界区 */
		printf("task1运行次数：%d\r\n",++task1_num);
		taskEXIT_CRITICAL();                /* 退出临界区 */
		delay_ms(10);
	}
}

/* 任务二，实现列表项的插入删除 */
void task2( void * pvParameters )
{		
	uint32_t task2_num=0;
	while(1)
	{
		taskENTER_CRITICAL();               /* 进入临界区 */
		printf("task2运行次数：%d\r\n",++task2_num);
		taskEXIT_CRITICAL();                /* 退出临界区 */
		delay_ms(10);
	}
}

