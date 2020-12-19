#include "hmc5883l.h"

#include "i2c.h"
#include "mpu6050.h"
#include "stdio.h"
#include "sysTick.h"

void HMC5883L_WriteReg(u8 reg_add, u8 reg_dat) {
    I2C_SAND_BYTE(HMC_WRITE, reg_add, reg_dat);
}

void HMC5883L_ReadData(u8 reg_add, unsigned char* Read, u8 num) {
    I2C_READ_BUFFER(HMC_READ, reg_add, Read, 6);
}

// GY-86��ʼ������
void GY86_Init(void) {
    MPU6050_Config();
    MPU6050_Init();
    MPU6050_WriteReg(
        MPU_CFG,
        0x02);  //��MPU��CFG�Ĵ����ĵڶ�λ����Ϊ1������λ��ʹ��MPUʱ����
    MPU6050_WriteReg(
        MPU_CTRL,
        0x00);  //��MPU��CTRL�Ĵ����ĵ���λ����Ϊ0��������һ����ͬ����bypassģʽ
    Delay_ms(200);
    HMC_Init();  // HMC��ʼ��
}

// HMC��ʼ������
void HMC_Init(void) {
    HMC5883L_WriteReg(HMC_CONFIGA,
                      0x50);  // 01010000/����ƽ����4���������15��������������
    HMC5883L_WriteReg(HMC_CONFIGB, 0xE0);  // 11100000/�����������С
    HMC5883L_WriteReg(HMC_MODE, 0x00);     // 00000000/����Ϊ����ģʽ
}

void HMC5884LReadMe(short* MeData) {
    u8 buf[6];
    // HMC5883L_ReadData(HMC_DATA_XMSB,buf,6);
    HMC5883L_ReadData(HMC_DATA_XMSB, buf, 6);
    MeData[0] = (buf[0] << 8) | buf[1];
    MeData[1] = (buf[2] << 8) | buf[3];
    MeData[2] = (buf[4] << 8) | buf[5];
}
