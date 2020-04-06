#include "km_tm4c123gh6pm.h"
#include "pll.h"
// 0x400FE000 base address of system control and 608 offset address of RCGCGPIO and offset of PRGPIO is A08
#define SYSCTL_RCGCGPIO  *((int *)(0x400FE608)) 
#define SYSCTL_PRGPIO    *((int *)(0x400FEA08))  
#define GPIOPORTF_DIR    *((int *)(0x40025400))	
#define GPIOPORTF_AFSEL  *((int *)(0x40025420))
#define GPIOPORTF_DEN    *((int *)(0x4002551C))
#define GPIOPORTF_AMSEL  *((int *)(0x40025528))
#define GPIOPORTF_DATA   *((int *)(0x400253FC))
int x=0;
void WaitForInterrupt(void);

void SysTick_Handler(void)  //ISR of sys tick handler
{
	GPIOPORTF_DATA ^= 0x02; //toggles RED LED
	NVIC_ST_CURRENT_R=3214; //clear on write 
}

void PORTF_INITIALIZATION(void)
{
	SYSCTL_RCGCGPIO |= (0x20);  // Enable GPIO PORT F CLOCK
	while( !(SYSCTL_PRGPIO & 0x20))
	{
		;
	}		// Wait untill PORT F peripheral is READY
	
	GPIOPORTF_DIR |= 0x02;     //OUTPUT DIRECTION;
	GPIOPORTF_AFSEL &= ~(0x02);  //initialization
	GPIOPORTF_DEN |=(0x02);  //initialization
	GPIOPORTF_AMSEL &= ~(0x02);  //initialization
	
}

void sys_tick_timer_Init(void)
{
	NVIC_ST_CTRL_R|=0x04; //Enable sytem clock
	NVIC_ST_CTRL_R&= ~(0x1); //Disable system tick timer first
	
	NVIC_ST_CURRENT_R=3214;
	NVIC_ST_RELOAD_R = 50000000;
	NVIC_ST_CTRL_R|= 0x03;
}

void EnableInterrupts(void);
int main()
{
	KM_PLL_Init();
	sys_tick_timer_Init();
	PORTF_INITIALIZATION();
	//EnableInterrupts();
	while(1)  
	{
		WaitForInterrupt();
	}
}
