/*
YYJ@2014
obddev.taobao.com
*/
#ifndef __BMA250_BSP_H
#define __BMA250_BSP_H


#include "stm32f10x.h"

enum BMA_MODE{NORMAL=0,STANDBY,SUSPEND,LOW_POWER1,LOW_POWER2,DEEP_SUSPEND};


//read-only	0x00-0x0E
#define BGW_CHIPID 		0x00	//芯片ID 0xF9
#define ACCD_X_LSB		0x02
#define ACCD_X_MSB		0x03
#define ACCD_Y_LSB		0x04
#define ACCD_Y_MSB		0x05
#define ACCD_Z_LSB		0x06
#define ACCD_Z_MSB		0x07
#define ACCD_TEMP 		0x08 	//温度传感器地址,8位补码表示
#define INT_STATUS_0	0x09
#define INT_STATUS_1	0x0A
#define INT_STATUS_2	0x0B
#define INT_STATUS_3	0x0C
#define FIFO_STATUS		0x0E

#define PMU_RANGE			0x0F 	//0011->+-2g	0101->+-4g	1000->+-8g	1100->+-16g
#define PMU_BW				0x10
#define PWU_LPW				0x11
#define PMU_LOW_NOISE	0x12
#define ACCD_HBW			0x13
#define BGW_SOFTRESET	0x14
#define INT_EN_0			0x16
#define INT_EN_1			0x17
#define INT_EN_2			0x18
#define INT_MAP_0			0x19
#define INT_MAP_1			0x1A
#define INT_MAP_2			0x1B
#define INT_SRC				0x1E
#define INT_OUT_CTRL	0x20
#define INT_RST_LATCH	0x21
#define INT_0					0x22
#define INT_1					0x23
#define INT_2					0x24
#define INT_3					0x25
#define INT_4					0x26
#define INT_5					0x27
#define INT_6					0x28
#define INT_7					0x29
#define INT_8					0x2A
#define INT_9					0x2B
#define INT_A					0x2C
#define INT_B					0x2D
#define INT_C					0x2E
#define INT_D					0x2F
#define FIFO_CONFIG_0	0x30
#define PMU_SELF_TEST	0x32
#define TRIM_NVM_CTRL	0x33
#define BGW_SPI3_WDT	0x34
#define OFC_CTRL			0x36
#define OFC_SETTING		0x37
#define OFC_OFFSET_X	0x38
#define OFC_OFFSET_Y	0x39
#define OFC_OFFSET_Z	0x3A
#define TRIM_GP0			0x3B
#define TRIM_GP1			0x3C
#define FIFO_CONFIG_1	0x3E	//00b->bypass mode 01b->fifo mode 10b->stream mode
#define FIFO_DATA			0x3F


#define ACC_RANGE_2G                0x03    //  3.91 mg/LSB
#define ACC_RANGE_4G                0x05    //  7.81 mg/LSB
#define ACC_RANGE_8G                0x08    // 15.62 mg/LSB
#define ACC_RANGE_16G               0x0C    // 31.25 mg/LSB


void BMA250_Init(void);
void BMA250_WriteByte(u8 addr,u8 data);
u8 BMA250_ReadByte(u8 addr);
void BMA250_ReadAcc(s16 *pX,s16 *pY,s16 *pZ);

#endif

