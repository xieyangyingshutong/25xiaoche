// key_logic.c
#include "key.h"
#include "oled.h"
#include "ti_msp_dl_config.h"

int targetCount = 1;      // Target lap count set by user
int count1 = 0;           // L2 triggers count per lap (4 per lap)
int count2 = 0;           // Completed lap count
int carRunning = 0;       // 1=running, 0=stopped


uint32_t lastCount1Time = 0; // Last time count1 incremented (tick_ms)
int L2_last = 0;


// ---------------------- Key detection functions ----------------------
int Get_Key_Next(void) {
    if (!DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_KEY1_PIN)) {
        delay_ms(10);
        if (!DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_KEY1_PIN)) return 1;
    }
    return 0;
}
int Get_Key_Confirm(void) {
    if (!DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_KEY2_PIN)) {
        delay_ms(10);
        if (!DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_KEY2_PIN)) return 1;
    }
    return 0;
}


// ---------------------- Key handling logic ----------------------
void Handle_Keys(void) {
    if (!carRunning) {
        if (Get_Key_Next()) {
            targetCount++;
            if (targetCount > 5) targetCount = 1;
            OLED_UpdateTarget();
            delay_ms(300);
        }
        if (Get_Key_Confirm()) {
            carRunning = 1;
            count1 = 0;
            count2 = 0;
            lastCount1Time = tick_ms;
            OLED_ShowString(3, 1, "follow...", 1);
            OLED_UpdateProgress();
            delay_ms(300);
        }
    }
}

// ---------------------- L2 detection + lap counting ----------------------
void Check_L2_Update(void) {
    int L2_current = DL_GPIO_readPins(SENSOR_L2_PORT, SENSOR_L2_PIN) ? 1 : 0;

    if (carRunning && L2_current == 1 && L2_last == 0) {
        count1++;
        lastCount1Time = tick_ms;
    }
    L2_last = L2_current;

    // Timeout: if no L2 edge detected for 3000ms, complete current lap
    if ((tick_ms - lastCount1Time > 3000) && count1 > 0) {
        if (count1 >= 4) {
            count1 = 0;
            count2++;
            OLED_UpdateProgress();
        } else {
            count1 = 0;
        }
    }

    if (count2 >= targetCount) {
        carRunning = 0;
        Set_PWM(0,0);
        OLED_ShowString(3, 1, "finish stop", 1);
    }
}

void OLED_UpdateTarget(void) {
    OLED_ShowString(1, 1, "Target:", 1);             // Line 1: target lap count
    OLED_ShowNum(1, 9, targetCount, 1, 1);           // Display number
}

void OLED_UpdateProgress(void) {
    OLED_ShowString(2, 1, "Done:", 1);               // Line 2: completed laps
    OLED_ShowNum(2, 7, count2, 1, 1);
}

void OLED_UpdateStatus(void) {
    if (carRunning) {
        OLED_ShowString(3, 1, "Status: Running ", 1); // Line 3: status
    } else {
        OLED_ShowString(3, 1, "Status: Stopped ", 1);
    }
}

void OLED_UpdateDebug(void) {
    OLED_ShowString(4, 1, "C1:", 1);                 // Display count1
    OLED_ShowNum(4, 4, count1, 2, 1);
    OLED_ShowString(4, 7, "C2:", 1);                 // Display count2
    OLED_ShowNum(4, 10, count2, 1, 1);
}
