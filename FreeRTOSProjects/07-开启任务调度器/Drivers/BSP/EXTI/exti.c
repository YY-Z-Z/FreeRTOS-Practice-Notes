/*
外部中断 驱动代码
 */

#include "../Drivers/SYSTEM/sys/sys.h"
#include "../Drivers/SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/EXTI/exti.h"
#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t task1_handler;//全局定义句柄
/**
 * @brief       KEY2 外部中断服务程序
 * @param       无
 * @retval      无
 */
void KEY2_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(KEY2_INT_GPIO_PIN);        /* 调用中断处理公用函数 清除KEY2所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_INT_GPIO_PIN);        /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

/**
 * @brief       中断服务程序中需要做的事情
 *              在HAL库中所有的外部中断服务函数都会调用此函数
 * @param       GPIO_Pin:中断引脚号
 * @retval      无
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);      /* 消抖 */
    switch(GPIO_Pin)
    {
			BaseType_t xYieldRequired;
        case KEY2_INT_GPIO_PIN:
            if (KEY2 == 0)
            {
                xYieldRequired=xTaskResumeFromISR(task1_handler); //中断中恢复被挂起的函数
								printf("在中断中恢复task1\r\n");
						}
						if(xYieldRequired==pdTRUE)
						{
								portYIELD_FROM_ISR(xYieldRequired); //进行任务切换
						}
            break;
        default : break;
    }
}

/**
 * @brief       外部中断初始化程序
 * @param       无
 * @retval      无
 */
void extix_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    key_init();
    
    gpio_init_struct.Pin = KEY2_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 下降沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &gpio_init_struct);    /* KEY2配置为下降沿触发中断 */
    
    HAL_NVIC_SetPriority(KEY2_INT_IRQn, 2, 2);               /* 抢占2，子优先级2 */
    HAL_NVIC_EnableIRQ(KEY2_INT_IRQn);                       /* 使能中断线2 */
}












