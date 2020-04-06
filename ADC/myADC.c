#include "km_tm4c123gh6pm.h"
#include "pll.h"
#include "kmlib1.h"

void sys_tick_timer_init(void)
{
	NVIC_ST_CTRL_R|=0x04;    //Enable sytem clock
	NVIC_ST_CTRL_R&= ~(0x1); //Disable system tick timer first
  NVIC_ST_CTRL_R|= 0x01;   //Enable sys_tick timer
	NVIC_ST_CURRENT_R=3214;
	NVIC_ST_RELOAD_R = 5000000;
}

void ADC_Init(void)
{
	SYSCTL_RCGCADC_R |= 0x03;
}

void PORT_E_Init(void)
{
	SYSCTL_RCGCGPIO_R |= (0x10); 
	while( !(SYSCTL_PRGPIO_R & 0x10));
	
	GPIO_PORTE_AFSEL_R |= (0xC);  //Enable alternate functionality
	GPIO_PORTE_DEN_R  &= ~(0xC);  //Digital Disable
	GPIO_PORTE_AMSEL_R |= (0xC);  //Analog Mode Enable
}

int main()
{
	ADC_Init();
	PORT_E_Init();
	
}