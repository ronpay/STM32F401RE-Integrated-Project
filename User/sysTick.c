/**
 ******************************************************************************
 * @file    sysTick.c
 * @version V1.0
 * @date    2015-xx-xx
 * @brief   SysTick ϵͳ�δ�ʱ��1ms�жϺ�����,�ж�ʱ����������ã�
 *          ���õ��� 1us 10us 1ms �жϡ�
 */

#include "sysTick.h"

static __IO u32 TimingDelay;
static __IO uint32_t g_ul_ms_ticks = 0;

/**
 * @brief  ����ϵͳ�δ�ʱ�� SysTick
 * @param  ��
 * @retval ��
 */
void SysTick_Init(void) {
    /* SystemFrequency / 1000    1ms�ж�һ��
     * SystemFrequency / 100000	 10us�ж�һ��
     * SystemFrequency / 1000000 1us�ж�һ��
     */
    if (SysTick_Config(SystemCoreClock / 1000)) {
        /* Capture error */
        while (1)
            ;
    }
}

/**
 * @brief   us��ʱ����,1msΪһ����λ
 * @param
 *		@arg nTime: Delay_ms( 1 ) ��ʵ�ֵ���ʱΪ 1 ms
 * @retval  ��
 */

void Delay_ms(__IO u32 nTime) {
    TimingDelay = nTime;

    while (TimingDelay != 0)
        ;
}

/**
 * @brief   s��ʱ����
 * @param
 *		@arg ms: Delay_s( 1 ) ��ʵ�ֵ���ʱΪ 1s
 * @retval  ��
 */
void Delay_s(unsigned int ms) {
    unsigned char i;
    for (i = 0; i < ms; ms--) {
        Delay_ms(1000);
    }
}

/**
 * @brief  ��ȡ���ĳ���
 * @param  ��
 * @retval ��
 * @attention  �� SysTick �жϺ��� SysTick_Handler()����
 */
void TimingDelay_Decrement(void) {
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

/**
 * @brief  ��ȡ��ǰ����ֵ
 * @param  �洢���º���ֵ�ı���
 * @retval ��
 */
int get_tick_count(unsigned long *count) {
    count[0] = g_ul_ms_ticks;
    return 0;
}

/**
 * @brief  �����ۼ��������ж���ÿ�����1
 * @param  ��
 * @retval ��
 */
void TimeStamp_Increment(void) { g_ul_ms_ticks++; }
/*********************************************END OF FILE**********************/
