#ifndef __PID_H
#define __PID_H

#include <stdint.h>

// PID控制相关变量（声明为extern，在别的.c文件里定义）
extern float Kp, Ki, Kd;
extern float Error, Last_Error, Track;
extern float LeftSpeed, RightSpeed;
extern float BaseSpeed;

// 函数声明
uint8_t Line_Tracking_Control(void);

#endif
