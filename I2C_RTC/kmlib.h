
// KM PLL LIB 
//Assumption PLL init to 50MHZ.
void KM_SysTick_Init(void);
void KM_delay_ms(unsigned long delay);
void KM_delay_us(unsigned long delay);

// KM LCD LIB
void KM_LCD_Init(void);
void KM_LCD_Write_Cmd( unsigned char  );
void KM_LCD_Write_Data( unsigned char );
void KM_LCD_Write_Str(char *);
char* KM_itoa(int  );

// KM UART LIB 
// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5

//------------UART_Init------------
// Initialize the UART for 9600 baud rate (assuming 50 MHz clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void KM_UART1_Init(void);

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char KM_UART1_InChar(void);

//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void KM_UART1_OutChar(unsigned char data);

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void KM_UART1_OutString(char *pt);

//------------UART_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
unsigned long KM_UART1_InUDec(void);

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void KM_UART1_OutUDec(unsigned long n);

//---------------------UART_InUHex----------------------------------------
// Accepts ASCII input in unsigned hexadecimal (base 16) format
// Input: none
// Output: 32-bit unsigned number
// No '$' or '0x' need be entered, just the 1 to 8 hex digits
// It will convert lower case a-f to uppercase A-F
//     and converts to a 16 bit unsigned number
//     value range is 0 to FFFFFFFF
// If you enter a number above FFFFFFFF, it will return an incorrect value
// Backspace will remove last digit typed
unsigned long KM_UART1_InUHex(void);

//--------------------------UART_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void KM_UART1_OutUHex(unsigned long number);

//------------UART_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string
// -- Modified by Agustinus Darmawan + Mingjie Qiu --
void KM_UART1_InString(char *bufPt, unsigned short max);

// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

// the PLL to the desired frequency.
#define SYSDIV2 7
// bus frequency is 400MHz/(SYSDIV2+1) = 400MHz/(7+1) = 50 MHz
// bus frequency is 400MHz/(SYSDIV2+1) = 400MHz/(4+1) = 80 MHz

// configure the system to get its clock from the PLL
void KM_PLL_Init(void);


/*
SYSDIV2  Divisor  Clock (MHz)
 0        1       reserved
 1        2       reserved
 2        3       reserved
 3        4       reserved
 4        5       80.000
 5        6       66.667
 6        7       reserved
 7        8       50.000
 8        9       44.444
 9        10      40.000
 10       11      36.364
 11       12      33.333
 12       13      30.769
 13       14      28.571
 14       15      26.667
 15       16      25.000
 16       17      23.529
 17       18      22.222
 18       19      21.053
 19       20      20.000
 20       21      19.048
 21       22      18.182
 22       23      17.391
 23       24      16.667
 24       25      16.000
 25       26      15.385
 26       27      14.815
 27       28      14.286
 28       29      13.793
 29       30      13.333
 30       31      12.903
 31       32      12.500
 32       33      12.121
 33       34      11.765
 34       35      11.429
 35       36      11.111
 36       37      10.811
 37       38      10.526
 38       39      10.256
 39       40      10.000
 40       41      9.756
 41       42      9.524
 42       43      9.302
 43       44      9.091
 44       45      8.889
 45       46      8.696
 46       47      8.511
 47       48      8.333
 48       49      8.163
 49       50      8.000
 50       51      7.843
 51       52      7.692
 52       53      7.547
 53       54      7.407
 54       55      7.273
 55       56      7.143
 56       57      7.018
 57       58      6.897
 58       59      6.780
 59       60      6.667
 60       61      6.557
 61       62      6.452
 62       63      6.349
 63       64      6.250
 64       65      6.154
 65       66      6.061
 66       67      5.970
 67       68      5.882
 68       69      5.797
 69       70      5.714
 70       71      5.634
 71       72      5.556
 72       73      5.479
 73       74      5.405
 74       75      5.333
 75       76      5.263
 76       77      5.195
 77       78      5.128
 78       79      5.063
 79       80      5.000
 80       81      4.938
 81       82      4.878
 82       83      4.819
 83       84      4.762
 84       85      4.706
 85       86      4.651
 86       87      4.598
 87       88      4.545
 88       89      4.494
 89       90      4.444
 90       91      4.396
 91       92      4.348
 92       93      4.301
 93       94      4.255
 94       95      4.211
 95       96      4.167
 96       97      4.124
 97       98      4.082
 98       99      4.040
 99       100     4.000
 100      101     3.960
 101      102     3.922
 102      103     3.883
 103      104     3.846
 104      105     3.810
 105      106     3.774
 106      107     3.738
 107      108     3.704
 108      109     3.670
 109      110     3.636
 110      111     3.604
 111      112     3.571
 112      113     3.540
 113      114     3.509
 114      115     3.478
 115      116     3.448
 116      117     3.419
 117      118     3.390
 118      119     3.361
 119      120     3.333
 120      121     3.306
 121      122     3.279
 122      123     3.252
 123      124     3.226
 124      125     3.200
 125      126     3.175
 126      127     3.150
 127      128     3.125
*/
