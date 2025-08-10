#include "ti_msp_dl_config.h"
#include "board.h"

volatile unsigned long tick_ms;
volatile uint32_t start_time;


void SysTick_Init(void)
{
    DL_SYSTICK_config(CPUCLK_FREQ / 1000);  // ÿ�������һ���ж�
    NVIC_SetPriority(SysTick_IRQn, 0);
}


////����SysTick����ֵ
uint32_t Systick_getTick(void)
{
	return (SysTick ->VAL);
}


//ms�����ӳ�
void delay_ms(uint32_t ms)
{
	//���������������ӳ�
	//if( ms > SysTickMAX_COUNT/(SysTickFre/1000) ) ms = SysTickMAX_COUNT/(SysTickFre/1000);
	for(int i=0;i<1000;i++)
	{
		delay_us(ms);
	}
}

//void delay_ms(uint32_t ms)
//{
//    uint32_t start = Systick_getTick();
//    while((Systick_getTick() - start) < ms);
//}


void delay_us(uint32_t us)
{
	if( us > SysTickMAX_COUNT/(SysTickFre/1000000) ) us = SysTickMAX_COUNT/(SysTickFre/1000000);
	
	us = us*(SysTickFre/1000000); //��λת��
	
	//���ڱ������߹���ʱ��
	uint32_t runningtime = 0;
	
	//��õ�ǰʱ�̵ļ���ֵ
	uint32_t InserTick = Systick_getTick();
	
	//����ˢ��ʵʱʱ��
	uint32_t tick = 0;
	
	uint8_t countflag = 0;
	//�ȴ��ӳ�
	while(1)
	{
		tick = Systick_getTick();//ˢ�µ�ǰʱ�̼���ֵ
		
		if( tick > InserTick ) countflag = 1;//���������ѯ,���л���ʱ�ļ��㷽ʽ
		
		if( countflag ) runningtime = InserTick + SysTickMAX_COUNT - tick;
		else runningtime = InserTick - tick;
		
		if( runningtime>=us ) break;
	}

}

void delay_1us(unsigned long __us){ delay_us(__us); }
void delay_1ms(unsigned long ms){ delay_ms(ms); }

#if !defined(__MICROLIB)
//��ʹ��΢��Ļ�����Ҫ�������ĺ���
#if (__ARMCLIB_VERSION <= 6000000)
//�����������AC5  �Ͷ�����������ṹ��
struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
#endif

//printf�����ض���
int fputc(int ch, FILE *stream)
{
	//������0æ��ʱ��ȴ�����æ��ʱ���ٷ��ʹ��������ַ�
	while( DL_UART_isBusy(UART_0_INST) == true );
	
	DL_UART_Main_transmitData(UART_0_INST, ch);
	
	return ch;
}

