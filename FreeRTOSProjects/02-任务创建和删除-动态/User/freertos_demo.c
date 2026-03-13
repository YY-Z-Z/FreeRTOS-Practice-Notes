#include "freertos_demo.h"
#include "../Drivers/SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
/*FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"

/* START_TASK 任务 配置 */
#define START_TASK_PRIO 1 //任务优先级
#define STAR_TASK_STACK_SIZE 128 //单位是字节
TaskHandle_t start_task_handler; //任务句柄
void start_task( void * pvParameters );

/* TASK1 任务 配置 */
#define TASK1_PRIO 2 //任务优先级
#define TASK1_STACK_SIZE 128 //单位是字节
TaskHandle_t task1_handler; //任务句柄
void task1( void * pvParameters );

/* TASK2 任务 配置 */
#define TASK2_PRIO 3 //任务优先级
#define TASK2_STACK_SIZE 128 //单位是字节
TaskHandle_t task2_handler; //任务句柄
void task2( void * pvParameters );

/* TASK3 任务 配置 */
#define TASK3_PRIO 4 //任务优先级
#define TASK3_STACK_SIZE 128 //单位是字节
TaskHandle_t task3_handler; //任务句柄
void task3( void * pvParameters );


/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
	  //用()进行强转可以防止报警告
	  //创建开始任务start_task
    xTaskCreate( (TaskFunction_t         ) start_task,
                 (char *                 ) "start_task",
                 (configSTACK_DEPTH_TYPE ) STAR_TASK_STACK_SIZE,
                 (void *                 ) NULL,
                 (UBaseType_t            ) START_TASK_PRIO,
                 (TaskHandle_t *         ) &task1_handler
               );
	 vTaskStartScheduler(); //启动实时内核调度器
}

/**
 * @brief       开始任务，实现创建其他的三个任务
 * @param       无
 * @retval      无
 */
void start_task( void * pvParameters )
{
	  //taskENTER_CRITICAL(); //进入临界区
		//创建task1
		xTaskCreate( (TaskFunction_t         ) task1,
                 (char *                 ) "task1",
                 (configSTACK_DEPTH_TYPE ) TASK1_STACK_SIZE,
                 (void *                 ) NULL,
                 (UBaseType_t            ) TASK1_PRIO,
                 (TaskHandle_t *         ) &task1_handler
               );
								 
		//创建task2
		xTaskCreate( (TaskFunction_t         ) task2,
                 (char *                 ) "task2",
                 (configSTACK_DEPTH_TYPE ) TASK2_STACK_SIZE,
                 (void *                 ) NULL,
                 (UBaseType_t            ) TASK2_PRIO,
                 (TaskHandle_t *         ) &task2_handler
               );
		
		//创建task3
		xTaskCreate( (TaskFunction_t         ) task3,
                 (char *                 ) "task3",
                 (configSTACK_DEPTH_TYPE ) TASK3_STACK_SIZE,
                 (void *                 ) NULL,
                 (UBaseType_t            ) TASK3_PRIO,
                 (TaskHandle_t *         ) &task3_handler
               );
								 
		//删除自己
		vTaskDelete(NULL);
		//taskEXIT_CRITICAL(); //退出临界区——临界区保护，就是保护那些不想被打断的程序段
}

/*任务一：实现LED0每500ms翻转一次*/
void task1( void * pvParameters )
{
		while(1)
		{
			printf("task1正在运行！！！\r\n");
			LED0_TOGGLE(); //LED翻转
			vTaskDelay(500);
		}
}

/*任务二：实现LED1每500ms翻转一次*/
void task2( void * pvParameters )
{
		printf("task2正在运行！！！\r\n");
		while(1)
		{
			LED0_TOGGLE();
			vTaskDelay(500);
		}
}

/*任务三：判断按键KEY0是否按下，按下则删除task1*/
void task3( void * pvParameters )
{
		printf("task3正在运行！！！\r\n");
		uint8_t key = 0;
		while(1)
		{
			key=key_scan(0);
			if(key==KEY0_PRES)
			{
				if(task1_handler!=NULL) //防止重复删除报错
				{
					printf("删除task1\r\n");
					vTaskDelete(task1_handler);				
			
				}
			}
			vTaskDelay(10);
		}
}


