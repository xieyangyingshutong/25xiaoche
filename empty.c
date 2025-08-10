/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "board.h"
#include "HUIDU.h"
#include "PID.h"
#include "oled.h"

//定义/声明变量和常量
extern int carRunning; 

//定义函数体
void Line_Tracking_Control(void); 

int main(void)
{
	int i=0;
	
    SYSCFG_DL_init();
	DL_Timer_startCounter(PWM_0_INST);
	NVIC_ClearPendingIRQ(ENCODERA_INT_IRQN);
    NVIC_ClearPendingIRQ(ENCODERB_INT_IRQN);
	NVIC_EnableIRQ(ENCODERA_INT_IRQN);
    NVIC_EnableIRQ(ENCODERB_INT_IRQN);
	NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
	delay_ms(100);
	OLED_Init();
	OLED_CLS();
	OLED_UpdateTarget();
    OLED_ShowString(3, 1, "waiting", 1);
    Set_PWM(0,0);
	
    while (1) 
    {
//			Key();
//			if (!Flag_Stop)
//			{
//			Update_Sensor_State();
//			printf("L2:%d L1:%d M:%d R1:%d R2:%d | Track: %.2f | L:%d R:%d\n", 
//       L2, L1, M, R1, R2, Track, (int)LeftSpeed, (int)RightSpeed);
			Handle_Keys();
        if (carRunning) {
            Line_Tracking_Control();
            Check_L2_Update();
        } else {
            Set_PWM(0,0);
        }
		OLED_UpdateStatus();
    OLED_UpdateDebug(); 
    }
//			Line_Tracking_Control();
//			Key_Display();
////			}
////			else
////			{
////				Set_PWM(0,0);
////			}
//			uint8_t key1_state = DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_PIN);
//			uint8_t key2_state = DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_PIN);
//			printf("Key1=%d, Key2=%d\r\n", key1_state, key2_state);

			delay_ms(5);//5?
    }


