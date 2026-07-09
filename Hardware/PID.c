#include "PID.h"
#include "HUIDU.h"
#include "motor.h"

// PID control gains (adjustable)
float Kp = 9.5;
float Ki = 0;
float Kd = 6;
float AmplifyFactor = 5;  // Amplitude amplification factor

float Error = 0;
float Last_Error = 0;
float Track = 0;

float LeftSpeed = 0;
float RightSpeed = 0;
float BaseSpeed = 1650;

void Line_Tracking_Control(void)
{
    Update_Sensor_State();  // Read all 5 grayscale sensor states

    // ========= Weight method =========
    // Black line = 1, white = 0
    int sum = 0;
    int weight = 0;

    // Assign position weight to each sensor
    if (L2) { weight += -90; sum += 1; }
    if (L1) { weight += -25; sum += 1; }
    if (M)  { weight +=  0;  sum += 1; }
    if (R1) { weight +=  25; sum += 1; }
    if (R2) { weight +=  90; sum += 1; }

    if (sum > 0)
    {
        // Average deviation value: -90 to +90
        Track = (float)weight / sum;
    }
    else
    {
        // All sensors detect white (off track): keep last known Track
        Track = Track;
    }

    // ========= PID control =========
    Error = Track;  // Position deviation

    float dError = Error - Last_Error;
    float Turn = AmplifyFactor * (Kp * Error + Kd * dError);

    LeftSpeed  = BaseSpeed + Turn;
    RightSpeed = BaseSpeed - Turn;

    // Clamp PWM range
    if (LeftSpeed > 10000)  LeftSpeed = 10000;
    if (LeftSpeed < 0)      LeftSpeed = 0;
    if (RightSpeed > 10000) RightSpeed = 10000;
    if (RightSpeed < 0)     RightSpeed = 0;

    Set_PWM(LeftSpeed, RightSpeed);
    Last_Error = Error;
}
