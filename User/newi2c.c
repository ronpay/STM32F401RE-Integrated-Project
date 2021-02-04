#include "newi2c.h"

void I2C_WriteByte(I2C_TypeDef * I2Cx,uint8_t slave_addr,uint8_t reg_addr, uint8_t data) {
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        ;

    I2C_GenerateSTART(I2Cx, ENABLE);  //����I2Cx
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ; /*EV5,��ģʽ*/

    I2C_Send7bitAddress(I2Cx, slave_addr,
                        I2C_Direction_Transmitter);  //������ַ
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    I2C_SendData(I2Cx, reg_addr);  //�Ĵ�����ַ
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_SendData(I2Cx, data);  //��������
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_GenerateSTOP(I2Cx, ENABLE);  //�ر�I2Cx����
}

void I2C_ReadData(I2C_TypeDef * I2Cx,
    uint8_t slave_addr, uint8_t reg_addr, uint8_t *pBuffer,
    uint16_t NumByteToRead) {  // I2C��ȡ���ݴ���������ַ���Ĵ������ڲ���ַ��������
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        ;
    I2C_GenerateSTART(I2Cx, ENABLE);  //�����ź�
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ;  //��� EV5
    I2C_Send7bitAddress(I2Cx, slave_addr,
                        I2C_Direction_Transmitter);  //д��������ַ
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;  //��� EV6
    I2C_Cmd(I2Cx, ENABLE);
    I2C_SendData(I2Cx, reg_addr);  //���Ͷ��ĵ�ַ
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;                             //��� EV8
    I2C_GenerateSTART(I2Cx, ENABLE);  //�����ź�
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ;  //��� EV5
    I2C_Send7bitAddress(I2Cx, slave_addr,
                        I2C_Direction_Receiver);  //��������ַ����������Ϊ��
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;  //���EV6
    while (NumByteToRead) {
        if (NumByteToRead == 1) {  //ֻʣ�����һ������ʱ���� if ���
            I2C_AcknowledgeConfig(I2Cx, DISABLE);  //�����һ������ʱ�ر�Ӧ��λ
            I2C_GenerateSTOP(I2Cx, ENABLE);  //���һ������ʱʹ��ֹͣλ
        }
        if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {  //��ȡ����
            *pBuffer = I2C_ReceiveData(I2Cx);  //���ÿ⺯��������ȡ���� pBuffer
            pBuffer++;                         //ָ����λ
            NumByteToRead--;                   //�ֽ����� 1
        }
    }
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
}

