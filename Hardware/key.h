#ifndef _KEY_H
#define _KEY_H
#include "ti_msp_dl_config.h"
#include "board.h"

// Key scanning functions
uint8_t click_N_Double(uint8_t time);   // Single/double click detection
uint8_t click(void);                    // Single click detection
uint8_t Long_Press(void);               // Long press detection
uint8_t Read_Key1_Click(void);
uint8_t Read_Key2_Click(void);

// Key handling
void Handle_Keys(void);
int Get_Key_Next(void);
int Get_Key_Confirm(void);

// L2 lap counting
void Check_L2_Update(void);

// OLED display functions
void OLED_UpdateTarget(void);
void OLED_UpdateProgress(void);
void OLED_UpdateStatus(void);
void OLED_UpdateDebug(void);

#endif
