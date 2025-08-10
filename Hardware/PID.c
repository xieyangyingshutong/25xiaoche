#include "PID.h"
#include "HUIDU.h"
#include "motor.h"

// 参数初始值（可调）
float Kp = 9.5;
float Ki = 0;
float Kd = 6;
float AmplifyFactor = 5;  // 新增放大系数

float Error = 0;
float Last_Error = 0;
float Track = 0;

float LeftSpeed = 0;
float RightSpeed = 0;
float BaseSpeed = 1650;

void Line_Tracking_Control(void)
{
    Update_Sensor_State();  // 更新五路传感器状态

    // ========= 权重分析法 =========
    // 黑线为1，白色为0 
    int sum = 0;
    int weight = 0;

    // 给每个传感器一个位置权重
    if (L2) { weight += -90; sum += 1; }//此处100是为了转90度弯
    if (L1) { weight += -25 ; sum += 1; }
    if (M)  { weight +=  0 ; sum += 1; }
    if (R1) { weight +=  25; sum += 1; }
    if (R2) { weight +=  90; sum += 1; }

    if (sum > 0)  // 
    { 
			// 平均偏差值（-2 ~ +2）
			Track = (float)weight / sum;
    }

    // ========= PID 控制 =========
    Error = Track;  // 中心偏移

    float dError = Error - Last_Error;
    float Turn = AmplifyFactor * (Kp * Error + Kd * dError);

    LeftSpeed  = BaseSpeed + Turn;
    RightSpeed = BaseSpeed - Turn;

    // 限幅
    if (LeftSpeed > 10000)  LeftSpeed = 10000;
    if (LeftSpeed < 0)      LeftSpeed = 0;
    if (RightSpeed > 10000) RightSpeed = 10000;
    if (RightSpeed < 0)     RightSpeed = 0;

    Set_PWM(LeftSpeed, RightSpeed);
    Last_Error = Error;
}

