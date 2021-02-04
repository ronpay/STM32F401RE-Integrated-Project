#include "motor.h"
#include "receiver.h"
#include "stm32f4xx.h"
#include "sysTick.h"
#include "tim.h"

#define ReceiverDelayReady 0
#define NumOfReciver 6

volatile uint32_t CapVal[6] = {0};      //��һ���½��ؼ���ֵ
volatile uint8_t captureFlag[6] = {0};  //����״̬
double Duty[6] = {0};
int Receiver_Delay[6] = {0};
u8 hm_flag = '2';
const float Cycle = 20000;
extern uint32_t Task_Delay[];

void USART6_IRQHandler(void) {
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
        hm_flag = USART_ReceiveData(USART6);
        if (hm_flag == '0') {
            printf("Ble is printing the data of mpu6050");
        } else if (hm_flag == '1') {
            printf("Ble is printing the data of reciver");
        } else if (hm_flag == '2') {
            ;
        } else {
            printf("Wrong input, please enter 0 or 1, without CR/LF");
        }
    }
}

void SysTick_Handler(void) {
    TimingDelay_Decrement();

    uint8_t i;
    for (i = 0; i < NumOfTask; i++) {
        if (Task_Delay[i]) {
            Task_Delay[i]--;
        }
        for (i = 0; i < NumOfReciver; i++) {
            if (Receiver_Delay[i]) {
                Receiver_Delay[i]--;
            }
        }
    }
}

void TIM3_IRQHandler(void) {
    int receiverNum;
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
        receiverNum = 0;

        switch (captureFlag[receiverNum]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                captureFlag[receiverNum]=1;
                break;
            case 1:
                CapVal[receiverNum] = TIM_GetCapture1(TIM3);
                Duty[receiverNum] = (float)((int)CapVal[receiverNum] % (int)Cycle);

                Motor_Set(Duty[receiverNum], receiverNum + 1);
                TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                captureFlag[receiverNum] = 0;

                if (hm_flag == '1' &&
                    Receiver_Delay[receiverNum] == ReceiverDelayReady) {
                    //						printf("Cycle:%f\n",Cycle);
                    //						printf("High:%f\n",(float)CapVal[i]);
                    printf("CH%i:%.2f percent\n", receiverNum + 1,
                           100 * Duty[receiverNum]);  // 5-10%

                    //						printf("Cycle:%f\n",Cycle);
                    //						printf("High:%f\n",(float)CapVal[i]);
                    printf("CH3:%.2f percent\n",
                           100 * (0.15f - Duty[receiverNum]));  // 5-10%
                    //						Delay_ms(200);
                    Receiver_Delay[receiverNum] = 500;
                }
                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update);
    }

    else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) {
        receiverNum = 1;

        switch (captureFlag[receiverNum]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                captureFlag[receiverNum]=1;
                break;
            case 1:
                CapVal[receiverNum] = TIM_GetCapture2(TIM3);
                Duty[receiverNum] = (float)((int)CapVal[receiverNum] % (int)Cycle);
                Motor_Set(Duty[receiverNum], receiverNum + 1);
                TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                captureFlag[receiverNum] = 0;

                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2 | TIM_IT_Update);
    }

    else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET) {
        receiverNum = 2;

        switch (captureFlag[receiverNum]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                captureFlag[receiverNum]=1;
                break;
            case 1:
                CapVal[receiverNum] = TIM_GetCapture3(TIM3);
                Duty[receiverNum] = (float)((int)CapVal[receiverNum] % (int)Cycle);
                Motor_Set(Duty[receiverNum], receiverNum + 1);
                TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                captureFlag[receiverNum] = 0;

                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3 | TIM_IT_Update);
    }

    else if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET) {
        receiverNum = 3;

        switch (captureFlag[receiverNum]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                captureFlag[receiverNum]=1;
                break;
            case 1:
                CapVal[receiverNum] = TIM_GetCapture4(TIM3);
                Duty[receiverNum] = (float)((int)CapVal[receiverNum] % (int)Cycle);
                Motor_Set(Duty[receiverNum], receiverNum + 1);
                TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                captureFlag[receiverNum] = 0;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4 | TIM_IT_Update);
    }
}

void HardFault_Handler(void) {
    while (1)
        ;
}
