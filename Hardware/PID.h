#ifndef __PID_H
#define __PID_H

// PID������ر���������Ϊextern���ڱ��.c�ļ��ﶨ�壩
extern float Kp, Ki, Kd;
extern float Error, Last_Error, Track;
extern float LeftSpeed, RightSpeed;
extern float BaseSpeed;

// ��������
void Line_Tracking_Control(void);

#endif
