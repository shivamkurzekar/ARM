//pf1 red led
// pf2 blue led
// pf0, pf4 are switch
// 0x400FE000 base address of system control and 608 offset address of RCGCGPIO and offset of PRGPIO is A08
#define SYSCTL_RCGCGPIO  *((int *)(0x400FE608)) 
#define SYSCTL_PRGPIO    *((int *)(0x400FEA08))  
#define GPIOPORTA_DIR    *((int *)(0x40004400))	
#define GPIOPORTA_AFSEL  *((int *)(0x40004420))
#define GPIOPORTA_DEN    *((int *)(0x4000451C))
#define GPIOPORTA_AMSEL  *((int *)(0x40004528))
#define GPIOPORTA_DATA   *((int *)(0x400043FC))

void PORTF_INITIALIZATION(void)
{
	SYSCTL_RCGCGPIO |= (0x01);  // Enable GPIO PORT A CLOCK
	while( !(SYSCTL_PRGPIO & 0x01))
	{
		;
	}		// Wait untill PORT F peripheral is READY
	
	GPIOPORTA_DIR |= 0x04;     //OUTPUT DIRECTION;
	GPIOPORTA_AFSEL &= ~(0x04);  //initialization
	GPIOPORTA_DEN |=(0x04);  //initialization
	GPIOPORTA_AMSEL &= ~(0x04);  //initialization
	
}
void delay(void)
{
	int i=1200000;
	while(i>0)
	{
		i--;
	}
}

int main()
{
	//pg no 662 APB address of port F
	// 0x400FE000 base address of system control and 608 offset address of RCGCGPIO and offset of PRGPIO is A08
	// type casting
	//SYSCTL_RCGCGPIO = SYSCTL_RCGCGPIO | (0x1 << 5);
	PORTF_INITIALIZATION();
	/*
	SYSCTL_RCGCGPIO |= (0x20);  // Enable GPIO PORT F CLOCK
	while( !(SYSCTL_RPGPIO & 0x20))
	{
		;
	}		// Wait untill PORT F peripheral is READY
	
	GPIOPORTF_DIR |= 0x02;     //OUTPUT DIRECTION;
	GPIOPORTF_AFSEL &= ~(0x02);  //initialization
	GPIOPORTF_DEN |=(0x02);  //initialization
	GPIOPORTF_AMSEL &= ~(0x02);  //initialization
	*/
	
	while(1)  
	{
		GPIOPORTA_DATA ^= 0x04; //toggles RED LED
		delay();
	}
}
