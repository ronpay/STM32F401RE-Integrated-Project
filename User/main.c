#include "LED.h"
#include "hm10.h"
#include "hmc5883l.h"
#include "motor.h"
#include "mpu6050.h"
#include "oled.h"
#include "receiver.h"
#include "stm32f4xx.h"
#include "sysTick.h"

#define HM10_EN 1
#define RECEIVER_EN 1
#define GY86_EN 1
#define MOTOR_EN 1
#define OLED_EN 1
#define LED_EN 0

uint32_t Task_Delay[NumOfTask] = {0};

extern u8 hm_flag;
extern double Duty[6];

int main(void) {
    /*Test for Sysclk*/
    RCC_ClocksTypeDef get_rcc_clock;
    RCC_GetClocksFreq(&get_rcc_clock);

#if HM10_EN
    HM10_Config();

#endif

#if OLED_EN

    OLED_Config();
    OLED_Init();

    OLED_Fill(0xFF);  //全屏点亮
    Delay_s(1);       // 1s

    OLED_Fill(0x00);  //全屏灭
    Delay_s(1);       // 1s

    OLED_ShowStr(0, 4, (unsigned char*)"Hello Group D", 2);  //测试8*16字符
    Delay_s(1);
    OLED_CLS();  //清屏
    OLED_OFF();  //测试OLED休眠
    Delay_s(1);  // 1s
    OLED_ON();   //测试OLED休眠后唤醒

#endif
    
#if LED_EN

    LED_Config();
#endif

#if MOTOR_EN
    
    Motor_Config();
    Motor_Unlock();
    /*Test Function*/
    //		while(1){
    //			Motor_Test();
    //		}
#endif

#if RECEIVER_EN

    Receiver_Config();

#endif

#if GY86_EN

    short Acel[3];
    short Gyro[3];
    float Temp;
    short Me[3];

    GY86_Init();

#endif

#if HM10_EN
    while (1) {
        //
        //  TASK ONE
        //
#if GY86_EN
        if (Task_Delay[0] == TASK_ENABLE && hm_flag == '0') {
            MPU6050ReadAcc(Acel);
            printf("\nAcceleration: %8d%8d%8d\n", Acel[0], Acel[1], Acel[2]);

            MPU6050ReadGyro(Gyro);
            printf("Gyroscope:    %8d%8d%8d\n", Gyro[0], Gyro[1], Gyro[2]);

            MPU6050_ReturnTemp(&Temp);
            printf("Temperature:  %8.2f\n", Temp);

            HMC5884LReadMe(Me);
            printf("MagneticField:%8d%8d%8d\n", Me[0], Me[1], Me[2]);

#if OLED_EN
            OLED_Show_3num(Acel[0], Acel[1], Acel[2], 1);
            OLED_Show_3num(Gyro[0], Gyro[1], Gyro[2], 0);
            OLED_ShowNum(24, 7, Temp, 2, 12);
            OLED_Show_3num(Me[0], Me[1], Me[2], 2);
            OLED_Show_3num(100 * Duty[0], 100 * Duty[1], 100 * Duty[2], 3);
            OLED_Show_3num(100 * Duty[3],100 * Duty[4], 100 * Duty[5], 4);
#endif
            Task_Delay[0] =
                500;  //更新一次数据，可根据自己的需求，提高采样频率，如100ms采样一次
        }
#endif
        //
        //  TASK TWO
        //
        if (Task_Delay[1] == TASK_ENABLE) {
#if RECEIVER_EN
            if (hm_flag == '1') {
                for (int i = 0; i < 6; i++) {
                    if (Duty[i] > 0.01) {
                        printf("CH%i:%.2f %% \n", i + 1, 100 * Duty[i]);
                    }
                }
            }
#endif
#if MOTOR_EN
            for (int i = 0; i < 4; i++) {
                Motor_Set(Duty[i], i + 1);
            }
#endif
            Task_Delay[1] = 500;
        }
    }
#endif

    while (1){
        ;
        }
}
