#ifndef _KEY_H
#define _KEY_H

#include <stdint.h>
#include "ti_msp_dl_config.h"
#include "board.h"

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

void OLED_UpdateTarget(void);
void OLED_UpdateProgress(void);
void OLED_UpdateStatus(void);
void OLED_UpdateDebug(void);

#endif
