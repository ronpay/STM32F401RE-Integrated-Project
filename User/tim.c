#include "tim.h"


void TIM2_PWMOUTPUT_Config(void) {
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*������ص�GPIO����ʱ��*/
    RCC_AHB1PeriphClockCmd(GENERAL_OCPWM_GPIO_CLK, ENABLE);
    /* ��ʱ��ͨ�����Ÿ��� */
    GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT, GENERAL_OCPWM_PINSOURCE_1,
                     GENERAL_OCPWM_AF);
    GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT, GENERAL_OCPWM_PINSOURCE_2,
                     GENERAL_OCPWM_AF);
    GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT, GENERAL_OCPWM_PINSOURCE_3,
                     GENERAL_OCPWM_AF);
    GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT, GENERAL_OCPWM_PINSOURCE_4,
                     GENERAL_OCPWM_AF);

    /* ��ʱ��ͨ���������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_PIN_1;
    GPIO_Init(GENERAL_OCPWM_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_PIN_2;
    GPIO_Init(GENERAL_OCPWM_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_PIN_3;
    GPIO_Init(GENERAL_OCPWM_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_PIN_4;
    GPIO_Init(GENERAL_OCPWM_GPIO_PORT, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // ����TIMx_CLK,x[2,3,4,5,12,13,14]
    RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE);

    /* �ۼ� TIM_Period�������һ�����»����ж�*/
    //����ʱ����0������8399����Ϊ8400�Σ�Ϊһ����ʱ����
    // TIM_TimeBaseStructure.TIM_Period = 8400-1;
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;

    // ͨ�ÿ��ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=84MHz
    // �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
    // TIM_TimeBaseStructure.TIM_Prescaler = 840-1;
    // TIM_TimeBaseStructure.TIM_Prescaler = 199;
    TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;
    // ����ʱ�ӷ�Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // ������ʽ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    // ��ʼ����ʱ��TIMx, x[2,3,4,5,12,13,14]
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

    /*PWMģʽ����*/
    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // TIM_OCInitStructure.TIM_Pulse = 2800-1;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity =
        TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

    TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);  //ʹ��ͨ��1
    TIM_OC2Init(GENERAL_TIM, &TIM_OCInitStructure);  //ʹ��ͨ��2
    TIM_OC3Init(GENERAL_TIM, &TIM_OCInitStructure);  //ʹ��ͨ��3
    TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);  //ʹ��ͨ��4

    /*ʹ��ͨ��1234����*/
    TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);

    // ʹ�ܶ�ʱ��
    TIM_Cmd(GENERAL_TIM, ENABLE);
}

void TIM3_Cap_Init(u16 arr, u16 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef TIM3_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //ʱ��TIM3
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��GPIOA
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOA

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�����������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      //����

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  // PA6
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

    //��ʼ����ʱ��TIM3

    TIM_TimeBaseStructure.TIM_Period =
        arr;  //���ü������Զ���װֵ 	ͨ��Ϊ��0xFFFF
    TIM_TimeBaseStructure.TIM_Prescaler = psc;  //Ԥ��Ƶ����psc = 1M = 1us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    ////�ظ���������

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  //������ʼ��

    TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);  //���TIM3�����жϱ�־

    //��ʼ��TIM3�����벶�����
    TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //�����ز���
    TIM3_ICInitStructure.TIM_ICSelection =
        TIM_ICSelection_DirectTI;                           //ӳ�䵽TI1��
    TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;  //����Ƶ��ֱ�Ӳ���
    TIM3_ICInitStructure.TIM_ICFilter =
        0x00;  // IC1F = 000 ���������˲��� ���˲�

    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;  // cc1s = 01  ͨ��ѡ��
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;  // cc1s = 02  ͨ��ѡ��
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;  // cc1s = 03  ͨ��ѡ��
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;  // cc1s = 04  ͨ��ѡ��
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);

    TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_Update,
                 ENABLE);  //��������жϣ�����CC1IE�����ж�
    TIM_ITConfig(TIM3, TIM_IT_CC2 | TIM_IT_Update,
                 ENABLE);  //��������жϣ�����CC1IE�����ж�
    TIM_ITConfig(TIM3, TIM_IT_CC3 | TIM_IT_Update,
                 ENABLE);  //��������жϣ�����CC1IE�����ж�
    TIM_ITConfig(TIM3, TIM_IT_CC4 | TIM_IT_Update,
                 ENABLE);  //��������жϣ�����CC1IE�����ж�
    //�жϷ����ʼ��
    TIM_Cmd(TIM3, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel =
        TIM3_IRQn;  //�����жϣ�TIM3_CC_IRQn;
                    //TIM3�Ĵ����жϣ�TIM3_TRG_COM_TIM31_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
