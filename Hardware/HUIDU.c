#include "HUIDU.h"
#include "ti_msp_dl_config.h"

volatile int8_t  L2 = 0, L1 = 0, R1 = 0, R2 = 0,M = 0; 
float Last_Track;
float a;

void Update_Sensor_State(void)
{
    L2 = DL_GPIO_readPins(SENSOR_L2_PORT, SENSOR_L2_PIN) ? 1 : 0;
    L1 = DL_GPIO_readPins(SENSOR_L1_PORT, SENSOR_L1_PIN) ? 1 : 0;
    M  = DL_GPIO_readPins(SENSOR_M_PORT,  SENSOR_M_PIN)  ? 1 : 0;
    R1 = DL_GPIO_readPins(SENSOR_R1_PORT, SENSOR_R1_PIN) ? 1 : 0;
    R2 = DL_GPIO_readPins(SENSOR_R2_PORT, SENSOR_R2_PIN) ? 1 : 0;
}


    // if(Track!=0){ Last_Track = Track; }
    // else if(Track==0){ Track = Last_Track;}
