#ifndef __HUIDU_H_
#define __HUIDU_H_

#include <stdint.h>
extern volatile int8_t L2, L1, R1, R2, M;
extern float Track;
extern float Last_Track;
extern float a;

// º¯ÊıÉùÃ÷
void Update_Sensor_State(void);
int BiaoZhi(void);

#endif 
