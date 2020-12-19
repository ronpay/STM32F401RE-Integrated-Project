#ifndef __HMC5883L_H
#define __HMC5883L_H

#include "stm32f4xx.h"

#define MPU_CFG 0x37  //�üĴ���������ļĴ���������������MPU��ͨģʽ
#define MPU_CTRL 0x6A
#define HMC_CONFIGA 0x00    //���üĴ���A
#define HMC_CONFIGB 0x01    //���üĴ���B
#define HMC_MODE 0x02       //ģʽ�Ĵ���
#define HMC_DATA_XMSB 0x03  //���ݼĴ���X��λ
#define HMC_DATA_XLSB 0x04  //���ݼĴ���X��λ
#define HMC_DATA_ZMSB 0x05  //���ݼĴ���Z��λ
#define HMC_DATA_ZLSB 0x06  //���ݼĴ���Z��λ
#define HMC_DATA_YMSB 0x07  //���ݼĴ���Y��λ
#define HMC_DATA_YLSB 0x08  //���ݼĴ���Y��λ
#define HMC_STATUS 0x09     //״̬�Ĵ���
#define HMC_RECOGA 0x0A     //ʶ��Ĵ���A
#define HMC_RECOGB 0x0B     //ʶ��Ĵ���B
#define HMC_RECOGC 0x0C     //ʶ��Ĵ���C
#define MPU_ADD 0xD0        // MPU��i2c��ַ
#define HMC_WRITE 0x3C      // HMC��i2cд��ַ
#define HMC_READ 0x3D       // HMC��i2c����ַ
#define HMC_ADD 0x3C

void HMC_Init(void);
void GY86_Init(void);
// void READ_HMCALL(u16 *n);
void HMC5884LReadMe(short *MeData);
#endif
