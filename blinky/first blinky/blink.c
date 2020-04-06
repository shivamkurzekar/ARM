
// 0x400FE000 base address of system control and 608 offset address of RCGCGPIO and offset of PRGPIO is A08
#define SYSCTL_RCGCGPIO  *((int *)(0x400FE608)) 
#define SYSCTL_PRGPIO    *((int *)(0x400FEA08))  
#define GPIOPORTF_DIR    *((int *)(0x40025400))	
#define GPIOPORTF_AFSEL  *((int *)(0x40025420))
#define GPIOPORTF_DEN    *((int *)(0x4002551C))
#define GPIOPORTF_AMSEL  *((int *)(0x40025528))
#define GPIOPORTF_DATA   *((int *)(0x400253FC))

void PORTF_INITIALIZATION(VOID)
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
void delay(void)
{
	int i=320000;
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
		GPIOPORTF_DATA ^= 0x02; //toggles RED LED
		delay();
	}
}
