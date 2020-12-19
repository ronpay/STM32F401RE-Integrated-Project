#include "motor.h"
#include "receiver.h"
#include "stm32f4xx.h"
#include "sysTick.h"
#include "tim.h"
volatile uint32_t CapVal[6] = {0};  //第一次下降沿计数值
volatile uint8_t Flag[6] = {0};     //捕获状态
float Duty[6] = {0};
int cap = 0;  //正在进行捕获的通道识别
int i = 0;
int f = 0;
int flag = 0;
int cnt = 0;
u8 hm_flag = '2';
float Cycle = 0;

void USART6_IRQHandler(void) {
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
        hm_flag = USART_ReceiveData(USART6);
        if (hm_flag == '0') {
            printf("Ble is printing the data of mpu6050");
        } else if (hm_flag == '1') {
            printf("Ble is printing the data of reciver");
        }
    }
}

extern uint32_t Task_Delay[];

void SysTick_Handler(void) {
    TimingDelay_Decrement();

    uint8_t i;
    for (i = 0; i < NumOfTask; i++) {
        if (Task_Delay[i]) {
            Task_Delay[i]--;
        }
    }
}

void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
        i = 0;

        if (flag == 0) {
            TIM_Cmd(TIM3, DISABLE);
            TIM_SetCounter(TIM3, 0);
            TIM_Cmd(TIM3, ENABLE);
            Cycle = TIM_GetCapture3(TIM3) / 3;
            flag++;
        }

        switch (Flag[i]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                Flag[i]++;
                break;
            case 1:
                CapVal[i] = TIM_GetCapture1(TIM3);
                Duty[i] = CapVal[i] / Cycle;
                Motor_Set(Duty[i], i + 1);
                TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                Flag[i] = 0;

                if (hm_flag == '1') {
                    printf("Cycle:%f", Cycle);
                    printf("High:%f", (float)CapVal[i]);
                    printf("CH%i:%.2f percent\n", i + 1,
                           100 * Duty[i]);  // 5-10%
                    Delay_ms(200);
                }
                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update);
    }

    else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) {
        i = 1;

        if (flag == 0) {
            TIM_Cmd(TIM3, DISABLE);
            TIM_SetCounter(TIM3, 0);
            TIM_Cmd(TIM3, ENABLE);
            Cycle = TIM_GetCapture3(TIM3) / 3;
            flag++;
        }

        switch (Flag[i]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                Flag[i]++;
                break;
            case 1:
                CapVal[i] = TIM_GetCapture2(TIM3);
                Duty[i] = CapVal[i] / Cycle;
                Motor_Set(Duty[i], i + 1);
                TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                Flag[i] = 0;

                if (hm_flag == '1') {
                    printf("Cycle:%f", Cycle);
                    printf("High:%f", (float)CapVal[i]);
                    printf("CH%i:%.2f percent\n", i + 1,
                           100 * Duty[i]);  // 5-10%
                    Delay_ms(200);
                }
                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2 | TIM_IT_Update);
    }

    else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET) {
        i = 2;

        if (flag == 0) {
            TIM_Cmd(TIM3, DISABLE);
            TIM_SetCounter(TIM3, 0);
            TIM_Cmd(TIM3, ENABLE);
            Cycle = TIM_GetCapture3(TIM3);
            flag++;
        }

        switch (Flag[i]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                Flag[i]++;
                break;
            case 1:
                CapVal[i] = TIM_GetCapture3(TIM3);
                Duty[i] = (CapVal[i] / Cycle);
                Motor_Set(Duty[i], i + 1);
                TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                Flag[i] = 0;
                flag = 0;
                cnt++;
                if (hm_flag == '1' && cnt >= 50) {
                    printf("Cycle:%f", Cycle);
                    printf("High:%f", (float)CapVal[i]);
                    printf("CH%i:%.2f percent\n", i + 1,
                           100 * Duty[i]);  // 5-10%
                    cnt = 0;
                }
                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3 | TIM_IT_Update);
    }

    else if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET) {
        i = 3;

        if (flag == 0) {
            TIM_Cmd(TIM3, DISABLE);
            TIM_SetCounter(TIM3, 0);
            TIM_Cmd(TIM3, ENABLE);
            Cycle = TIM_GetCapture3(TIM3) / 3;
            flag++;
        }

        switch (Flag[i]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                Flag[i]++;
                break;
            case 1:
                CapVal[i] = TIM_GetCapture4(TIM3);
                Duty[i] = CapVal[i] / Cycle;
                Motor_Set(Duty[i], i + 1);
                TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                Flag[i] = 0;

                if (hm_flag == '1') {
                    printf("Cycle:%f", Cycle);
                    printf("High:%f", (float)CapVal[i]);
                    printf("CH%i:%.2f percent\n", i + 1,
                           100 * Duty[i]);  // 5-10%
                    Delay_ms(200);
                }
                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4 | TIM_IT_Update);
    }
}

void HardFault_Handler(void) {
    while (1)
        ;
}
