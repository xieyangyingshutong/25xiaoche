#ifndef _KEY_H
#define _KEY_H

#include <stdint.h>
#include "ti_msp_dl_config.h"
#include "board.h"

<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 4b687cd0abd71fc2d47a59400a67d6c33b945f06
/* Set by the start key and cleared when the requested lap count is reached. */
extern uint8_t carRunning;

/* Progress counters, exposed for the OLED debug page. */
extern uint8_t targetCount;
extern uint8_t count1;     /* detected corners in the current lap: 0..3 */
extern uint8_t count2;     /* completed laps */

/* Key 1 cycles the target from 1 to 5; key 2 starts a new run. */
void Handle_Keys(void);

/* Call once per control loop after the grayscale sensors have been sampled. */
void Check_L2_Update(void);
void Stop_Car_OnLineLost(void);

<<<<<<< HEAD
=======
=======
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
>>>>>>> e1aa0c5f8d19d4bbc523bcdc01b68d317442ca46
>>>>>>> 4b687cd0abd71fc2d47a59400a67d6c33b945f06
void OLED_UpdateTarget(void);
void OLED_UpdateProgress(void);
void OLED_UpdateStatus(void);
void OLED_UpdateDebug(void);

#endif
