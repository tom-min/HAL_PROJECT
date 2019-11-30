/**
  ******************************************************************************
  * @file    SPI/SPI_FLASH/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "stm322xg_eval.h"
#include "spi_flash.h"
#include "stdio.h"


/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main_SPI(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */     
       
 	/* Initialize Leds mounted on STM322xG-EVAL board */


	/* Initialize the SPI FLASH driver */
	sFLASH_Init();

  	/* Check the SPI Flash ID */
	if (sFLASH_ReadID() == W25Q16)
	{
		printf("FlashID Success\r\n");
		return PASSED;
	}
	else
	{
		/* Error*/
		printf("FlashID fail\r\n");
		return FAILED;
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
