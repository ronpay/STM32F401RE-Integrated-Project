#include "hm10.h"

void HM10_Config(void) { USART6_Config(); }

void HM10_Test(void) { USARTTest(USART6); }

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f) {
    /* ����һ���ֽ����ݵ����� */
    USART_SendData(USART6, (uint8_t)ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET)
        ;

    return (ch);
}
