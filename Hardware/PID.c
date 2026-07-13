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

<<<<<<< HEAD
#define LINE_LOST_STOP_MS 500U

static uint8_t lineLost = 0U;
static uint32_t lineLostSince = 0U;

uint8_t Line_Tracking_Control(void)
{
    Update_Sensor_State();  // Read all 5 grayscale sensor states

=======
void Line_Tracking_Control(void)
{
    Update_Sensor_State();  // Read all 5 grayscale sensor states

>>>>>>> e1aa0c5f8d19d4bbc523bcdc01b68d317442ca46
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
<<<<<<< HEAD
        lineLost = 0U;
    }
    else
    {
        /* A short gap can occur at a corner.  Keeping the previous steering
         * command during that gap is useful, but continuing indefinitely is
         * unsafe if the vehicle has actually left the track. */
        if (lineLost == 0U) {
            lineLost = 1U;
            lineLostSince = tick_ms;
        }
        if ((uint32_t)(tick_ms - lineLostSince) >= LINE_LOST_STOP_MS) {
            Set_PWM(0, 0);
            return 0U;
        }
=======
    }
    else
    {
        // All sensors detect white (off track): keep last known Track
        Track = Track;
>>>>>>> e1aa0c5f8d19d4bbc523bcdc01b68d317442ca46
    }

    // ========= PID control =========
    Error = Track;  // Position deviation

    float dError = Error - Last_Error;
    float Turn = AmplifyFactor * (Kp * Error + Kd * dError);

    LeftSpeed  = BaseSpeed + Turn;
    RightSpeed = BaseSpeed - Turn;

    // Clamp PWM range
<<<<<<< HEAD
    if (LeftSpeed > MOTOR_PWM_MAX_DUTY)  LeftSpeed = MOTOR_PWM_MAX_DUTY;
=======
    if (LeftSpeed > 10000)  LeftSpeed = 10000;
>>>>>>> e1aa0c5f8d19d4bbc523bcdc01b68d317442ca46
    if (LeftSpeed < 0)      LeftSpeed = 0;
    if (RightSpeed > MOTOR_PWM_MAX_DUTY) RightSpeed = MOTOR_PWM_MAX_DUTY;
    if (RightSpeed < 0)     RightSpeed = 0;

    Set_PWM(LeftSpeed, RightSpeed);
    Last_Error = Error;
    return 1U;
}
