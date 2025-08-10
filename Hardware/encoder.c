#include "encoder.h"
#include "led.h"
uint32_t gpio_interrup1,gpio_interrup2;
int Get_Encoder_countA,Get_Encoder_countB;
int32_t encoderA_cnt,PWMA,encoderB_cnt,PWMB;
/*******************************************************
函数功能：外部中断模拟编码器信号
入口函数：无
返回  值：无
***********************************************************/
void GROUP1_IRQHandler(void)
{
	//获取中断信号
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

//10ms定时中断
//void TIMER_0_INST_IRQHandler(void)
//{
//    if(DL_TimerA_getPendingInterrupt(TIMER_0_INST))
//    {
//        if(DL_TIMER_IIDX_ZERO)
//        {
//			LED_Flash(100);//led闪烁
//			Key();//获取当前BLS按键状态
//			encoderA_cnt = Get_Encoder_countA;//两个电机安装相反，其中一个编码器值需要相反
//			encoderB_cnt = -Get_Encoder_countB;
//			Get_Encoder_countA=Get_Encoder_countB=0;
//			if(!Flag_Stop)//单击BLS开启或关闭电机
//			{
//				PWMA = -Velocity_A(-15,encoderA_cnt);//PID控制
//				PWMB = -Velocity_B(-15,encoderB_cnt);//PID控制
//				//方波限幅(-7999~7999),频率10khz
//				PWMA=limit_PWM(PWMA,-7999,7999);
//				PWMB=limit_PWM(PWMB,-7999,7999);
//				Set_PWM(PWMA,PWMB);//PWM波驱动电机
//			}else Set_PWM(0,0);//关闭电机
//			
//		}
//    }
//}

void TIMER_0_INST_IRQHandler(void)
{
	//编码器速度计算
	if( DL_TimerG_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO)
	{
        /* 两个电机安装相反，所以编码器值也要相反 */
        encoderA_cnt = Get_Encoder_countA;
        encoderB_cnt = -Get_Encoder_countB;

        /* 编码器计数值清零 */
        Get_Encoder_countA = 0;
        Get_Encoder_countB = 0;
	}

}
