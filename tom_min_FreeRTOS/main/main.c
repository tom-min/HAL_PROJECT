#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"

#include "Tom_Common_Api.h"
#include "use_config.h"

#include "ff.h"
#include "spi_flash.h"
#include "hal_spi_flash.h"


TaskHandle_t  data_pack_task;
TaskHandle_t  data_transfer_task;
TaskHandle_t  data_sema;

TimerHandle_t  test1Timer_Handle;

/* 互斥信号量句柄 */  
SemaphoreHandle_t xSemaphore_mutex = NULL; 

/* 二值信号量句柄 */ 
SemaphoreHandle_t xSemaphore_binary1 = NULL; 
SemaphoreHandle_t xSemaphore_binary2 = NULL; 


int SendChar (int ch)  
{
#if 1
	USART_SendData(USART1,(unsigned char)ch);        
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
#else
	
  while (!(USART1->SR & USART_FLAG_TXE)); // USART1
  USART1->DR = (ch & 0x1FF);
#endif
  return (ch);
}


void USART_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//fatal mistake:Out
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//PA9:TX
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//PA10:RX

		USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;

	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}


void test1Timer_HandleCallback(TimerHandle_t xTimer)
{			
	printf("welcome to [%s]\r\n",__func__);
}


unsigned int tom_global;


void com_fun(unsigned int data)
{
	tom_global = data;
	printf("[%s],tom_global value: %d\r\n",__func__,tom_global);
	get_task_info(data_transfer_task);
	get_task_info(data_pack_task);
}


void Data_sema1(void *pvParameters)
{
	while(1)
	{
		//获取二值信号量的值，如果无效则阻塞，第二个参数为阻塞时间
		//xSemaphoreTake(xSemaphore_binary, 0);
		//xSemaphoreTake(xSemaphore_binary, 5000);
		xSemaphoreTake(xSemaphore_binary1, portMAX_DELAY);
		printf("welcome to [%s]\r\n",__func__);
		//获得资源的使用权
		xSemaphoreTake(xSemaphore_mutex, portMAX_DELAY); 
		com_fun(188);
		//释放资源的使用权
		xSemaphoreGive(xSemaphore_mutex); 
		xSemaphoreGive(xSemaphore_binary2); 
		vTaskDelay(1000);
	}
}	


void Data_sema2(void *pvParameters)
{
	while(1)
	{
		xSemaphoreTake(xSemaphore_binary2, portMAX_DELAY);
		printf("welcome to [%s]\r\n",__func__);
		vTaskDelay(50);
		xSemaphoreTake(xSemaphore_mutex, portMAX_DELAY); 
		com_fun(199);
		xSemaphoreGive(xSemaphore_mutex); 
		vTaskDelay(1000);
	}
}
 

void Data_sema3(void *pvParameters)
{
	while(1)
	{
		printf("welcome to [%s]\r\n",__func__);
		xSemaphoreGive(xSemaphore_binary1); 
		vTaskDelay(10000);
	}
}


int main_FreeRTos(void)
{  
	BaseType_t tmp;

	printf("welcome to [%s]\r\n",__func__);

	/* 创建互斥信号量 */  
    xSemaphore_mutex = xSemaphoreCreateMutex(); 

	/* 创建二值信号量 */  
    xSemaphore_binary1 = xSemaphoreCreateBinary(); 
	xSemaphore_binary2 = xSemaphoreCreateBinary(); 

	tmp = xTaskCreate(Data_sema1, "Data_pack", 500, NULL ,1, &data_pack_task);
	if(tmp != pdPASS)
	{
		printf("Data_pack task create failed\r\n");
	}
	else
	{
		printf("Data_pack task create successfully\r\n");
	}
	
	tmp = xTaskCreate(Data_sema2, "Data_transfer", 500, NULL ,2, &data_transfer_task);
	if(tmp != pdPASS)
	{
		printf("Data_transfer task create failed\r\n");
	}
	else
	{
		printf("Data_transfer task create successfully\r\n");
	}
	
#if 1	
	tmp = xTaskCreate(Data_sema3, "Data_sema", 500, NULL ,3, &data_sema);
	if(tmp != pdPASS)
	{
		printf("Data_sema task create failed\r\n");
	}
	else
	{
		printf("Data_sema task create successfully\r\n");
	}
#endif
		
    test1Timer_Handle = xTimerCreate((const char*)"test1Timer_Handle",
    							(TickType_t	)2000,
    							(UBaseType_t)pdTRUE,
    							(void*)1,
    							(TimerCallbackFunction_t)test1Timer_HandleCallback);  
    if (test1Timer_Handle == NULL)
    {
		printf("Failed to test1Timer_Handle\r\n");
    }
    else
    {
//		xTimerStart(test1Timer_Handle,mainDONT_BLOCK);
		printf("success to test1Timer_Handle\r\n");
    }
		
    vTaskStartScheduler();
    return 0;
}


int main(void)
{   
	RCC_ClocksTypeDef RCC_Clocks;

	USART_Configure();

	main_SPI();
	
#ifdef APPLICATION
	printf("welcome to APPLICATION, VERSION is %s.\r\n",APPLICATION_VERSION);
#endif		
		
	RCC_GetClocksFreq(&RCC_Clocks);
		
	//导致任务切换不了，卡死，坑我半天
	//	if(SysTick_Config(SystemCoreClock / 1000))
	//	{ 
	//		/* 出错 */ 
	//		while (1);
	//	}
		
	printf("\r\nSYSCLK_Frequency = %d MHz\n",RCC_Clocks.SYSCLK_Frequency/1000000);
	printf("\r\nHCLK_Frequency = %d MHz\n",RCC_Clocks.HCLK_Frequency/1000000);
	printf("\r\nPCLK1_Frequency = %d MHz\n",RCC_Clocks.PCLK1_Frequency/1000000);
	printf("\r\nPCLK2_Frequency = %d MHz\n",RCC_Clocks.PCLK2_Frequency/1000000);
		
	
#ifdef 	APPLICATION
	main_FreeRTos();
#endif		
}

