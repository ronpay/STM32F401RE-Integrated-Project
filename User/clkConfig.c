#include "clkConfig.h"

void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q) {
    __IO uint32_t HSEStartUpStatus = 0;

    // ʹ��HSE�������ⲿ����Ұ��F407ʹ�� HSE=25M
    RCC_HSEConfig(RCC_HSE_ON);

    // �ȴ�HSE�����ȶ�
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS) {
        // ��ѹ����ѹ�����������Ϊ1���Ա�������Ϊ���Ƶ��
        // ����ʱʹ���ܺ͹���ʵ��ƽ��
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_VOS;

        // HCLK = SYSCLK / 1
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        // PCLK2 = HCLK / 2
        RCC_PCLK2Config(RCC_HCLK_Div2);

        // PCLK1 = HCLK / 4
        RCC_PCLK1Config(RCC_HCLK_Div4);

        // ���Ҫ��Ƶ�͵�������������
        // ����PLL��Դʱ�ӣ�����VCO��Ƶ����m������VCO��Ƶ����n��
        //  ����ϵͳʱ�ӷ�Ƶ����p������OTG FS,SDIO,RNG��Ƶ����q
        RCC_PLLConfig(RCC_PLLSource_HSE, m, n, p, q);

        // ʹ��PLL
        RCC_PLLCmd(ENABLE);

        // �ȴ� PLL�ȶ�
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
        }

        /*-----------------------------------------------------*/
        //���� OVER-RIDEģʽ�����ܴﵽ����Ƶ��(F429���иù���)
        //    PWR->CR |= PWR_CR_ODEN;
        //    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
        //    {
        //    }
        //    PWR->CR |= PWR_CR_ODSWEN;
        //    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
        //    {
        //    }
        // ����FLASHԤȡָ,ָ���,���ݻ���͵ȴ�״̬
        FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN |
                     FLASH_ACR_LATENCY_5WS;
        /*-----------------------------------------------------*/

        // ��PLL�ȶ�֮�󣬰�PLLʱ���л�Ϊϵͳʱ��SYSCLK
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        // ��ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
        while (RCC_GetSYSCLKSource() != 0x08) {
        }
    } else {  // HSE����������

        while (1) {
        }
    }
}
