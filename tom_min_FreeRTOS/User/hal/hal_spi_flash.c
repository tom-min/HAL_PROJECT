#include "hal_spi_flash.h"



//SectorAddr必须为4K的整数倍，不然这个函数有问题
void hal_flash_EraseSector(uint32_t SectorAddr,uint16_t SectorCnt)
{
	uint16_t i;
	uint32_t current_SectorAddr = 0;
	
	//最大6M
	if(SectorAddr >= 0x600000)
	{
		return;
	}

	for(i=0;i<SectorCnt;i++)
	{
		current_SectorAddr = SectorAddr + i*sFLASH_SPI_SECTORSIZE;
		sFLASH_EraseSector(current_SectorAddr);
	}
}


void hal_flash_writebuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	hal_flash_EraseSector(WriteAddr,10);
	
	sFLASH_WriteBuffer(pBuffer,WriteAddr,NumByteToWrite);
}


void hal_flash_readbuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	sFLASH_ReadBuffer(pBuffer,WriteAddr,NumByteToWrite);
}


