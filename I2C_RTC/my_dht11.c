#include "km_tm4c123gh6pm.h"
#include "pll.h"
#include "kmlib.h"


void high_nibble(char);
void low_nibble(char);
int Master_Receive_data[8];
int Master_Receive_data_2[8];


void SW_delay(void)
{
	int i=10000000;                         // count for 1 sec
	while(i>0)
	{
		i--;
	}
}

void DHT_Init(void)
{
	SW_delay();
}

void CLOCK_Init(void)
{
	 SYSCTL_RCGCI2C_R |=0X01;               // I2C0 Clock Enable
	 while(!(SYSCTL_PRI2C_R & 0x01));
		
	 SYSCTL_RCGCGPIO_R |= (0x02);           // Port B clock Init
	 while(!(SYSCTL_PRGPIO_R & 0x02));
	
	SYSCTL_RCGCGPIO_R |= (0x1);             // Port A clock Init
	while( !(SYSCTL_PRGPIO_R & 0x1));      
	
	SYSCTL_RCGCGPIO_R |= (0x8);             // Port D clock Init
	while( !(SYSCTL_PRGPIO_R & 0x8));
	
	SYSCTL_RCGCGPIO_R |= (0x10);            // Port E clock Init
	while( !(SYSCTL_PRGPIO_R & 0x10));
}
void PORTA_INITIALIZATION(void)
{
	
	GPIO_PORTA_DIR_R |= 0xE0;       // OUTPUT DIRECTION;
	GPIO_PORTA_AFSEL_R &= ~(0xE0);  // Disable Alternate Functionality
	GPIO_PORTA_DEN_R |=(0xE0);      // Digital Enable
	GPIO_PORTA_AMSEL_R &= ~(0xE0);  // Analog Mode Disabled
}
void PORTD_INITIALIZATION(void)
{
	
	GPIO_PORTD_DIR_R |= 0xF;       //OUTPUT DIRECTION;
	GPIO_PORTD_AFSEL_R &= ~(0xF);  //initialization
	GPIO_PORTD_DEN_R |=(0xF);      //initialization
	GPIO_PORTD_AMSEL_R &= ~(0xF);  //initialization
}
void PORTB_INITIALIZATION(void)
{
	/////////PORT B INITIALIZATION
	GPIO_PORTB_AFSEL_R |=(0x0C);
	GPIO_PORTB_ODR_R |= (0x08); 
	 GPIO_PORTB_DEN_R |= (0x0C);   
	 GPIO_PORTB_PCTL_R =(GPIO_PORTB_PCTL_R & 0XFFFF00FF)|(0X00003300);   //SELECT UART PORT AND MUX POSITION
	
	 GPIO_PORTB_AMSEL_R &=~(0X0C); 
	                                     
}
void PORTE_INITIALIZATION(void)
{
	//GPIO_PORTE_DIR_R |= 0x2;           //DANGER____________
	
	GPIO_PORTE_AFSEL_R &= ~(0x2);  //Disable alternate functionality
	GPIO_PORTE_DEN_R |=(0x2);      //Digital Enable
	GPIO_PORTE_AMSEL_R &= ~(0x2);  //Analog Mode disable
}

 void sys_tick_timer_init(void)
{
	NVIC_ST_CURRENT_R=0;
		NVIC_ST_CTRL_R&= ~(0x1); //Disable system tick timer first
	NVIC_ST_CTRL_R|=0x04;    //Enable sytem clock

	
  NVIC_ST_CTRL_R|= 0x01;   //Enable sys_tick timer
}

void delayMicroseconds(int delay)
{
	int count = delay*50;
	NVIC_ST_CURRENT_R=3214;
	NVIC_ST_RELOAD_R = count;
	while(!(NVIC_ST_CTRL_R&0x10000));
	//NVIC_ST_CTRL_R&= ~(0x1);
}
int DHT_Rx_Data(void)
{
	int i=0, bit=0, data=0,arr[45];
	for (i=0; i<8; i++)
	{

			//KM_delay_us(40);
		 
		 while((GPIO_PORTE_DATA_R & (0x02)) == 0)
		{
			;
		}
		//////////////////km delay working    /////////delay microseconds not working
		
		KM_delay_us(30);
		
		 if((GPIO_PORTE_DATA_R & (0x02)) == 2)
		 {
			 bit = 1;
			  data = (data << 1);
			 data |= bit;
			 //arr[i]=1;
			 while((GPIO_PORTE_DATA_R & (0x02)) == 2);
		 }
	   else
		 {
			 bit = 0;
			  data = (data << 1);
			 data |= bit;
			 //arr[i]=0;
		 }
 }
	return data;
}



void DHT_Request(void)
{
	
	GPIO_PORTE_DIR_R |= (0x02);               // PE1 bit i.e 2nd bit set for O/P operation
	GPIO_PORTE_DATA_R &= ~(0x02);              // RESET PE1 bit in DATA Register
	KM_delay_ms(20);
	GPIO_PORTE_DATA_R |= (0x02);              // SET PE1 bit in DATA Register
	GPIO_PORTE_DIR_R &= ~(0x02);               // PE1 bit i.e 2nd bit set for I/P operation
}


 
 // return 1 - low ; return 0 - High 
int Check_Low()
{
	if (GPIO_PORTE_DATA_R & 0x02)
  return 0;
	else
	return 1;
}

int Check_High()
{
	if (GPIO_PORTE_DATA_R & 0x02)
  return 1;
	else
	return 0;
}

// return 0 - DEVICE FOUND; return 1 -DEVICE NOT FOUND
int DHT11_Response(void)
{
	volatile int count=0,x=0;
	GPIO_PORTE_DIR_R &= ~(0x2);           //INPUT DIRECTION PE1 initialization;
	while(Check_High())                   // wait till the pin goes LOW
	{
		  if (count > 2000)
			{

			x=1;
				return 1;
			}
		 count ++;
	}
	while(Check_Low());  // 80usec low pulse 
	while(Check_High()); // 80usec High pulse
	return x;
}

//////////////////////-------------LCD__FUNCTION__STARTS------------------/////////////////////////


void enable(void)             // enable the EN bit 
{
	GPIO_PORTA_DATA_R |=(0x40);
	delayMicroseconds(5000);   //Wait for 5ms
	GPIO_PORTA_DATA_R &= ~(0x40);
}

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
	//GPIO_PORTD_DATA_R = 0x2;       // Send 28 to data pins (for 5*7 matrix)
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

void printStringLine(char str[])
{
	int i=0;

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
		num[0] = ('0');
		num[1] = (48+temp);
		num[2] = '\0';
		printStringLine(num);                   // Print the string on line 1
	}
	
	else if(temp >= 10 && temp <= 99)
	{
		tens = temp/10;
		unit = (temp - (tens*10));
		num[0] = (48+tens);
		num[1] = (48+unit);
		num[2] = '\0';
		printStringLine(num);
	}
}


	 
void i2c_port_B_config(void)
{
  I2C0_MCR_R =(0X10);                             // I2C master function enable 
	I2C0_MTPR_R =24;                               // Select I2C mode
}


void MasterTransmitMuleipleByte(void)
{
	int n = 0;
	int Master_Transmit_data[7] = {1,8,8,5,36,5,24};
	for(n = 0; n <= 7; n++)
	{
		while(I2C0_MCS_R&0x01){};     // Check Busy bit
		I2C0_MSA_R = (0x68<<1)&0xFE;  // Provide slave addr
		I2C0_MSA_R &= ~0x01;          // Enable write
		I2C0_MDR_R = (n)&0xFF;        // Provide word address
		I2C0_MCS_R = 0x03;            // Start and Run
		while(I2C0_MCS_R&0x01){};     // Check busy bit
		
		I2C0_MDR_R = Master_Transmit_data[n]&0xFF; 
		I2C0_MCS_R = 0x05;            // Stop and Run
		while(I2C0_MCS_R&0x01){};     // Check busy bit
			
	}
}


void MasterReceiveMuleipleByte(void)
{
	int i = 0;
	for (i = 0; i <= 6; i++)
	{
	while(I2C0_MCS_R&0x01){};       // Check error bit
	I2C0_MSA_R = (0x68<<1)&0xFE;    // Provide slave address
	I2C0_MSA_R &= ~0x01;            // Write Enable
	I2C0_MDR_R = i&0xFF;            // Provide word address
	I2C0_MCS_R = 0x07;              // Stop start and run
	while(I2C0_MCS_R&0x01){};       // Check busy bit
  I2C0_MSA_R = (0x68<<1)&0xFE;	  // Provide Word Address
	I2C0_MSA_R |= 0x01;             // Receive Enable
	I2C0_MCS_R = 0x07;              // Stop start and run
	while(I2C0_MCS_R&0x01){};       // Check busy bit
		
	Master_Receive_data[i] = I2C0_MDR_R&0xFF;     // Get Data
	Master_Receive_data_2[i] = (Master_Receive_data[i] - 6 * (Master_Receive_data[i] >> 4));
		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////___MAIN_FUNCTION___//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
	int datacheck = 0, x=0;
	int Hum_Hi=0, Hum_Lo=0, Temp_Hi=0, Temp_Lo=0, Checksum=0;
	
	
	 KM_PLL_Init();	            // Init OF SYSTEM CLOCK 50HZ
	 KM_SysTick_Init();         // Init SYSTEM TICK TIMER
	 CLOCK_Init();              // Provide clock to corresponding ports and peripherals
	 PORTB_INITIALIZATION();    // Init Port B for I2C
	PORTA_INITIALIZATION();                      // Initializing corresponding pins on Port A
	PORTD_INITIALIZATION();                      // Initializing corresponding pins on Port D
  PORTE_INITIALIZATION();                      // Initializing corresponding pins on Port E

   i2c_port_B_config();
	
   MasterTransmitMuleipleByte();
	LCD_Write_Cmd(0x01);
	
	

	 while(1)
	 {
	 MasterReceiveMuleipleByte();
		 DHT_Init();
		DHT_Request();
		x = DHT11_Response();
    if(x==0)
		{
		Hum_Hi = DHT_Rx_Data();
	  Hum_Lo = DHT_Rx_Data();
	  Temp_Hi = DHT_Rx_Data();
	  Temp_Lo = DHT_Rx_Data();
	  Checksum = DHT_Rx_Data();
		
		//LCD_Write_Cmd(0x80);            // Initialize the cursor on Line 1
		//LCD_Write_Cmd(0xC0);            // Initialize the cursor on Line 2
////////////////////////////////////////////////////////////////////////////////
    LCD_Write_Cmd(0x01);                          // Display Clear		
		LCD_Write_Cmd(0x80);            // Initialize the cursor on Line 1
		print_int(Master_Receive_data_2[2]);	
		printStringLine(":");
		print_int(Master_Receive_data_2[1]);	
		printStringLine("   ");
		print_int(Master_Receive_data_2[4]);	
		printStringLine("/");
		print_int(Master_Receive_data_2[5]);	
		printStringLine("/");
		print_int(Master_Receive_data_2[6]);	
////////////////////////////////////////////////////////////////////////////////			
		//LCD_Write_Cmd(0x01);                          // Display Clear		
		LCD_Write_Cmd(0xC0);            // Initialize the cursor on Line 1
		printStringLine("H:");
		print_int(Hum_Hi);
		printStringLine(".");
		print_int(Hum_Lo);
		
		//LCD_Write_Cmd(0xC0);            // Initialize the cursor on Line 1
		printStringLine("  ");
		printStringLine("T:");
		print_int(Temp_Hi);
		printStringLine(".");
		print_int(Temp_Lo);
		
	  SW_delay();
    }
		
		else 
		{
		LCD_Write_Cmd(0x01);                          // Display Clear		
		LCD_Write_Cmd(0x80);            // Initialize the cursor on Line 1
		
		printStringLine("RequestTimeOut!!");
		}
	 }
	 
	
	 



}
 