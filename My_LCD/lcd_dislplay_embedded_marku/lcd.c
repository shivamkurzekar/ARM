#include "pll.h"
#include "km_tm4c123gh6pm.h"	
#include "kmlib1.h"
int SYSCTL_Init();
int SysTickTimer_Init();
int write_data(char );
  void Enable(void)  
 {
	 
	 GPIO_PORTA_DATA_R |=(0x40);
   KM_delay_ms(5);
   GPIO_PORTA_DATA_R &=~(0x40);	 
 }
 
 void high_nibble(char p)
 {
	GPIO_PORTD_DATA_R &= ~(0x0F);// clear d0 to d3
	GPIO_PORTD_DATA_R |= (p>>4)&0x0F;
  Enable();
 }
 
void low_nibble(char p)
 {
	GPIO_PORTD_DATA_R &= ~(0x0F);// clear d0 to d3
	GPIO_PORTD_DATA_R |= p&0x0F;  
	Enable();
 }
 
 void LCD_Write_Data(char x)
 {
	 GPIO_PORTA_DATA_R |=0x80; // Se Data
	 high_nibble(x);
	 low_nibble(x);
 }
 
 void LCD_Write_Cmd(char x)
 {
	 GPIO_PORTA_DATA_R &=~(0x80); // Celar RS
	 high_nibble(x);
	 low_nibble(x);
}
 int main()
 {  
   KM_PLL_Init();	 //INITIALIZAYION OF SYSTEM CLOCK 50HZ
 
	SYSCTL_Init();
	SysTickTimer_Init();
	  
	GPIO_PORTA_DATA_R &= ~(0xE0);//  clear whole A register

  KM_delay_ms(20);
	 
  KM_delay_ms(5);//delay >4.1msec
	high_nibble(0x30);
	KM_delay_us(120);
	high_nibble(0x30);
	high_nibble(0x20);
  LCD_Write_Cmd(0x07); // Display ON, Cursor OFF
  LCD_Write_Cmd(0x01); // Display Clear
  LCD_Write_Cmd(0x83); // Cursor 1st Line
	LCD_Write_Data('B');
}	

 
 int SYSCTL_Init(void)
 {
	 
	  SYSCTL_RCGCGPIO_R |= (0x09);   //*(0x400FE608) is aphysical addr of RCGCGPIO,0x400FE000 AND 0X608 IS OFFSET ADDRESS
	 //TYPE CASTING IS USED
	 while(!SYSCTL_PRGPIO_R & 0x09)
	 {
		 ;
	 }//wait untill gpio portf pripheral is ready
  GPIO_PORTD_DIR_R |=(0x0F); // SET THE PA5 TO PA7 AS OUTPUT OPERATION
	GPIO_PORTA_DIR_R |=(0xE0); // SET THE PD0 TO PD3 AS OUTPUT OPERATION
  GPIO_PORTD_AFSEL_R &=~(0x0F); //CLEAR PA5 TO PA7 DUE TO USING GPIO
	GPIO_PORTA_AFSEL_R &=~(0xE0); //CLEAR PD0 TO PD3 DUE TO USING GPIO
	GPIO_PORTD_DEN_R |=(0x0F); //ENABLING ALL PINS  
	GPIO_PORTA_DEN_R |=(0xE0); //ENABLING ALL PINS 
	GPIO_PORTD_AMSEL_R &=~(0x0F); //CLEAR PA5 TO PA7 DUE TO USING DIGITAL MODE
	GPIO_PORTA_AMSEL_R &=~(0xE0); //CLEAR PA5 TO PA7 DUE TO USING DIGITAL MODE
	return 0;
 
 }
 
 
 int SysTickTimer_Init1(long int x)
 {
	 NVIC_ST_RELOAD_R=x;
	 while(!NVIC_ST_CTRL_R&0x10000)
	 {
		 ;
	 }	
 }
  int SysTickTimer_Init(long int x)
 {
	 NVIC_ST_CTRL_R |=0x4;//ENABLE SYSTEM CLOCK
	 NVIC_ST_CTRL_R &=~(0x01);///disable systicktimer 
	 NVIC_ST_CURRENT_R =3124;//CLEAR 
	 //100MSEC DELAY
  NVIC_ST_CTRL_R |=0x01;//ENABLE SYSTICK TIMER
	 
 }
