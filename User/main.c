#include "LED.h"
#include "hm10.h"
#include "hmc5883l.h"
#include "motor.h"
#include "mpu6050.h"
#include "oled.h"
#include "receiver.h"
#include "stm32f4xx.h"
#include "sysTick.h"

#define HM10_ON 1
#define RECEIVER_ON 1
#define GY86_ON 1
#define MOTOR_ON 1
#define USE_OLED 1
#define LED_ON 0

uint32_t Task_Delay[NumOfTask] = {0};

u32 temp = 0;
float set = 0;
extern u8 hm_flag;
int main(void) {
    /*Test for Sysclk*/
    		RCC_ClocksTypeDef get_rcc_clock;
    		RCC_GetClocksFreq(&get_rcc_clock);
    SysTick_Init();

#if HM10_ON
    HM10_Config();

#endif

#if USE_OLED

    OLED_Config();
    OLED_Init();

    OLED_Fill(0xFF);  //ȫ������
    Delay_s(1);       // 1s

    OLED_Fill(0x00);  //ȫ����
    Delay_s(1);       // 1s

    OLED_ShowStr(0, 4, (unsigned char*)"Hello Group D", 2);  //����8*16�ַ�
    Delay_s(1);
    OLED_CLS();  //����
    OLED_OFF();  //����OLED����
    Delay_s(1);  // 1s
    OLED_ON();   //����OLED���ߺ���

#endif

#if LED_ON

    LED_Config();
#endif

#if MOTOR_ON

    Motor_Config();
    Motor_Unlock();
    /*Test Function*/
//		while(1){
//			Motor_Test();
//		}
#endif

#if RECEIVER_ON

    Receiver_Config();

#endif

#if GY86_ON

    short Acel[3];
    short Gyro[3];
    float Temp;
    short Me[3];
    extern float Duty[6];
    unsigned int Cap[6];
    GY86_Init();

    //���MPU6050
    for (int i = 0; i < 6; i++) {
        Cap[i] = (Duty[i] * 10000);
    }
    if (MPU6050ReadID() == 1) {
        while (1) {
            if (Task_Delay[0] == TASK_ENABLE) {
                Task_Delay[0] = 1000;
            }
            if (Task_Delay[1] == 0 && hm_flag == '0') {
                MPU6050ReadAcc(Acel);
                printf("\nAcceleration: %8d%8d%8d\n", Acel[0], Acel[1],
                       Acel[2]);

                MPU6050ReadGyro(Gyro);
                printf("Gyroscope:    %8d%8d%8d\n", Gyro[0], Gyro[1], Gyro[2]);

                MPU6050_ReturnTemp(&Temp);
                printf("Temperature:  %8.2f\n", Temp);

                HMC5884LReadMe(Me);
                printf("MagneticField:%8d%8d%8d\n", Me[0], Me[1], Me[2]);

#if USE_OLED
                OLED_Show_3num(Acel[0], Acel[1], Acel[2], 1);
                OLED_Show_3num(Gyro[0], Gyro[1], Gyro[2], 0);
                OLED_ShowNum(24, 7, Temp, 2, 12);
                OLED_Show_3num(Me[0], Me[1], Me[2], 2);
                OLED_Show_3num(Cap[0], Cap[1], Cap[2], 3);
                OLED_Show_3num(Cap[3], Cap[4], Cap[5], 4);
#endif

                Task_Delay[1] =
                    500;  //����һ�����ݣ��ɸ����Լ���������߲���Ƶ�ʣ���100ms����һ��
            }
        }

    } else {
        printf("\r\nû�м�⵽MPU6050��������\r\n");
#if OLED_ON

#endif
        while (1)
            ;
    }
#endif

    // while(1);
}
