/**
  ******************************************************************************
  * @file    OLED_I2C.c
  * @brief   128*64�����OLED��ʾ�������ļ�����������SD1306����IICͨ�ŷ�ʽ��ʾ��
  ******************************************************************************
        * Function List:
        *	1. void I2C_Configuration(void)-- ����CPU��Ӳ��I2C
        * 2. void I2C_WriteByte(uint8_t addr,uint8_t data) --
  ��Ĵ�����ַдһ��byte������
        * 3. void WriteCmd(unsigned char I2C_Command) -- д����
        * 4. void WriteDat(unsigned char I2C_Data) -- д����
        * 5. void OLED_Init(void) -- OLED����ʼ��
        * 6. void OLED_Set Pos(unsigned char x, unsigned char y) --
  ������ʼ������
        * 7. void OLED_Fill(unsigned char fill_Data) -- ȫ�����
        * 8. void OLED_CLS(void) -- ����
        * 9. void OLED_ON(void) -- ����
        * 10. void OLED_OFF(void) -- ˯��
        * 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char
  ch[], unsigned char TextSize) -- ��ʾ�ַ���(�����С��6*8��8*16����)
        * 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char
  N) -- ��ʾ����(������Ҫ��ȡģ��Ȼ��ŵ�codetab.h��)
        * 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char
  x1,unsigned char y1,unsigned char BMP[]) -- BMPͼƬ
        *
  *
  ******************************************************************************
  */

#include "oled.h"
/**
 * @brief  I2C3 I/O����
 * @param  ��
 * @retval ��
 */

void OLED_Config(void) { I2C3_Configuration(); }

/**
 * @brief  I2C_WriteByte����OLED�Ĵ�����ַдһ��byte������
 * @param  addr���Ĵ�����ַ
 *					data��Ҫд�������
 * @retval ��
 */
void I2C_WriteByte(uint8_t addr, uint8_t data) {
    while (I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY))
        ;

    I2C_GenerateSTART(I2C3, ENABLE);  //����I2C3
    while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT))
        ; /*EV5,��ģʽ*/

    I2C_Send7bitAddress(I2C3, OLED_ADDRESS,
                        I2C_Direction_Transmitter);  //������ַ -- Ĭ��0x78
    while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    I2C_SendData(I2C3, addr);  //�Ĵ�����ַ
    while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_SendData(I2C3, data);  //��������
    while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_GenerateSTOP(I2C3, ENABLE);  //�ر�I2C3����
}

/**
 * @brief  WriteCmd����OLEDд������
 * @param  I2C_Command���������
 * @retval ��
 */
void WriteCmd(unsigned char I2C_Command)  //д����
{
    I2C_WriteByte(0x00, I2C_Command);
}

/**
 * @brief  WriteDat����OLEDд������
 * @param  I2C_Data������
 * @retval ��
 */
void WriteDat(unsigned char I2C_Data)  //д����
{
    I2C_WriteByte(0x40, I2C_Data);
}

/**
 * @brief  OLED_Init����ʼ��OLED
 * @param  ��
 * @retval ��
 */
void OLED_Init(void) {
    Delay_ms(1000);  // 1s,�������ʱ����Ҫ,�ϵ����ʱ��û�д�����������

    WriteCmd(0xAE);  // display off
    WriteCmd(0x20);  // Set Memory Addressing Mode
    WriteCmd(0x10);  // 00,Horizontal Addressing Mode;01,Vertical Addressing
                     // Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0);  // Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8);  // Set COM Output Scan Direction
    WriteCmd(0x00);  //---set low column address
    WriteCmd(0x10);  //---set high column address
    WriteCmd(0x40);  //--set start line address
    WriteCmd(0x81);  //--set contrast control register
    WriteCmd(0xff);  //���ȵ��� 0x00~0xff
    WriteCmd(0xa1);  //--set segment re-map 0 to 127
    WriteCmd(0xa6);  //--set normal display
    WriteCmd(0xa8);  //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F);  //
    WriteCmd(0xa4);  // 0xa4,Output follows RAM content;0xa5,Output ignores RAM
                     // content
    WriteCmd(0xd3);  //-set display offset
    WriteCmd(0x00);  //-not offset
    WriteCmd(0xd5);  //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0);  //--set divide ratio
    WriteCmd(0xd9);  //--set pre-charge period
    WriteCmd(0x22);  //
    WriteCmd(0xda);  //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb);  //--set vcomh
    WriteCmd(0x20);  // 0x20,0.77xVcc
    WriteCmd(0x8d);  //--set DC-DC enable
    WriteCmd(0x14);  //
    WriteCmd(0xaf);  //--turn on oled panel
}

/**
 * @brief  OLED_SetPos�����ù��
 * @param  x,���xλ��
 *					y�����yλ��
 * @retval ��
 */
void OLED_SetPos(unsigned char x, unsigned char y)  //������ʼ������
{
    WriteCmd(0xb0 + y);
    WriteCmd(((x & 0xf0) >> 4) | 0x10);
    WriteCmd((x & 0x0f) | 0x01);
}

/**
 * @brief  OLED_Fill�����������Ļ
 * @param  fill_Data:Ҫ��������
 * @retval ��
 */
void OLED_Fill(unsigned char fill_Data)  //ȫ�����
{
    unsigned char m, n;
    for (m = 0; m < 8; m++) {
        WriteCmd(0xb0 + m);  // page0-page1
        WriteCmd(0x00);      // low column start address
        WriteCmd(0x10);      // high column start address
        for (n = 0; n < 128; n++) {
            WriteDat(fill_Data);
        }
    }
}

/**
 * @brief  OLED_CLS������
 * @param  ��
 * @retval ��
 */
void OLED_CLS(void)  //����
{
    OLED_Fill(0x00);
}

/**
 * @brief  OLED_ON����OLED�������л���
 * @param  ��
 * @retval ��
 */
void OLED_ON(void) {
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X14);  //������ɱ�
    WriteCmd(0XAF);  // OLED����
}

/**
 * @brief  OLED_OFF����OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
 * @param  ��
 * @retval ��
 */
void OLED_OFF(void) {
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X10);  //�رյ�ɱ�
    WriteCmd(0XAE);  // OLED����
}

/**
 * @brief  OLED_ShowStr����ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
 * @param  x,y : ��ʼ������(x:0~127, y:0~7);
 *					ch[] :- Ҫ��ʾ���ַ���;
 *					TextSize : �ַ���С(1:6*8 ; 2:8*16)
 * @retval ��
 */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[],
                  unsigned char TextSize) {
    unsigned char c = 0, i = 0, j = 0;
    switch (TextSize) {
        case 1: {
            while (ch[j] != '\0') {
                c = ch[j] - 32;
                if (x > 126) {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x, y);
                for (i = 0; i < 6; i++) WriteDat(F6x8[c][i]);
                x += 6;
                j++;
            }
        } break;
        case 2: {
            while (ch[j] != '\0') {
                c = ch[j] - 32;
                if (x > 120) {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x, y);
                for (i = 0; i < 8; i++) WriteDat(F8X16[c * 16 + i]);
                OLED_SetPos(x, y + 1);
                for (i = 0; i < 8; i++) WriteDat(F8X16[c * 16 + i + 8]);
                x += 8;
                j++;
            }
        } break;
    }
}

/**
 * @brief  OLED_ShowCN����ʾcodetab.h�еĺ���,16*16����
 * @param  x,y: ��ʼ������(x:0~127, y:0~7);
 *					N:������codetab.h�е�����
 * @retval ��
 */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) {
    unsigned char wm = 0;
    unsigned int adder = 32 * N;
    OLED_SetPos(x, y);
    for (wm = 0; wm < 16; wm++) {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x, y + 1);
    for (wm = 0; wm < 16; wm++) {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
}

/**
 * @brief  OLED_DrawBMP����ʾBMPλͼ
 * @param  x0,y0 :��ʼ������(x0:0~127, y0:0~7);
 *					x1,y1 :
 *���Խ���(������)������(x1:1~128,y1:1~8)
 * @retval ��
 */
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                  unsigned char y1, unsigned char BMP[]) {
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++) {
        OLED_SetPos(x0, y);
        for (x = x0; x < x1; x++) {
            WriteDat(BMP[j++]);
        }
    }
}

//��ʾ2������
// x,y :�������
// len :���ֵ�λ��
// size:�����С
// mode:ģʽ	0,���ģʽ;1,����ģʽ
// num:��ֵ(0~4294967295);
unsigned int oled_pow(uint8_t m, uint8_t n) {
    unsigned int result = 1;
    while (n--) result *= m;
    return result;
}
void OLED_Set_Pos(uint8_t x, uint8_t y) {
    WriteCmd(0xb0 + y);
    WriteCmd(((x & 0xf0) >> 4) | 0x10);
    WriteCmd(x & 0x0f);
}
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size) {
    unsigned char c = 0, i = 0;
    c = chr - ' ';  //�õ�ƫ�ƺ��ֵ
    if (x > 128 - 1) {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16) {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++) WriteDat(F8X16[c * 16 + i]);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++) WriteDat(F8X16[c * 16 + i + 8]);
    } else {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 6; i++) WriteDat(F6x8[c][i]);
    }
}

void OLED_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t len,
                  uint8_t size2) {
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
                continue;
            } else
                enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
    }
}

// OLED������ʾ 3��
void OLED_Show_3num(uint16_t x, uint16_t y, uint16_t z, uint8_t row) {
    x = (int)x, y = (int)y, z = (int)z, row = (int)row;

    OLED_ShowNum(22, row, x, 3, 12);
    OLED_ShowNum(64, row, y, 3, 12);
    OLED_ShowNum(106, row, z, 3, 12);
}
