#include "mpu6050.h"

#include "i2c.h"
#include "newi2c.h"

#define MPU_ERROR I2C_ERROR
#define MPU_INFO I2C_INFO

void MPU6050_Config(void) { I2C1_Init(); }

/**
 * @brief   д���ݵ�MPU6050�Ĵ���
 * @param   reg_add:�Ĵ�����ַ
 * @param	 reg_data:Ҫд�������
 * @retval
 */
void MPU6050_WriteReg(u8 reg_add, u8 reg_data) {
	I2C_WriteByte(I2C1,MPU6050_ADDRESS<<1,reg_add,reg_data);
 //   Sensors_I2C_WriteRegister(MPU6050_ADDRESS, reg_add, 1, &reg_data);
}

/**
 * @brief   ��MPU6050�Ĵ�����ȡ����
 * @param   reg_add:�Ĵ�����ַ
 * @param	 Read���洢���ݵĻ�����
 * @param	 num��Ҫ��ȡ��������
 * @retval
 */
void MPU6050_ReadData(u8 reg_add, unsigned char *Read, u8 num) {
	I2C_ReadData(I2C1,MPU6050_ADDRESS<<1,reg_add,Read,num);
//    Sensors_I2C_ReadRegister(MPU6050_ADDRESS, reg_add, num, Read);
}

/**
 * @brief   ��ʼ��MPU6050оƬ
 * @param
 * @retval
 */
void MPU6050_Init(void) {
    int i = 0, j = 0;
    //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
    for (i = 0; i < 1000; i++) {
        for (j = 0; j < 1000; j++) {
            ;
        }
    }
    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);   //�������״̬
    MPU6050_WriteReg(MPU6050_RA_INT_ENABLE, 0X00);   //�ر������ж�
    MPU6050_WriteReg(MPU6050_RA_USER_CTRL, 0X00);    // I2C��ģʽ�ر�
    MPU6050_WriteReg(MPU6050_RA_FIFO_EN, 0X00);      //�ر�FIFO
    MPU6050_WriteReg(MPU6050_RA_INT_PIN_CFG, 0X80);  // INT���ŵ͵�ƽ��Ч

    MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV, 0x07);  //�����ǲ�����
    MPU6050_WriteReg(MPU6050_RA_CONFIG, 0x06);
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG,
                     0x01);  //���ü��ٶȴ�����������16Gģʽ
    MPU6050_WriteReg(
        MPU6050_RA_GYRO_CONFIG,
        0x18);  //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}

/**
 * @brief   ��ȡMPU6050��ID
 * @param
 * @retval  ��������1���쳣����0
 */
uint8_t MPU6050ReadID(void) {
    unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I, &Re, 1);  //��������ַ
    if (Re != 0x68) {
        MPU_ERROR(
            "MPU6050 dectected "
            "error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
        return 0;
    } else {
        MPU_INFO("MPU6050 ID = %d\r\n", Re);
        return 1;
    }
}

/**
 * @brief   ��ȡMPU6050�ļ��ٶ�����
 * @param
 * @retval
 */
void MPU6050ReadAcc(short *accData) {
    u8 buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
 * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
 * @param
 * @retval
 */
void MPU6050ReadGyro(short *gyroData) {
    u8 buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT, buf, 6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
 * @brief   ��ȡMPU6050��ԭʼ�¶�����
 * @param
 * @retval
 */
void MPU6050ReadTemp(short *tempData) {
    u8 buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H, buf, 2);  //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}

/**
 * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
 * @param
 * @retval
 */
void MPU6050_ReturnTemp(float *Temperature) {
    short temp3;
    u8 buf[2];

    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H, buf, 2);  //��ȡ�¶�ֵ
    temp3 = (buf[0] << 8) | buf[1];
    *Temperature = ((double)temp3 / 340.0) + 36.53;
}
