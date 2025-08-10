#ifndef _KEY_H
#define _KEY_H
#include "ti_msp_dl_config.h"
#include "board.h"
 

uint8_t click_N_Double (uint8_t time);  //单击按键扫描和双击按键扫描
uint8_t click(void);               //单击按键扫描
uint8_t Long_Press(void);           //长按扫描
uint8_t Read_Key1_Click(void);
uint8_t Read_Key2_Click(void);
void Handle_Keys(void);
void OLED_UpdateTarget(void);
void OLED_UpdateProgress(void);
int Get_Key_Next(void);
int Get_Key_Confirm(void);
void Check_L2_Update(void);
void OLED_UpdateStatus(void);
void OLED_UpdateDebug(void);
int is_timeout(uint32_t lastTick, uint32_t timeout_ms);
#endif
