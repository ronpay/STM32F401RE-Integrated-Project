#include "i2c.h"

/********************************* Defines ************************************/

#define WAIT_FOR_FLAG(flag, value, timeout, errorcode)                        \
    I2CTimeout = timeout;                                                     \
    while (I2C_GetFlagStatus(SENSORS_I2C, flag) != value) {                   \
        if ((I2CTimeout--) == 0) return I2Cx_TIMEOUT_UserCallback(errorcode); \
    }

#define CLEAR_ADDR_BIT                               \
    I2C_ReadRegister(SENSORS_I2C, I2C_Register_SR1); \
    I2C_ReadRegister(SENSORS_I2C, I2C_Register_SR2);

/********************************* Globals ************************************/

/********************************* Prototypes *********************************/
static unsigned long ST_Sensors_I2C_WriteRegister(
    unsigned char Address, unsigned char RegisterAddr,
    unsigned short RegisterLen, const unsigned char *RegisterValue);
static unsigned long ST_Sensors_I2C_ReadRegister(unsigned char Address,
                                                 unsigned char RegisterAddr,
                                                 unsigned short RegisterLen,
                                                 unsigned char *RegisterValue);
static unsigned long ST_Sensors_I2C_WriteNoRegister(unsigned char Address,
                                                    unsigned char RegisterAddr);
static unsigned long ST_Sensors_I2C_ReadNoRegister(
    unsigned char Address, unsigned short RegisterLen,
    unsigned char *RegisterValue);

/*******************************  Function ************************************/

void I2C3_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    /*I2C2����ʱ��ʹ�� */
    OLED_I2C_CLK_INIT(OLED_I2C_CLK, ENABLE);

    /*I2C2����GPIOʱ��ʹ�� */
    RCC_AHB1PeriphClockCmd(OLED_I2C_SCL_GPIO_CLK | OLED_I2C_SDA_GPIO_CLK,
                           ENABLE);

    /*!< GPIO ���� */
    GPIO_PinAFConfig(OLED_I2C_SCL_GPIO_PORT, OLED_I2C_SCL_SOURCE,
                     OLED_I2C_SCL_AF);
    GPIO_PinAFConfig(OLED_I2C_SDA_GPIO_PORT, OLED_I2C_SDA_SOURCE,
                     OLED_I2C_SDA_AF);

    /*!< ����OLED_I2C����: SCL */
    GPIO_InitStructure.GPIO_Pin = OLED_I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  // I2C���뿪©���
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(OLED_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    /*!<����OLED_I2C����: SDA */
    GPIO_InitStructure.GPIO_Pin = OLED_I2C_SDA_PIN;
    GPIO_Init(OLED_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

    /*!< GPIO ���� */
    GPIO_SetBits(OLED_I2C_SDA_GPIO_PORT, OLED_I2C_SCL_PIN | OLED_I2C_SDA_PIN);

    /* I2C ���� */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle =
        I2C_DutyCycle_2; /* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
    I2C_InitStructure.I2C_OwnAddress1 = OLED_ADDRESS;  //������I2C��ַ
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress =
        I2C_AcknowledgedAddress_7bit;             /* I2C��Ѱַģʽ */
    I2C_InitStructure.I2C_ClockSpeed = I2C_Speed; /* ͨ������ */

    I2C_Init(OLED_I2C, &I2C_InitStructure); /* I2C2 ��ʼ�� */
    I2C_Cmd(OLED_I2C, ENABLE);              /* ʹ�� I2C2 */

    GPIO_PinAFConfig(OLED_I2C_SCL_GPIO_PORT, OLED_I2C_SCL_SOURCE,
                     OLED_I2C_SCL_AF);
    GPIO_PinAFConfig(OLED_I2C_SDA_GPIO_PORT, OLED_I2C_SDA_SOURCE,
                     OLED_I2C_SDA_AF);
}

/**
 * @brief  ��ʼ��I2C���ߣ�ʹ��I2Cǰ��Ҫ����
 * @param  ��
 * @retval ��
 */
void I2C1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure1;

    /* Enable I2Cx clock */
    RCC_APB1PeriphClockCmd(SENSORS_I2C_RCC_CLK, ENABLE);

    /* Enable I2C GPIO clock */
    RCC_AHB1PeriphClockCmd(SENSORS_I2C_SCL_GPIO_CLK | SENSORS_I2C_SDA_GPIO_CLK,
                           ENABLE);

    /* Configure I2Cx pin: SCL ----------------------------------------*/
    GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    /* Connect pins to Periph */
    GPIO_PinAFConfig(SENSORS_I2C_SCL_GPIO_PORT, SENSORS_I2C_SCL_GPIO_PINSOURCE,
                     SENSORS_I2C_AF);
    GPIO_Init(SENSORS_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    /* Configure I2Cx pin: SDA ----------------------------------------*/
    GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SDA_GPIO_PIN;

    /* Connect pins to Periph */
    GPIO_PinAFConfig(SENSORS_I2C_SDA_GPIO_PORT, SENSORS_I2C_SDA_GPIO_PINSOURCE,
                     SENSORS_I2C_AF);
    GPIO_Init(SENSORS_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

    I2C_DeInit(SENSORS_I2C);
    I2C_InitStructure1.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure1.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure1.I2C_OwnAddress1 = I2C_OWN_ADDRESS;
    I2C_InitStructure1.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure1.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure1.I2C_ClockSpeed = I2C_SPEED;

    /* Enable the I2C peripheral */
    I2C_Cmd(SENSORS_I2C, ENABLE);

    /* Initialize the I2C peripheral */
    I2C_Init(SENSORS_I2C, &I2C_InitStructure1);

    return;
}

/**
 * @brief  ��ʱ�ص����������I2C��־��ʱ���ñ���������ʼ��I2C���¼�⡣
 * @param  ������룬ÿ�������ж����ı�ţ����ڲ��Ҵ���
 * @retval Ĭ�Ϸ���1����ʾI2C������
 */
static uint32_t I2Cx_TIMEOUT_UserCallback(char value) {
    /* The following code allows I2C error recovery and return to normal
       communication if the error source doesn�t still exist (ie. hardware
       issue..) */
    I2C_InitTypeDef I2C_InitStructure;

    I2C_GenerateSTOP(SENSORS_I2C, ENABLE);
    I2C_SoftwareResetCmd(SENSORS_I2C, ENABLE);
    I2C_SoftwareResetCmd(SENSORS_I2C, DISABLE);

    I2C_DeInit(SENSORS_I2C);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDRESS;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

    /* Enable the I2C peripheral */
    I2C_Cmd(SENSORS_I2C, ENABLE);

    /* Initialize the I2C peripheral */
    I2C_Init(SENSORS_I2C, &I2C_InitStructure);

    I2C_ERROR("I2C callback error code = %d", value);

    return 1;
}

/**
 * @brief  д�Ĵ���(��γ���)�������ṩ���ϲ�Ľӿ�
 * @param  slave_addr: �ӻ���ַ
 * @param 	reg_addr:�Ĵ�����ַ
 * @param len��д��ĳ���
 *	@param data_ptr:ָ��Ҫд�������
 * @retval ����Ϊ0��������Ϊ��0
 */
int Sensors_I2C_WriteRegister(unsigned char slave_addr, unsigned char reg_addr,
                              unsigned short len,
                              const unsigned char *data_ptr) {
    char retries = 0;
    int ret = 0;
    unsigned short retry_in_mlsec = Get_I2C_Retry();

tryWriteAgain:
    ret = 0;
    ret = ST_Sensors_I2C_WriteRegister(slave_addr, reg_addr, len, data_ptr);

    if (ret && retry_in_mlsec) {
        if (retries++ > 4) return ret;
        Delay(retry_in_mlsec);
        goto tryWriteAgain;
    }
    return ret;
}

/**
 * @brief  ���Ĵ���(��γ���)�������ṩ���ϲ�Ľӿ�
 * @param  slave_addr: �ӻ���ַ
 * @param 	reg_addr:�Ĵ�����ַ
 * @param len��Ҫ��ȡ�ĳ���
 *	@param data_ptr:ָ��Ҫ�洢���ݵ�ָ��
 * @retval ����Ϊ0��������Ϊ��0
 */
int Sensors_I2C_ReadRegister(unsigned char slave_addr, unsigned char reg_addr,
                             unsigned short len, unsigned char *data_ptr) {
    char retries = 0;
    int ret = 0;
    unsigned short retry_in_mlsec = Get_I2C_Retry();

tryReadAgain:
    ret = 0;
    ret = ST_Sensors_I2C_ReadRegister(slave_addr, reg_addr, len, data_ptr);

    if (ret && retry_in_mlsec) {
        if (retries++ > 4) return ret;

        Delay(retry_in_mlsec);
        goto tryReadAgain;
    }
    return ret;
}

/**
 * @brief  д�Ĵ���(���γ���)�����ǵײ�I2C�ӿ�
 * @param  slave_addr: �ӻ���ַ
 * @param 	reg_addr:�Ĵ�����ַ
 * @param len��д��ĳ���
 *	@param data_ptr:ָ��Ҫд�������
 * @retval ����Ϊ0��������Ϊ��0
 */
static unsigned long ST_Sensors_I2C_WriteRegister(
    unsigned char Address, unsigned char RegisterAddr,
    unsigned short RegisterLen, const unsigned char *RegisterValue) {
    uint32_t result = 0;
    uint32_t i = 0;  // i = RegisterLen;
    __IO uint32_t I2CTimeout = I2Cx_LONG_TIMEOUT;

    //  RegisterValue = RegisterValue + (RegisterLen - 1);

    /* Wait for the busy flag to be cleared */
    WAIT_FOR_FLAG(I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 1);

    /* Start the config sequence */
    I2C_GenerateSTART(SENSORS_I2C, ENABLE);

    /* Wait for the start bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 2);

    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(SENSORS_I2C, (Address << 1), I2C_Direction_Transmitter);

    /* Wait for address bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 3);

    /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
    CLEAR_ADDR_BIT

    /* Wait for address bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 4);

    /* Transmit the first address for write operation */
    I2C_SendData(SENSORS_I2C, RegisterAddr);

    for (i = 0; i < (RegisterLen); i++) {
        /* Wait for address bit to be set */
        WAIT_FOR_FLAG(I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 5);

        /* Prepare the register value to be sent */
        I2C_SendData(SENSORS_I2C, RegisterValue[i]);
    }

    /* Wait for address bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 6);

    /* End the configuration sequence */
    I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

    /* Return the verifying value: 0 (Passed) or 1 (Failed) */
    return result;
}

/**
 * @brief  ���Ĵ���(���γ���)�����ǵײ�I2C�ӿ�
 * @param  slave_addr: �ӻ���ַ
 * @param 	reg_addr:�Ĵ�����ַ
 * @param len��Ҫ��ȡ�ĳ���
 *	@param data_ptr:ָ��Ҫ�洢���ݵ�ָ��
 * @retval ����Ϊ0��������Ϊ��0
 */
static unsigned long ST_Sensors_I2C_ReadRegister(unsigned char Address,
                                                 unsigned char RegisterAddr,
                                                 unsigned short RegisterLen,
                                                 unsigned char *RegisterValue) {
    uint32_t i = 0, result = 0;
    __IO uint32_t I2CTimeout = I2Cx_LONG_TIMEOUT;

    /* Wait for the busy flag to be cleared */
    WAIT_FOR_FLAG(I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 7);

    /* Start the config sequence */
    I2C_GenerateSTART(SENSORS_I2C, ENABLE);

    /* Wait for the start bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 8);

    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(SENSORS_I2C, (Address << 1), I2C_Direction_Transmitter);

    /* Wait for the start bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 9);

    /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
    CLEAR_ADDR_BIT;

    /* Wait for address bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 10);

    /* Transmit the register address to be read */
    I2C_SendData(SENSORS_I2C, RegisterAddr);

    /* Wait for address bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 11);

    /*!< Send START condition a second time */
    I2C_GenerateSTART(SENSORS_I2C, ENABLE);

    /* Wait for the start bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 12);

    /*!< Send address for read */
    I2C_Send7bitAddress(SENSORS_I2C, (Address << 1), I2C_Direction_Receiver);

    /* Wait for the start bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 13);

    if (RegisterLen == 1) {
        /*!< Disable Acknowledgment */
        I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);

        /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
        CLEAR_ADDR_BIT;

        /*!< Send STOP Condition */
        I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

        /* Wait for the RXNE bit to be set */
        WAIT_FOR_FLAG(I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 14);

        RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
    } else if (RegisterLen == 2) {
        /*!< Disable Acknowledgment */
        I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);

        /* Set POS bit */
        SENSORS_I2C->CR1 |= I2C_CR1_POS;

        /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
        CLEAR_ADDR_BIT;

        /* Wait for the buffer full bit to be set */
        WAIT_FOR_FLAG(I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 15);

        /*!< Send STOP Condition */
        I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

        /* Read 2 bytes */
        RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
        RegisterValue[1] = I2C_ReceiveData(SENSORS_I2C);
    } else if (RegisterLen == 3) {
        CLEAR_ADDR_BIT;

        /* Wait for the buffer full bit to be set */
        WAIT_FOR_FLAG(I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 16);
        /*!< Disable Acknowledgment */
        I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
        /* Read 1 bytes */
        RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
        /*!< Send STOP Condition */
        I2C_GenerateSTOP(SENSORS_I2C, ENABLE);
        /* Read 1 bytes */
        RegisterValue[1] = I2C_ReceiveData(SENSORS_I2C);
        /* Wait for the buffer full bit to be set */
        WAIT_FOR_FLAG(I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 17);
        /* Read 1 bytes */
        RegisterValue[2] = I2C_ReceiveData(SENSORS_I2C);
    } else /* more than 2 bytes */
    {
        /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
        CLEAR_ADDR_BIT;

        for (i = 0; i < (RegisterLen); i++) {
            if (i == (RegisterLen - 3)) {
                /* Wait for the buffer full bit to be set */
                WAIT_FOR_FLAG(I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 16);

                /*!< Disable Acknowledgment */
                I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);

                /* Read 1 bytes */
                RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);

                /*!< Send STOP Condition */
                I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

                /* Read 1 bytes */
                RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);

                /* Wait for the buffer full bit to be set */
                WAIT_FOR_FLAG(I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 17);

                /* Read 1 bytes */
                RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);
                goto endReadLoop;
            }

            /* Wait for the RXNE bit to be set */
            WAIT_FOR_FLAG(I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 18);
            RegisterValue[i] = I2C_ReceiveData(SENSORS_I2C);
        }
    }

endReadLoop:
    /* Clear BTF flag */
    I2C_ClearFlag(SENSORS_I2C, I2C_FLAG_BTF);
    /* Wait for the busy flag to be cleared */
    WAIT_FOR_FLAG(I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 19);
    /*!< Re-Enable Acknowledgment to be ready for another reception */
    I2C_AcknowledgeConfig(SENSORS_I2C, ENABLE);
    // Disable POS -- TODO
    SENSORS_I2C->CR1 &= ~I2C_CR1_POS;

    /* Return the byte read from sensor */
    return result;
}

/**
 * @brief
 * д����(��γ���)�����ǵײ�I2C�ӿڣ�ר����ֻ��IIC�豸��������ĺ��������������
 * @param  slave_addr: �ӻ���ַ
 * @param 	reg_addr:�Ĵ�����ַ��Ҫд������
 * @retval ����Ϊ0��������Ϊ��0
 */
int Sensors_I2C_WriteNoRegister(unsigned char slave_addr,
                                unsigned char reg_addr) {
    char retries = 0;
    int ret = 0;
    unsigned short retry_in_mlsec = Get_I2C_Retry();

tryWriteAgain:
    ret = 0;
    ret = ST_Sensors_I2C_WriteNoRegister(slave_addr, reg_addr);

    if (ret && retry_in_mlsec) {
        if (retries++ > 4) return ret;

        Delay(retry_in_mlsec);
        goto tryWriteAgain;
    }
    return ret;
}

/**
 * @brief
 *������(��γ���)�����ǵײ�I2C�ӿڣ�ר����ֻ��IIC�豸��ȡ����(����Ҫ���ͼĴ�����ַ)
 * @param  slave_addr: �ӻ���ַ
 * @param 	RegisterLen:���ݳ���
 *	@param	RegisterValue��ָ��洢���ݵ�ָ��
 * @retval ����Ϊ0��������Ϊ��0
 */
int Sensors_I2C_ReadNoRegister(unsigned char slave_addr, unsigned short len,
                               unsigned char *data_ptr) {
    char retries = 0;
    int ret = 0;
    unsigned short retry_in_mlsec = Get_I2C_Retry();

tryReadAgain:
    ret = 0;
    ret = ST_Sensors_I2C_ReadNoRegister(slave_addr, len, data_ptr);

    if (ret && retry_in_mlsec) {
        if (retries++ > 4) return ret;

        Delay(retry_in_mlsec);
        goto tryReadAgain;
    }
    return ret;
}

/**
 * @brief
 * д����(���γ���)�����ǵײ�I2C�ӿڣ�ר����ֻ��IIC�豸��������ĺ��������������
 * @param  slave_addr: �ӻ���ַ
 * @param 	reg_addr:�Ĵ�����ַ��Ҫд������
 * @retval ����Ϊ0��������Ϊ��0
 */
static unsigned long ST_Sensors_I2C_WriteNoRegister(
    unsigned char Address, unsigned char RegisterAddr) {
    uint32_t result = 0;
    __IO uint32_t I2CTimeout = I2Cx_LONG_TIMEOUT;

    /* Wait for the busy flag to be cleared */
    WAIT_FOR_FLAG(I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 1);

    /* Start the config sequence */
    I2C_GenerateSTART(SENSORS_I2C, ENABLE);

    /* Wait for the start bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 2);

    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(SENSORS_I2C, (Address << 1), I2C_Direction_Transmitter);

    /* Wait for address bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 3);

    /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
    CLEAR_ADDR_BIT

    /* Wait for address bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 4);

    /* Transmit the first address for write operation */
    I2C_SendData(SENSORS_I2C, RegisterAddr);

    /* Wait for address bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 6);

    /* End the configuration sequence */
    I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

    /* Return the verifying value: 0 (Passed) or 1 (Failed) */
    return result;
}

/**
 * @brief
 *������(���γ���)�����ǵײ�I2C�ӿڣ�ר����ֻ��IIC�豸��ȡ����(����Ҫ���ͼĴ�����ַ)
 * @param  slave_addr: �ӻ���ַ
 * @param 	RegisterLen:���ݳ���
 *	@param	RegisterValue��ָ��洢���ݵ�ָ��
 * @retval ����Ϊ0��������Ϊ��0
 */
static unsigned long ST_Sensors_I2C_ReadNoRegister(
    unsigned char Address, unsigned short RegisterLen,
    unsigned char *RegisterValue) {
    uint32_t i = 0, result = 0;
    __IO uint32_t I2CTimeout = I2Cx_LONG_TIMEOUT;

    /* Wait for the busy flag to be cleared */
    WAIT_FOR_FLAG(I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 7);

    /*!< Send START condition a second time */
    I2C_GenerateSTART(SENSORS_I2C, ENABLE);

    /* Wait for the start bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 12);

    /*!< Send address for read */
    I2C_Send7bitAddress(SENSORS_I2C, (Address << 1), I2C_Direction_Receiver);

    /* Wait for the start bit to be set */
    WAIT_FOR_FLAG(I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 13);

    if (RegisterLen == 1) {
        /*!< Disable Acknowledgment */
        I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);

        /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
        CLEAR_ADDR_BIT;

        /*!< Send STOP Condition */
        I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

        /* Wait for the RXNE bit to be set */
        WAIT_FOR_FLAG(I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 14);

        RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
    } else if (RegisterLen == 2) {
        /*!< Disable Acknowledgment */
        I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);

        /* Set POS bit */
        SENSORS_I2C->CR1 |= I2C_CR1_POS;

        /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
        CLEAR_ADDR_BIT;

        /* Wait for the buffer full bit to be set */
        WAIT_FOR_FLAG(I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 15);

        /*!< Send STOP Condition */
        I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

        /* Read 2 bytes */
        RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
        RegisterValue[1] = I2C_ReceiveData(SENSORS_I2C);
    } else if (RegisterLen == 3) {
        CLEAR_ADDR_BIT;

        /* Wait for the buffer full bit to be set */
        WAIT_FOR_FLAG(I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 16);
        /*!< Disable Acknowledgment */
        I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
        /* Read 1 bytes */
        RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
        /*!< Send STOP Condition */
        I2C_GenerateSTOP(SENSORS_I2C, ENABLE);
        /* Read 1 bytes */
        RegisterValue[1] = I2C_ReceiveData(SENSORS_I2C);
        /* Wait for the buffer full bit to be set */
        WAIT_FOR_FLAG(I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 17);
        /* Read 1 bytes */
        RegisterValue[2] = I2C_ReceiveData(SENSORS_I2C);
    } else /* more than 2 bytes */
    {
        /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
        CLEAR_ADDR_BIT;

        for (i = 0; i < (RegisterLen); i++) {
            if (i == (RegisterLen - 3)) {
                /* Wait for the buffer full bit to be set */
                WAIT_FOR_FLAG(I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 16);

                /*!< Disable Acknowledgment */
                I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);

                /* Read 1 bytes */
                RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);

                /*!< Send STOP Condition */
                I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

                /* Read 1 bytes */
                RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);

                /* Wait for the buffer full bit to be set */
                WAIT_FOR_FLAG(I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 17);

                /* Read 1 bytes */
                RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);
                goto endReadLoop;
            }

            /* Wait for the RXNE bit to be set */
            WAIT_FOR_FLAG(I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 18);
            RegisterValue[i] = I2C_ReceiveData(SENSORS_I2C);
        }
    }

endReadLoop:
    /* Clear BTF flag */
    I2C_ClearFlag(SENSORS_I2C, I2C_FLAG_BTF);
    /* Wait for the busy flag to be cleared */
    WAIT_FOR_FLAG(I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 19);
    /*!< Re-Enable Acknowledgment to be ready for another reception */
    I2C_AcknowledgeConfig(SENSORS_I2C, ENABLE);
    // Disable POS -- TODO
    SENSORS_I2C->CR1 &= ~I2C_CR1_POS;

    /* Return the byte read from sensor */
    return result;
}

static unsigned short RETRY_IN_MLSEC = 55;

/**
 * @brief  �������Եȴ�������
 * @param  ��
 * @retval ��
 */
void Set_I2C_Retry(unsigned short ml_sec) { RETRY_IN_MLSEC = ml_sec; }

/**
 * @brief  ��ȡ���Եȴ�������
 * @param  ��
 * @retval ��
 */
unsigned short Get_I2C_Retry(void) { return RETRY_IN_MLSEC; }

//ΪHMC5883L�����õģ�
void I2C_SAND_BYTE(u8 SlaveAddr, u8 writeAddr,
                   u8 pBuffer) {  // I2C����һ���ֽڣ��ӵ�ַ���ڲ���ַ�����ݣ�
    I2C_GenerateSTART(I2C1, ENABLE);  //���Ϳ�ʼ�ź�
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;  //�ȴ����
    I2C_Send7bitAddress(
        I2C1, SlaveAddr,
        I2C_Direction_Transmitter);  //���ʹ�������ַ��״̬��д�룩
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;                           //�ȴ����
    I2C_SendData(I2C1, writeAddr);  //���ʹ������ڲ��Ĵ�����ַ
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;                         //�ȴ����
    I2C_SendData(I2C1, pBuffer);  //����Ҫд�������
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;                            //�ȴ����
    I2C_GenerateSTOP(I2C1, ENABLE);  //���ͽ����ź�
}

void I2C_READ_BUFFER(
    u8 SlaveAddr, u8 readAddr, u8 *pBuffer,
    u16 NumByteToRead) {  // I2C��ȡ���ݴ���������ַ���Ĵ������ڲ���ַ��������
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
        ;
    I2C_GenerateSTART(I2C1, ENABLE);  //�����ź�
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;  //��� EV5
    I2C_Send7bitAddress(I2C1, SlaveAddr,
                        I2C_Direction_Transmitter);  //д��������ַ
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;  //��� EV6
    I2C_Cmd(I2C1, ENABLE);
    I2C_SendData(I2C1, readAddr);  //���Ͷ��ĵ�ַ
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;                             //��� EV8
    I2C_GenerateSTART(I2C1, ENABLE);  //�����ź�
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;  //��� EV5
    I2C_Send7bitAddress(I2C1, SlaveAddr,
                        I2C_Direction_Receiver);  //��������ַ����������Ϊ��
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;  //���EV6
    while (NumByteToRead) {
        if (NumByteToRead == 1) {  //ֻʣ�����һ������ʱ���� if ���
            I2C_AcknowledgeConfig(I2C1, DISABLE);  //�����һ������ʱ�ر�Ӧ��λ
            I2C_GenerateSTOP(I2C1, ENABLE);  //���һ������ʱʹ��ֹͣλ
        }
        if (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {  //��ȡ����
            *pBuffer = I2C_ReceiveData(I2C1);  //���ÿ⺯��������ȡ���� pBuffer
            pBuffer++;                         //ָ����λ
            NumByteToRead--;                   //�ֽ����� 1
        }
    }
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}
