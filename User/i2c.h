#ifndef __I2C_H__
#define __I2C_H__

#include <stdio.h>

#include "stm32f4xx.h"
#include "sysTick.h"
// OLED
#define OLED_ADDRESS 0x78  //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78

/* STM32 I2C ����ģʽ */
#define I2C_Speed 400000

/*I2C�ӿ�*/
#define OLED_I2C I2C3
#define OLED_I2C_CLK RCC_APB1Periph_I2C3
#define OLED_I2C_CLK_INIT RCC_APB1PeriphClockCmd

#define OLED_I2C_SCL_PIN GPIO_Pin_8
#define OLED_I2C_SCL_GPIO_PORT GPIOA
#define OLED_I2C_SCL_GPIO_CLK RCC_AHB1Periph_GPIOA
#define OLED_I2C_SCL_SOURCE GPIO_PinSource8
#define OLED_I2C_SCL_AF GPIO_AF_I2C3

#define OLED_I2C_SDA_PIN GPIO_Pin_9
#define OLED_I2C_SDA_GPIO_PORT GPIOC
#define OLED_I2C_SDA_GPIO_CLK RCC_AHB1Periph_GPIOC
#define OLED_I2C_SDA_SOURCE GPIO_PinSource9
#define OLED_I2C_SDA_AF GPIO_AF_I2C3

// GY-86
#define I2C_SPEED 400000
#define I2C_OWN_ADDRESS 0xC0

//���뼶��ʱ(��Ҫ��ʱ��֧��)��������дDelay��
#define Delay Delay_ms

#define I2Cx_FLAG_TIMEOUT ((uint32_t)900)                        // 0x1100
#define I2Cx_LONG_TIMEOUT ((uint32_t)(300 * I2Cx_FLAG_TIMEOUT))  // was300

/*���Ŷ���*/

#define SENSORS_I2C_SCL_GPIO_PORT GPIOB
#define SENSORS_I2C_SCL_GPIO_CLK RCC_AHB1Periph_GPIOB
#define SENSORS_I2C_SCL_GPIO_PIN GPIO_Pin_8
#define SENSORS_I2C_SCL_GPIO_PINSOURCE GPIO_PinSource8

#define SENSORS_I2C_SDA_GPIO_PORT GPIOB
#define SENSORS_I2C_SDA_GPIO_CLK RCC_AHB1Periph_GPIOB
#define SENSORS_I2C_SDA_GPIO_PIN GPIO_Pin_9
#define SENSORS_I2C_SDA_GPIO_PINSOURCE GPIO_PinSource9

#define SENSORS_I2C_AF GPIO_AF_I2C1

#define SENSORS_I2C I2C1
#define SENSORS_I2C_RCC_CLK RCC_APB1Periph_I2C1

/*��Ϣ���*/
#define I2C_DEBUG_ON 1
#define I2C_DEBUG_FUNC_ON 0

#define I2C_INFO(fmt, arg...) printf("<<-I2C-INFO->> " fmt "\n", ##arg)
#define I2C_ERROR(fmt, arg...) printf("<<-I2C-ERROR->> " fmt "\n", ##arg)
#define I2C_DEBUG(fmt, arg...)                                        \
    do {                                                              \
        if (I2C_DEBUG_ON)                                             \
            printf("<<-I2C-DEBUG->> [%d]" fmt "\n", __LINE__, ##arg); \
    } while (0)

#define I2C_DEBUG_FUNC()                                                    \
    do {                                                                    \
        if (I2C_DEBUG_FUNC_ON)                                              \
            printf("<<-I2C-FUNC->> Func:%s@Line:%d\n", __func__, __LINE__); \
    } while (0)

/*��������*/
void I2C3_Configuration(void);
#define I2C_Config() I2C1_Init();

void I2C1_Init(void);
void Set_I2C_Retry(unsigned short ml_sec);
unsigned short Get_I2C_Retry(void);

int Sensors_I2C_ReadRegister(unsigned char slave_addr, unsigned char reg_addr,
                             unsigned short len, unsigned char *data_ptr);
int Sensors_I2C_WriteRegister(unsigned char slave_addr, unsigned char reg_addr,
                              unsigned short len,
                              const unsigned char *data_ptr);

int Sensors_I2C_WriteNoRegister(unsigned char slave_addr,
                                unsigned char reg_addr);
int Sensors_I2C_ReadNoRegister(unsigned char slave_addr, unsigned short len,
                               unsigned char *data_ptr);

//ΪHMC5883L�����õ�
void I2C_SAND_BYTE(u8 SlaveAddr, u8 writeAddr, u8 pBuffer);
void I2C_READ_BUFFER(u8 SlaveAddr, u8 readAddr, u8 *pBuffer, u16 NumByteToRead);

#endif  // __I2C_H__
