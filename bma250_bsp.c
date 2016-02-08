/*
YYJ@2014
obddev.taobao.com
*/

#include "BMA250_bsp.h"
#include "stm32f10x_spi.h"
u8 id=0;


#define CS_ENABLE()	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
#define CS_DISABLE()	GPIO_SetBits(GPIOA,GPIO_Pin_4);

void BMA250_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
  // Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------
  // Confugure SCK and MOSI pins as Alternate Function Push Pull
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	CS_DISABLE();
	
  // Confugure MISO pin as Input Floating
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	BMA250_WriteByte(PMU_RANGE, ACC_RANGE_2G);
	
	id=BMA250_ReadByte(BGW_CHIPID);
	//id should be 0xF9
	
	
}
//读取加速度传感器的值 
void BMA250_ReadAcc(s16 *pX,s16 *pY,s16 *pZ)
{
	s8 dataout[6]={0,0,0,0,0,0};
	u8 i=0;
	CS_ENABLE();
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
	SPI_I2S_SendData(SPI1, 0x80|ACCD_X_LSB);//register addr,读之前最高位要置

1
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}
  SPI_I2S_ReceiveData(SPI1);
	
	for(i = 0; i<6; i++)
  {
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    dataout[i] = SPI_I2S_ReceiveData(SPI1);
  }
	CS_DISABLE();
	
	*pX = ( (((u8)dataout[0]) >> 6) | ((s16)(dataout[1]) << 2) );
	*pY = ( (((u8)dataout[2]) >> 6) | ((s16)(dataout[3]) << 2) );
	*pZ = ( (((u8)dataout[4]) >> 6) | ((s16)(dataout[5]) << 2) );
}

//向一个地址写入数据
void BMA250_WriteByte(u8 addr,u8 data)
{		
		CS_ENABLE();
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
    SPI_I2S_SendData(SPI1, addr);
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}
		SPI_I2S_ReceiveData(SPI1);
	
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
    SPI_I2S_SendData(SPI1, data);
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}
		SPI_I2S_ReceiveData(SPI1);
		CS_DISABLE();
}
//读一个地址
u8 BMA250_ReadByte(u8 addr) 
{
	  u8 RxData = 0;
    CS_ENABLE();
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
    SPI_I2S_SendData(SPI1, 0x80|addr);
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}
    SPI_I2S_ReceiveData(SPI1);
	
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
    SPI_I2S_SendData(SPI1, 0);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}
    RxData = SPI_I2S_ReceiveData(SPI1);
		CS_DISABLE();
    return (u8)RxData;
}
