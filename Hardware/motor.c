#include "motor.h"
float Velcity_Kp=1.0,  Velcity_Ki=0.4; //����ٶ�PID����
/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��V1.0
�޸�ʱ�䣺2024-07-019

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update��2024-07-019

All rights reserved
***********************************************/
int limit_PWM(int value,int low,int high)
{
	if(value>high) return high;
	else if(value<low) return low;
	else return value;
}


void Set_PWM(int pwmA, int pwmB)
{
    /* The configured PWM timer period is 8000.  Keep every caller inside
     * that range before writing a capture/compare register. */
    pwmA = limit_PWM(pwmA, -MOTOR_PWM_MAX_DUTY, MOTOR_PWM_MAX_DUTY);
    pwmB = limit_PWM(pwmB, -MOTOR_PWM_MAX_DUTY, MOTOR_PWM_MAX_DUTY);

    if (pwmA > 0) {
        DL_GPIO_setPins(AIN_PORT, AIN_AIN1_PIN);
        DL_GPIO_clearPins(AIN_PORT, AIN_AIN2_PIN);
        DL_Timer_setCaptureCompareValue(PWM_0_INST, ABS(pwmA), GPIO_PWM_0_C0_IDX);
    } else if (pwmA < 0) {
        DL_GPIO_setPins(AIN_PORT, AIN_AIN2_PIN);
        DL_GPIO_clearPins(AIN_PORT, AIN_AIN1_PIN);
        DL_Timer_setCaptureCompareValue(PWM_0_INST, ABS(pwmA), GPIO_PWM_0_C0_IDX);
    } else {  // ֹͣʱ�������������
        DL_GPIO_clearPins(AIN_PORT, AIN_AIN1_PIN | AIN_AIN2_PIN);
        DL_Timer_setCaptureCompareValue(PWM_0_INST, 0, GPIO_PWM_0_C0_IDX);
    }

    if (pwmB > 0) {
        DL_GPIO_setPins(BIN_PORT, BIN_BIN1_PIN);
        DL_GPIO_clearPins(BIN_PORT, BIN_BIN2_PIN);
        DL_Timer_setCaptureCompareValue(PWM_0_INST, ABS(pwmB), GPIO_PWM_0_C1_IDX);
    } else if (pwmB < 0) {
        DL_GPIO_setPins(BIN_PORT, BIN_BIN2_PIN);
        DL_GPIO_clearPins(BIN_PORT, BIN_BIN1_PIN);
        DL_Timer_setCaptureCompareValue(PWM_0_INST, ABS(pwmB), GPIO_PWM_0_C1_IDX);
    } else {
        DL_GPIO_clearPins(BIN_PORT, BIN_BIN1_PIN | BIN_BIN2_PIN);
        DL_Timer_setCaptureCompareValue(PWM_0_INST, 0, GPIO_PWM_0_C1_IDX);
    }
}


/***************************************************************************
�������ܣ������PID�ջ�����
��ڲ��������ҵ���ı�����ֵ
����ֵ  �������PWM
***************************************************************************/
int Velocity_A(int TargetVelocity, int CurrentVelocity)
{  
    int Bias;  //������ر���
	static int ControlVelocityA, Last_biasA; //��̬�������������ý�������ֵ��Ȼ����
	Bias=TargetVelocity-CurrentVelocity; //���ٶ�ƫ��
	ControlVelocityA+=Velcity_Ki*(Bias-Last_biasA)+Velcity_Kp*Bias;  //����ʽPI������
                                                               //Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�
	                                                             //Velcity_Ki*Bias             �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
	Last_biasA=Bias;	
	if(ControlVelocityA>7000) ControlVelocityA=7000;
	else if(ControlVelocityA<-7000) ControlVelocityA=-7000;
	return ControlVelocityA; //�����ٶȿ���ֵ
}

/***************************************************************************
�������ܣ������PID�ջ�����
��ڲ��������ҵ���ı�����ֵ
����ֵ  �������PWM
***************************************************************************/
int Velocity_B(int TargetVelocity, int CurrentVelocity)
{  
	int Bias;  //������ر���
	static int ControlVelocityB, Last_biasB; //��̬�������������ý�������ֵ��Ȼ����
	Bias=TargetVelocity-CurrentVelocity; //���ٶ�ƫ��
	ControlVelocityB+=Velcity_Ki*(Bias-Last_biasB)+Velcity_Kp*Bias;  //����ʽPI������
                                                               //Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�
	                                                            //Velcity_Ki*Bias             �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
	Last_biasB=Bias;	
	if(ControlVelocityB>7000) ControlVelocityB=7000;
	else if(ControlVelocityB<-7000) ControlVelocityB=-7000;
	return ControlVelocityB; //�����ٶȿ���ֵ
}

