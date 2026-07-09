#include "ti_msp_dl_config.h"
#include "board.h"

volatile unsigned long tick_ms;


void SysTick_Init(void)
{
    DL_SYSTICK_config(CPUCLK_FREQ / 1000);  // Generate interrupt every 1ms
    NVIC_SetPriority(SysTick_IRQn, 0);
}


// Get current SysTick counter value (24-bit down-counter)
uint32_t Systick_getTick(void)
{
	return (SysTick ->VAL);
}

// SysTick interrupt: increment millisecond counter every 1ms
void SysTick_Handler(void)
{
    tick_ms++;
}

// ms delay using tick_ms (SysTick-based millisecond counter)
void delay_ms(uint32_t ms)
{
    uint32_t start = tick_ms;
    while ((tick_ms - start) < ms);
}

// us delay using SysTick down-counter busy-wait
void delay_us(uint32_t us)
{
	if( us > SysTickMAX_COUNT/(SysTickFre/1000000) ) us = SysTickMAX_COUNT/(SysTickFre/1000000);

	us = us*(SysTickFre/1000000); // Unit conversion

	// Variable to store elapsed time
	uint32_t runningtime = 0;

	// Get current tick count
	uint32_t InserTick = Systick_getTick();

	// Refresh real-time timer
	uint32_t tick = 0;

	uint8_t countflag = 0;
	// Wait for delay
	while(1)
	{
		tick = Systick_getTick(); // Refresh current tick count

		if( tick > InserTick ) countflag = 1; // Counter wrap handling

		if( countflag ) runningtime = InserTick + SysTickMAX_COUNT - tick;
		else runningtime = InserTick - tick;

		if( runningtime>=us ) break;
	}

}

void delay_1us(unsigned long __us){ delay_us(__us); }
void delay_1ms(unsigned long ms){ delay_ms(ms); }

#if !defined(__MICROLIB)
// When not using Microlib, need to implement these functions
#if (__ARMCLIB_VERSION <= 6000000)
// For AC5 compiler, define FILE struct
struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

// Define _sys_exit() to avoid using semihosting mode
void _sys_exit(int x)
{
	x = x;
}
#endif

// printf redirect
int fputc(int ch, FILE *stream)
{
	// When UART0 busy, wait; when idle, send next character
	while( DL_UART_isBusy(UART_0_INST) == true );

	DL_UART_Main_transmitData(UART_0_INST, ch);

	return ch;
}
