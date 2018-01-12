//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// .. Leveraged from the TI Blinky example but no external dependencies
//
//*****************************************************************************

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#define SW2 (1<<0)
#define SW1 (1<<4)
#define RED (1<<1)
#define BLUE (1<<2)
#define GREEN (1<<3)

//#define GPIO_PORTF_PUR *((volatile uint_t *)0x40025510)
//
// This is a poor delay method.. Will delay appx 1us,
// Probably inaccurate
//
void
delayUs(uint32_t us)
{
    while(us--)
    {
        __asm("    nop\n" //Delayed to 1Hz 
              "    nop\n" 
              "    nop\n"
	      "    nop\n"
	      "    nop\n"
              "    nop\n"
              "    nop\n"
              "    nop\n"
	      "    nop\n");
    }
}

//
// This is a Very poor delay method.. Will delay appx 1ms.
// Probably inaccurate
//
void
delayMs(uint32_t ms)
{
    while(ms--)
    {
        delayUs(1000); 
    }
}

int
main(void)
{
    volatile uint32_t ui32Loop;

    //
    // Enable the GPIO port that is used for the on-board LED.
    // Pause for a few moments afterwards to let the chip settle..
    //
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
    delayUs(10);

    //
    // setup the main loop delay for 500ms on then 500ms off...
    //
    ui32Loop = 500;

    //
    // Enable the GPIO pin for the LED (PF3).
    // Set the direction as output.
    // Enable the GPIO pin for digital function.
    //
    GPIO_PORTF_DIR_R = RED|BLUE|GREEN;
    GPIO_PORTF_DEN_R = RED|BLUE|GREEN;
   
    //==================================================
    //	Added Code to make Switch Detection
    //==================================================
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R |=SW2;
        

    GPIO_PORTF_DIR_R &= ~SW2; //Switch 2 Enabled as an Input
    GPIO_PORTF_DEN_R |= SW2;

    GPIO_PORTF_DIR_R &= ~SW1; //Switch 1 Enabled as an Input
    GPIO_PORTF_DEN_R |= SW1;

    GPIO_PORTF_PUR_R |= SW2;
    GPIO_PORTF_PUR_R |= SW1;

    GPIO_PORTF_CR_R |=SW2;
    //
    // Loop forever.
    //
    while(1)
    {
        //
        // Turn on the LED.
        //
        
	
        GPIO_PORTF_DATA_R |= GREEN;


        //
        // Delay for a bit.
        //
       delayMs(ui32Loop);
        
	if(!(GPIO_PORTF_DATA_R & SW1))
	{
	    GPIO_PORTF_DATA_R |= RED;
	    
	    for(int i =0;i>20;i++)
	    {
	        GPIO_PORTF_DATA_R |= RED;
                delayMs(ui32Loop);
	        GPIO_PORTF_DATA_R &= ~RED;
	        delayMs(ui32Loop);
            }
	}
	else
	{
	   GPIO_PORTF_DATA_R &= ~RED;
	}
	if(!(GPIO_PORTF_DATA_R & SW2))
	{
	    GPIO_PORTF_DATA_R |= BLUE;
	}
	else
	{
	   GPIO_PORTF_DATA_R &= ~BLUE;
	}
        //
        // Turn off the LED.
        //
       GPIO_PORTF_DATA_R &= ~GREEN;

        //
        // Delay for a bit.
        //
        delayMs(ui32Loop);
    }
}
