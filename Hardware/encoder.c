#include "encoder.h"
#include "led.h"
volatile uint32_t gpio_interrup1,gpio_interrup2;
volatile int Get_Encoder_countA,Get_Encoder_countB;
volatile int32_t encoderA_cnt,PWMA,encoderB_cnt,PWMB;
/*******************************************************
�������ܣ��ⲿ�ж�ģ��������ź�
��ں�������
����  ֵ����
***********************************************************/
void GROUP1_IRQHandler(void)
{
	//��ȡ�ж��ź�
    gpio_interrup1 = DL_GPIO_getEnabledInterruptStatus(ENCODERA_PORT,ENCODERA_E1A_PIN|ENCODERA_E1B_PIN);
    gpio_interrup2 = DL_GPIO_getEnabledInterruptStatus(ENCODERB_PORT,ENCODERB_E2A_PIN|ENCODERB_E2B_PIN);
    
    
	//encoderA
	if((gpio_interrup1 & ENCODERA_E1A_PIN)==ENCODERA_E1A_PIN)
	{
		if(!DL_GPIO_readPins(ENCODERA_PORT,ENCODERA_E1B_PIN))
		{
			Get_Encoder_countA--;
		}
		else
		{
			Get_Encoder_countA++;
		}
	}
	else if((gpio_interrup1 & ENCODERA_E1B_PIN)==ENCODERA_E1B_PIN)
	{
		if(!DL_GPIO_readPins(ENCODERA_PORT,ENCODERA_E1A_PIN))
		{
			Get_Encoder_countA++;
		}
		else
		{
			Get_Encoder_countA--;
		}
	}
	
	//encoderB
	if((gpio_interrup2 & ENCODERB_E2A_PIN)==ENCODERB_E2A_PIN)
	{
		if(!DL_GPIO_readPins(ENCODERB_PORT,ENCODERB_E2B_PIN))
		{
			Get_Encoder_countB--;
		}
		else
		{
			Get_Encoder_countB++;
		}
	}
	else if((gpio_interrup2 & ENCODERB_E2B_PIN)==ENCODERB_E2B_PIN)
	{
		if(!DL_GPIO_readPins(ENCODERB_PORT,ENCODERB_E2A_PIN))
		{
			Get_Encoder_countB++;
		}                 
		else              
		{                 
			Get_Encoder_countB--;
		}
	}
	DL_GPIO_clearInterruptStatus(ENCODERA_PORT,ENCODERA_E1A_PIN|ENCODERA_E1B_PIN);
	DL_GPIO_clearInterruptStatus(ENCODERB_PORT,ENCODERB_E2A_PIN|ENCODERB_E2B_PIN);
}

//10ms��ʱ�ж�
//void TIMER_0_INST_IRQHandler(void)
//{
//    if(DL_TimerA_getPendingInterrupt(TIMER_0_INST))
//    {
//        if(DL_TIMER_IIDX_ZERO)
//        {
//			LED_Flash(100);//led��˸
//			Key();//��ȡ��ǰBLS����״̬
//			encoderA_cnt = Get_Encoder_countA;//���������װ�෴������һ��������ֵ��Ҫ�෴
//			encoderB_cnt = -Get_Encoder_countB;
//			Get_Encoder_countA=Get_Encoder_countB=0;
//			if(!Flag_Stop)//����BLS������رյ��
//			{
//				PWMA = -Velocity_A(-15,encoderA_cnt);//PID����
//				PWMB = -Velocity_B(-15,encoderB_cnt);//PID����
//				//�����޷�(-7999~7999),Ƶ��10khz
//				PWMA=limit_PWM(PWMA,-7999,7999);
//				PWMB=limit_PWM(PWMB,-7999,7999);
//				Set_PWM(PWMA,PWMB);//PWM���������
//			}else Set_PWM(0,0);//�رյ��
//			
//		}
//    }
//}

void TIMER_0_INST_IRQHandler(void)
{
	//�������ٶȼ���
	if( DL_TimerG_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO)
	{
        /* ���������װ�෴�����Ա�����ֵҲҪ�෴ */
        encoderA_cnt = Get_Encoder_countA;
        encoderB_cnt = -Get_Encoder_countB;

        /* ����������ֵ���� */
        Get_Encoder_countA = 0;
        Get_Encoder_countB = 0;
	}

}
