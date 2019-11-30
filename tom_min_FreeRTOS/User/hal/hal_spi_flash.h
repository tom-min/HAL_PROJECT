#ifndef __HAL_SPI_FLASH_
#define __HAL_SPI_FLASH_


#include "spi_flash.h"
#include "stdint.h"


#define IPHONE_INFO_SECTOR_ADDR	0x00000000
#define IPHONE_INFO_MAX_SECTOR		256

#define HUAWEI_INFO_SECTOR_ADDR	0x00100000
#define HUAWEI_INFO_MAX_SECTOR		256

#define XIAOMI_INFO_SECTOR_ADDR	0x00200000
#define XIAOMI_INFO_MAX_SECTOR		256

#define OPPO_INFO_SECTOR_ADDR		0x00300000
#define OPPO_INFO_MAX_SECTOR		256

#define VIVO_INFO_SECTOR_ADDR		0x00400000
#define VIVO_INFO_MAX_SECTOR		256

#define OTHER_INFO_SECTOR_ADDR		0x00500000
#define OTHER_INFO_MAX_SECTOR		256


typedef struct{
	char Phone_model[20];
	char Vendor[20];
	char Production_Date[20];
	char Phone_RAM[10];
	char Phone_ROM[10];
}Phone_Info;



#endif


