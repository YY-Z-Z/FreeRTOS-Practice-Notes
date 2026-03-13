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

/* 任务一，LED每500ms翻转一次 */
void task1( void * pvParameters )
{

	while(1)
	{
		LED0_TOGGLE();
		vTaskDelay(500);
	}
}

/* 任务二，实现列表项的插入删除 */
void task2( void * pvParameters )
{	
	vListInitialise(&TestList); //初始化列表
	vListInitialiseItem(&ListItem1); //初始化列表项
	vListInitialiseItem(&ListItem2); //初始化列表项
	vListInitialiseItem(&ListItem2); //初始化列表项
	ListItem1.xItemValue=40;
	ListItem2.xItemValue=60;
	ListItem3.xItemValue=50;
	
	printf("******************第二步：打印列表和列表项的地址******************\r\n");
	printf("项目t\t地址\r\n", &TestList);
	printf("TestList\t0x%p\r\n", &TestList);
	printf("TestList->pxIndex\t0x%p\r\n", TestList.pxIndex);
	printf("TestList->xListEnd\t0x%p\r\n", &(TestList.xListEnd));
	printf("ListItem1\t0x%p\r\n", &ListItem1);
	printf("ListItem2\t0x%p\r\n", &ListItem2);
	printf("ListItem3\t0x%p\r\n", &ListItem3);
	printf("************************************************************\r\n");
	
	printf("\r\n/*****************第三步：列表项1插入列表******************/\r\n");
	vListInsert((List_t*    )&TestList,         /* 列表 */
							(ListItem_t*)&ListItem1);       /* 列表项 */
	printf("项目\t\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("/**************************结束***************************/\r\n");
	
	/* 第四步：列表项2插入列表 */
	printf("\r\n/*****************第四步：列表项2插入列表******************/\r\n");
	vListInsert((List_t*    )&TestList,         /* 列表 */
							(ListItem_t*)&ListItem2);       /* 列表项 */
	printf("项目\t\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
	printf("/**************************结束***************************/\r\n");
	
	/* 第五步：列表项3插入列表 */
	printf("\r\n/*****************第五步：列表项3插入列表******************/\r\n");
	vListInsert((List_t*    )&TestList,         /* 列表 */
							(ListItem_t*)&ListItem3);       /* 列表项 */
	printf("项目\t\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
	printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
	printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
	printf("/**************************结束***************************/\r\n");
	
	/* 第六步：移除列表项2 */
	printf("\r\n/*******************第六步：移除列表项2********************/\r\n");
	uxListRemove((ListItem_t*   )&ListItem2);   /* 移除列表项 */
	printf("项目\t\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
	printf("/**************************结束***************************/\r\n");
	
	/* 第七步：列表末尾添加列表项2 */
	printf("\r\n/****************第七步：列表末尾添加列表项2****************/\r\n");
	TestList.pxIndex = &ListItem1;
	vListInsertEnd((List_t*     )&TestList,     /* 列表 */
								 (ListItem_t* )&ListItem2);   /* 列表项 */
	printf("项目\t\t\t\t地址\r\n");
	printf("TestList->pxIndex\t\t0x%p\r\n", TestList.pxIndex);
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
	printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
	printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
	printf("/************************实验结束***************************/\r\n");
	
	while(1)
	{
		
		vTaskDelay(500);
	}
}

