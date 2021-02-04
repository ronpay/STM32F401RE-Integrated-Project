#include "newi2c.h"

void I2C_WriteByte(I2C_TypeDef * I2Cx,uint8_t slave_addr,uint8_t reg_addr, uint8_t data) {
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        ;

    I2C_GenerateSTART(I2Cx, ENABLE);  //开启I2Cx
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ; /*EV5,主模式*/

    I2C_Send7bitAddress(I2Cx, slave_addr,
                        I2C_Direction_Transmitter);  //器件地址
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    I2C_SendData(I2Cx, reg_addr);  //寄存器地址
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_SendData(I2Cx, data);  //发送数据
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_GenerateSTOP(I2Cx, ENABLE);  //关闭I2Cx总线
}

void I2C_ReadData(I2C_TypeDef * I2Cx,
    uint8_t slave_addr, uint8_t reg_addr, uint8_t *pBuffer,
    uint16_t NumByteToRead) {  // I2C读取数据串（器件地址，寄存器，内部地址，数量）
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        ;
    I2C_GenerateSTART(I2Cx, ENABLE);  //开启信号
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ;  //清除 EV5
    I2C_Send7bitAddress(I2Cx, slave_addr,
                        I2C_Direction_Transmitter);  //写入器件地址
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;  //清除 EV6
    I2C_Cmd(I2Cx, ENABLE);
    I2C_SendData(I2Cx, reg_addr);  //发送读的地址
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;                             //清除 EV8
    I2C_GenerateSTART(I2Cx, ENABLE);  //开启信号
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ;  //清除 EV5
    I2C_Send7bitAddress(I2Cx, slave_addr,
                        I2C_Direction_Receiver);  //将器件地址传出，主机为读
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;  //清除EV6
    while (NumByteToRead) {
        if (NumByteToRead == 1) {  //只剩下最后一个数据时进入 if 语句
            I2C_AcknowledgeConfig(I2Cx, DISABLE);  //最后有一个数据时关闭应答位
            I2C_GenerateSTOP(I2Cx, ENABLE);  //最后一个数据时使能停止位
        }
        if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {  //读取数据
            *pBuffer = I2C_ReceiveData(I2Cx);  //调用库函数将数据取出到 pBuffer
            pBuffer++;                         //指针移位
            NumByteToRead--;                   //字节数减 1
        }
    }
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
}

