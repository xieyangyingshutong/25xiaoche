#include "PID.h"
#include "HUIDU.h"
#include "motor.h"

// ������ʼֵ���ɵ���
float Kp = 9.5;
float Ki = 0;
float Kd = 6;
float AmplifyFactor = 5;  // �����Ŵ�ϵ��

float Error = 0;
float Last_Error = 0;
float Track = 0;

float LeftSpeed = 0;
float RightSpeed = 0;
float BaseSpeed = 1650;

void Line_Tracking_Control(void)
{
    Update_Sensor_State();  // ������·������״̬

    // ========= Ȩ�ط����� =========
    // ����Ϊ1����ɫΪ0 
    int sum = 0;
    int weight = 0;

    // ��ÿ��������һ��λ��Ȩ��
    if (L2) { weight += -90; sum += 1; }//�˴�100��Ϊ��ת90����
    if (L1) { weight += -25 ; sum += 1; }
    if (M)  { weight +=  0 ; sum += 1; }
    if (R1) { weight +=  25; sum += 1; }
    if (R2) { weight +=  90; sum += 1; }

    if (sum > 0)  // 
    { 
			// ƽ��ƫ��ֵ��-2 ~ +2��
			Track = (float)weight / sum;
    }

    // ========= PID ���� =========
    Error = Track;  // ����ƫ��

    float dError = Error - Last_Error;
    float Turn = AmplifyFactor * (Kp * Error + Kd * dError);

    LeftSpeed  = BaseSpeed + Turn;
    RightSpeed = BaseSpeed - Turn;

    // �޷�
    if (LeftSpeed > 10000)  LeftSpeed = 10000;
    if (LeftSpeed < 0)      LeftSpeed = 0;
    if (RightSpeed > 10000) RightSpeed = 10000;
    if (RightSpeed < 0)     RightSpeed = 0;

    Set_PWM(LeftSpeed, RightSpeed);
    Last_Error = Error;
}

