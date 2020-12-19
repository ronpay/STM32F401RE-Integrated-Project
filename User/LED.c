#include "LED.h"

#include "stm32f4xx.h"
#include "sysTick.h"
void LED_Config(void) {
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����LED��ص�GPIO����ʱ��*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

    /*��������ģʽΪ���ģʽ*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    /*������������Ϊ2MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    while (1) {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        Delay_ms(32);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        Delay_ms(32);
    }
}
