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

<<<<<<< HEAD
int main(void)
{
    SYSCFG_DL_init();
    /* Use the 1 ms tick required by delay_ms(), key debounce and lap timing. */
    SysTick_Init();
=======
// External variables from key.c
extern int carRunning;

// Function from PID.c
void Line_Tracking_Control(void);

int main(void)
{
    SYSCFG_DL_init();
>>>>>>> e1aa0c5f8d19d4bbc523bcdc01b68d317442ca46
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
<<<<<<< HEAD
    OLED_UpdateProgress();
    OLED_UpdateStatus();
=======
    OLED_ShowString(3, 1, "waiting", 1);
>>>>>>> e1aa0c5f8d19d4bbc523bcdc01b68d317442ca46
    Set_PWM(0,0);

    while (1)
    {
        Handle_Keys();
        if (carRunning) {
            if (Line_Tracking_Control() != 0U) {
                Check_L2_Update();
            } else {
                Stop_Car_OnLineLost();
            }
        } else {
            Set_PWM(0,0);
        }
        OLED_UpdateStatus();
        OLED_UpdateDebug();
        delay_ms(5);
    }
}
