#include "km_tm4c123gh6pm.h"
#include "pll.h"

void high_nibble(char);
void low_nibble(char);

void PORTA_INITIALIZATION(void)
{
	SYSCTL_RCGCGPIO_R |= (0x1); 
	while( !(SYSCTL_PRGPIO_R & 0x1));
	
	GPIO_PORTA_DIR_R |= 0xE0;       // OUTPUT DIRECTION;
	GPIO_PORTA_AFSEL_R &= ~(0xE0);  // Disable Alternate Functionality
	GPIO_PORTA_DEN_R |=(0xE0);      // Digital Enable
	GPIO_PORTA_AMSEL_R &= ~(0xE0);  // Analog Mode Disabled
}

void PORTD_INITIALIZATION(void)
{
	SYSCTL_RCGCGPIO_R |= (0x8); 
	while( !(SYSCTL_PRGPIO_R & 0x8));
	
	GPIO_PORTD_DIR_R |= 0xF;       //OUTPUT DIRECTION;
	GPIO_PORTD_AFSEL_R &= ~(0xF);  //initialization
	GPIO_PORTD_DEN_R |=(0xF);      //initialization
	GPIO_PORTD_AMSEL_R &= ~(0xF);  //initialization
}


void sys_tick_timer_init(void)
{
	NVIC_ST_CTRL_R|=0x04;    // enable sytem clock
	NVIC_ST_CTRL_R&= ~(0x1); // Disable system tick timer first
  NVIC_ST_CTRL_R|= 0x01;   // enable sys_tick timer
}





void delayMicroseconds(int delay)
{
	int count = delay*50;                     // Calculate corresponding count 
	NVIC_ST_CURRENT_R = 3214;                   // Make Count flag reset 
	NVIC_ST_RELOAD_R = count;
	while(!(NVIC_ST_CTRL_R&0x10000))
		{
			;
		}
}


//////////////////////-------------LCD__FUNCTION__STARTS------------------/////////////////////////


void enable(void)             // enable the EN bit 
{
	GPIO_PORTA_DATA_R |=(0x40);
	delayMicroseconds(5000);   //Wait for 5ms
	GPIO_PORTA_DATA_R &= ~(0x40);
}

/*
void send_default_data(void)
{  
	
	GPIO_PORTA_DATA_R &= ~(0xA0);
	GPIO_PORTD_DATA_R &= ~(0x0C);
	GPIO_PORTD_DATA_R |=  (0x03);
	enable();
}
*/

void LCD_Write_Cmd(char x)
 {
	 GPIO_PORTA_DATA_R &=~(0x80); // CLEAR RS for 
	 high_nibble(x);
	 low_nibble(x);
}
 void high_nibble(char p)
 {
	GPIO_PORTD_DATA_R &= ~(0x0F);// clear d0 to d3
	GPIO_PORTD_DATA_R |= (p>>4)&0x0F;
  enable();
 }
 
void low_nibble(char p)
 {
	GPIO_PORTD_DATA_R &= ~(0x0F);// clear d0 to d3
	GPIO_PORTD_DATA_R |= p&0x0F;  
	enable();
 }
 
 void LCD_Write_Data(char x)
 {
	 GPIO_PORTA_DATA_R |=0x80; // SET RS 1 for data 
	 high_nibble(x);
	 low_nibble(x);
 }
 
void initialize_LCD(void)
{
	/*
	delayMicroseconds(15000); //wait for 15ms on Power ON 
	send_default_data();              //send the data [Rs R/W DB0 DB6 DB5 DB4] = [0 0 0 0 1 1] //to use 4 pins
	delayMicroseconds(4200);  //wait for 4.2ms 
	send_default_data();              //send the data [Rs R/W DB0 DB6 DB5 DB4] = [0 0 0 0 1 1] //to use 4 pins
	delayMicroseconds(150);   //wait for > 100us 
	send_default_data();              //send the data [Rs R/W DB0 DB6 DB5 DB4] = [0 0 0 0 1 1] //to use 4 pins
	
	GPIO_PORTA_DATA_R &= ~(0xA0);  // Set interface to 4 bit length
	GPIO_PORTD_DATA_R &= ~(0x0D);  
	GPIO_PORTD_DATA_R =  (0x02);
	enable();
	
	GPIO_PORTA_DATA_R &= ~(0xA0);  // Display Clear
  GPIO_PORTD_DATA_R =  (0x01);  
	enable();
	GPIO_PORTD_DATA_R =  (0x0F);   // cursor blink
	enable();
	//GPIO_PORTD_DATA_R =z 0x2;       // Send 28 to data pins (for 5*7 matrix)
  //enable();
	//GPIO_PORTD_DATA_R = 0x8;
  //enable();
	*/
	///////////////--------------INITIALIZE LCD FOR 4 BIT MODE-----------------//////////////////////// 
	GPIO_PORTA_DATA_R &= ~(0xE0);                 //clear corresponding bits on A register
  delayMicroseconds(20000);
  delayMicroseconds(5000);                      //delay > 4.1msec
	high_nibble(0x30);
	delayMicroseconds(120);                       //delay 120 microseconds
	high_nibble(0x30);
	high_nibble(0x20);
	
	///////////////-----------------Start Writing Commands---------------------////////////////////////
	
  LCD_Write_Cmd(0x01);                          // Display Clear
  LCD_Write_Cmd(0x80);                          // Cursor 1st Line
}

void printStringLine1(char str[])
{
	int i=0;

	while(str[i]!='\0')             // Checks for NULL character
	{
		LCD_Write_Data(str[i]);       // Write each character one by one
		i++;
	}
  
}
void printStringLine2(char str[])
{
	int i=0;
	LCD_Write_Cmd(0xC0);            // Initialize the cursor on Line 2
	while(str[i]!='\0')             // Checks for NULL character
	{
		LCD_Write_Data(str[i]);       // Write each character one by one
		i++;
	}
  
}

void print_int(int temp)
{
	char num[3];
	char unit, tens;
	if(temp>=0 && temp<=9)
	{
		num[0] = (48+temp);
		num[1] = '\0';
		printStringLine1(num);                   // Print the string on line 1
	}
	
	else if(temp >= 10 && temp <= 99)
	{
		tens = temp/10;
		unit = (temp - (tens*10));
		num[0] = (48+tens);
		num[1] = (48+unit);
		num[2] = '\0';
		printStringLine1(num);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//________________________________MAIN_FUNCTION___________________________________________//
////////////////////////////////////////////////////////////////////////////////////////////

int main()
{ 
	int temp = 55;
	char string1[16] = ".";                 // String to be displayed on first line of LCD
	char string2[16] = "1234567891234567";       // String to be displayed on second line of LCD
	PORTA_INITIALIZATION();                      // Initializing corresponding pins on Port A
	PORTD_INITIALIZATION();                      // Initializing corresponding pins on Port D
	KM_PLL_Init();                               // Initializing of system clock for 50 MHz
	sys_tick_timer_init();                       // Initializing System Tick Timer
	initialize_LCD();                            // Initializing LCD display for 4 Bit mode
	
//	LCD_Write_Data('A');
//	LCD_Write_Data('B');
//	LCD_Write_Data('B');
//	LCD_Write_Cmd(0xC0);
//	LCD_Write_Data('A');
//	LCD_Write_Data('B');
	LCD_Write_Cmd(0x80);            // Initialize the cursor on Line 1
	printStringLine1("humidity:");                   // Print the string on line 1
  print_int(temp);
	printStringLine1(string1);                   // Print the string on line 1
//	printStringLine2(string2);                   // Print the dtring on line 2
	print_int(temp);
	LCD_Write_Cmd(0xC0);            // Initialize the cursor on Line 1
	printStringLine1("Temp:  ");                   // Print the string on line 1
  print_int(temp);
	printStringLine1(string1);                   // Print the string on line 1
//	printStringLine2(string2);                   // Print the dtring on line 2
	print_int(temp);
	return 0;
}
