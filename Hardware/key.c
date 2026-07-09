#include "key.h"
#include "oled.h"
#include "ti_msp_dl_config.h"

/* A square contains four left corners.  L2 is used as the corner marker. */
#define MIN_TARGET_LAPS        1U
#define MAX_TARGET_LAPS        5U
#define CORNERS_PER_LAP        4U
#define KEY_DEBOUNCE_MS        30U
#define L2_REARM_LOW_MS        50U
#define MIN_CORNER_INTERVAL_MS 600U

uint8_t targetCount = MIN_TARGET_LAPS;
uint8_t count1 = 0U;
uint8_t count2 = 0U;
uint8_t carRunning = 0U;

static uint8_t runFinished = 0U;
static uint8_t runLineLost = 0U;
static uint8_t l2Previous = 0U;
static uint8_t cornerArmed = 0U;
static uint8_t cornerSeen = 0U;
static uint32_t lastCornerTime = 0U;
static uint32_t l2LowSince = 0U;

typedef struct {
    uint8_t rawPressed;
    uint8_t stablePressed;
    uint32_t rawChangedAt;
} DebouncedKey;

static DebouncedKey nextKey = {0U, 0U, 0U};
static DebouncedKey confirmKey = {0U, 0U, 0U};

static uint8_t Key_PressedEvent(DebouncedKey *key, uint8_t rawPressed)
{
    if (rawPressed != key->rawPressed) {
        key->rawPressed = rawPressed;
        key->rawChangedAt = tick_ms;
    }

    if ((key->stablePressed != key->rawPressed) &&
        ((uint32_t)(tick_ms - key->rawChangedAt) >= KEY_DEBOUNCE_MS)) {
        key->stablePressed = key->rawPressed;
        return key->stablePressed;
    }
    return 0U;
}

static void Reset_RunProgress(void)
{
    count1 = 0U;
    count2 = 0U;

    /* Capture the current level so starting on a black line is not a corner. */
    l2Previous = DL_GPIO_readPins(SENSOR_L2_PORT, SENSOR_L2_PIN) ? 1U : 0U;
    cornerArmed = 0U;
    cornerSeen = 0U;
    lastCornerTime = tick_ms;
    l2LowSince = tick_ms;
}

void Handle_Keys(void)
{
    uint8_t nextPressed =
        (DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_KEY1_PIN) == 0U);
    uint8_t confirmPressed =
        (DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_KEY2_PIN) == 0U);

    if (Key_PressedEvent(&nextKey, nextPressed) != 0U) {
        if (carRunning == 0U) {
            if ((runFinished != 0U) || (runLineLost != 0U)) {
                count1 = 0U;
                count2 = 0U;
            }
            targetCount++;
            if (targetCount > MAX_TARGET_LAPS) {
                targetCount = MIN_TARGET_LAPS;
            }
            runFinished = 0U;
            runLineLost = 0U;
            OLED_UpdateTarget();
            OLED_UpdateProgress();
        }
    }

    if (Key_PressedEvent(&confirmKey, confirmPressed) != 0U) {
        if (carRunning == 0U) {
            Reset_RunProgress();
            runFinished = 0U;
            runLineLost = 0U;
            carRunning = 1U;
            OLED_UpdateProgress();
            OLED_UpdateStatus();
        }
    }
}

void Check_L2_Update(void)
{
    uint8_t l2Current = DL_GPIO_readPins(SENSOR_L2_PORT, SENSOR_L2_PIN) ? 1U : 0U;

    if (l2Current == 0U) {
        if (l2Previous != 0U) {
            l2LowSince = tick_ms;
        }
        if ((uint32_t)(tick_ms - l2LowSince) >= L2_REARM_LOW_MS) {
            cornerArmed = 1U;
        }
    } else if ((l2Previous == 0U) && (cornerArmed != 0U) &&
               ((cornerSeen == 0U) ||
                ((uint32_t)(tick_ms - lastCornerTime) >= MIN_CORNER_INTERVAL_MS))) {
        lastCornerTime = tick_ms;
        cornerSeen = 1U;
        cornerArmed = 0U;
        count1++;

        /* A corner must be preceded by a stable L2-low period and be far
         * enough from the prior corner.  This rejects normal steering sway. */
        if (count1 == CORNERS_PER_LAP) {
            count1 = 0U;
            count2++;
            OLED_UpdateProgress();

            if (count2 >= targetCount) {
                carRunning = 0U;
                runFinished = 1U;
                Set_PWM(0, 0);
                OLED_UpdateStatus();
            }
        }
    }
    l2Previous = l2Current;
}

void Stop_Car_OnLineLost(void)
{
    carRunning = 0U;
    runFinished = 0U;
    runLineLost = 1U;
    count1 = 0U;
    Set_PWM(0, 0);
    OLED_UpdateStatus();
}

void OLED_UpdateTarget(void)
{
    OLED_ShowString(1, 1, "Target:", 1);
    OLED_ShowNum(1, 9, targetCount, 1, 1);
}

void OLED_UpdateProgress(void)
{
    OLED_ShowString(2, 1, "Done:", 1);
    OLED_ShowNum(2, 7, count2, 1, 1);
    OLED_ShowString(2, 8, "/", 1);
    OLED_ShowNum(2, 9, targetCount, 1, 1);
}

void OLED_UpdateStatus(void)
{
    static uint8_t lastStatus = 0xFFU;
    uint8_t status;

    if (carRunning != 0U) {
        status = 0U;
    } else if (runFinished != 0U) {
        status = 1U;
    } else if (runLineLost != 0U) {
        status = 2U;
    } else {
        status = 3U;
    }

    if (status == lastStatus) {
        return;
    }
    lastStatus = status;

    if (status == 0U) {
        OLED_ShowString(3, 1, "Status: Running      ", 1);
    } else if (status == 1U) {
        OLED_ShowString(3, 1, "Status: Finished     ", 1);
    } else if (status == 2U) {
        OLED_ShowString(3, 1, "Status: Line Lost    ", 1);
    } else {
        OLED_ShowString(3, 1, "Status: Set 1-5      ", 1);
    }
}

void OLED_UpdateDebug(void)
{
    static uint8_t lastCount1 = 0xFFU;
    static uint8_t lastCount2 = 0xFFU;

    if ((count1 == lastCount1) && (count2 == lastCount2)) {
        return;
    }
    lastCount1 = count1;
    lastCount2 = count2;

    OLED_ShowString(4, 1, "C1:", 1);
    OLED_ShowNum(4, 4, count1, 1, 1);
    OLED_ShowString(4, 7, "C2:", 1);
    OLED_ShowNum(4, 10, count2, 1, 1);
}
