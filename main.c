#include <msp430g2553.h>
#include <stdio.h>


#define LED_0 BIT0
#define LED_1 BIT6
#define LED_OUT P1OUT
#define LED_DIR P1DIR
#define BUTTON BIT3

#define TXD BIT2 // transmit pin
#define RXD BIT1 // receive pin

unsigned int blink = 0;
int miliseconds=0;
int z;
int t=0;
long jump;
char g;
int i;
char tmpc[11]={'p','a','s','o','k','i','t','e'}; // please jump
char tmp2[11]={'s','u','o','l','i','s',':'};     // jump height




void main(void)
{

    BCSCTL1 = CALBC1_1MHZ;
      DCOCTL = CALDCO_1MHZ;                     // submainclock 1mhz
      CCTL0 = CCIE;                             // CCR0 interrupt enabled

        CCR0 = 1000;                  // 1ms at 1mhz
        TACTL = TASSEL_2 + MC_1;                  // SMCLK, upmode

WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
LED_DIR |= (LED_0 + LED_1); // Set P1.0 and P1.6 to output direction
LED_OUT &= ~(LED_0 + LED_1); // Set the LEDs off
P1REN |= BUTTON; //Enables a puller-Resistor on the button-pin
P1OUT |= BUTTON; //Writes a "1" to the portpin, tellling the resistor to pullup
P1IES |= BUTTON; //Triggers when you PRESS the button :: Pick one...
P1IE |= BUTTON; //Enables the selector-mask for generating interrupts on the relevant pin



  P1SEL |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD

  IE2 |= UCA0TXIE; // Enable the Transmit interrupt
  IE2 |= UCA0RXIE; // Enable the Receive interrupt



__enable_interrupt(); // Interrupts get enabled 

for (;;)
{


}

}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    blink ^= BIT6;
    P1OUT ^= LED_0 ; // P1.0 = toggle
    P1IES ^= BUTTON; // toggle the interrupt edge,

    {

        if (t>0)
        {
            jump=(9.81*t*t)/8000;
            UC0IE |= UCA0TXIE;
            g=2;
        }

    }
    miliseconds=0;

    if (blink>0)
    {
        t=0;

    }


}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    if (P1IFG &= ~BUTTON)
    {
        miliseconds++;
    }
    z=miliseconds;
    //miliseconds=0;


    if (z>135)
      {
        g=1;
        UC0IE |= UCA0TXIE;
        P1OUT ^= LED_1 ; // P1.0 = toggle
          //t=miliseconds++;
           t++;

      }


  }


//---------------------UART----------------------------

// UART duomenu siuntimas
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    if (g==1)
    {
        UCA0TXBUF= tmpc[i++]; // sending please jump text
        if (i>sizeof tmpc-1)
        {
        UC0IE &= ~UCA0TXIE; // disabling interrupt
        i=0;
        }
    }


    if (g==2)
        {

               //jump=t;
                  tmp2[7]=jump/100;
                  tmp2[8]=(jump/10)%10;
                  tmp2[9]='.';
                  tmp2[10]=(jump%100)%10;


            tmp2[7]=tmp2[7]+0x30;
            tmp2[8]=tmp2[8]+0x30;
            tmp2[10]=tmp2[10]+0x30;
            UCA0TXBUF= tmp2[i++]; // jump height 
            if (i>sizeof tmp2-1)
            {
            UC0IE &= ~UCA0TXIE; // disabled interrupt
            i=0;
            }
        }
}
