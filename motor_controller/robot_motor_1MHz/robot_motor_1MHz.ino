/*
 * keyra_motora.ino
 *
 * Created: 27-Apr-21
 * Author : Kristófer Ingi Maack
 */ 

#include <avr/io.h>
#include <string.h>
#include "uart.h"

#define F_CPU 1000000UL // Setting CPU frequency to 1MHz

/******************************************** PLS READ THIS ********************************************
* Below is a bit of defines for the pins, don't let them scare you, they work like in Arduino code! 
* The name of the command is right behind the #define syntax.
* To set the pin as output or input use [NAME]_SET. I have designed it so all pins that are supposed to be input or output are set so accordingly.
* To set outpu as high or low use [NAME]_HIGH or [NAME]_LOW
* To read input use [NAME]_READ
*/

/*		DEFINGNG NAMES TO PINS		*/
#define LED_PIN PINB6

#define MODE_PIN PINB7

#define PWM1_PIN PIND5
#define AENBL_PIN PIND6

#define PWM2_PIN PINB1
#define BENBL_PIN PINB2

#define A1_PIN PIND3
#define B1_PIN PIND7

#define A2_PIN PIND2
#define B2_PIN PINB0

#define analog1_PIN PINC0
#define analog2_PIN PINC1
#define analog3_PIN PINC2
#define analog4_PIN PINC3




/*		DEFINING PINS		*/
#define LED_SET DDRB |= 1 << LED_PIN      // setting LED pin as output

#define MODE_SET DDRB |= 1 << MODE_PIN    // setting Mode pin as output

#define PWM1_SET DDRD |= 1 << PWM1_PIN    // setting PWM1 pin (AIN1) as output
#define AENBL_SET DDRD |= 1 << AENBL_PIN  // setting AENBL pin (AIN2) as output

#define PWM2_SET DDRB |= 1 << PWM2_PIN    // setting PWM2 pin (BIN1) as output
#define BENBL_SET DDRB |= 1 << BENBL_PIN  // setting BENBL pin (BIN2) as output


#define A1_SET DDRD &= ~(1 << A1_PIN) // setting A Signal Encoder1 pin as input
#define B1_SET DDRD &= ~(1 << B1_PIN) // setting B Signal Encoder1 pin as input

#define A2_SET DDRD &= ~(1 << A2_PIN) // setting A Signal Encoder2 pin as input
#define B2_SET DDRB &= ~(1 << B2_PIN) // setting B Signal Encoder2 pin as input


#define analog1_SET DDRC &= ~(1 << analog1_PIN) // setting Analog1 pin as input
#define analog2_SET DDRC &= ~(1 << analog2_PIN) // setting Analog2 pin as input
#define analog3_SET DDRC &= ~(1 << analog3_PIN) // setting Analog3 pin as input
#define analog4_SET DDRC &= ~(1 << analog4_PIN) // setting Analog4 pin as input


/*		SETTING PIN OUTPUTS		*/
#define LED_HIGH PORTB |= 1 << LED_PIN  // Making Mode High
#define LED_LOW PORTB &= ~(1<< LED_PIN) // Making Mode Low

#define MODE_HIGH PORTB |= 1 << MODE_PIN  // Making Mode High
#define MODE_LOW PORTB &= ~(1<< MODE_PIN) // Making Mode Low

#define PWM1_HIGH PORTD |= 1 << PWM1_PIN   // Making PWM1 High
#define PWM1_LOW PORTD &= ~(1<< PWM1_PIN) // Making PWM1 Low

#define AENBL_HIGH PORTD |= 1 << AENBL_PIN  // Making AENBL High
#define AENBL_LOW PORTD &= ~(1<< AENBL_PIN) // Making AENBL Low

#define PWM2_HIGH PORTB |= 1 << PWM2_PIN  // Making PWM2 High
#define PWM2_LOW PORTB &= ~(1<< PWM2_PIN) // Making OWN2 Low

#define BENBL_HIGH PORTB |= 1 << BENBL_PIN  // Making BENBL High
#define BENBL_LOW PORTB &= ~(1<< BENBL_PIN) // Making BENBL Low

/*		READING INPUTS		*/
#define A1_READ (PIND & (1 << A1_PIN)) >> A1_PIN // Reading input from A1 signal pin
#define B1_READ (PIND & (1 << B1_PIN)) >> B1_PIN // Reading input from A1 signal pin
#define A2_READ (PIND & (1 << A2_PIN)) >> A2_PIN // Reading input from A1 signal pin
#define B2_READ (PINB & (1 << B2_PIN)) >> B2_PIN // Reading input from A1 signal pin



int input = -1;

// Interrupt on RX
ISR (USART_RX_vect)
{
  LED_HIGH;
	input = UDR0;
}


//Initalizing motors
void init_motor() 
{
	MODE_SET;
	PWM1_SET;
	AENBL_SET;
	PWM2_SET;
	BENBL_SET;
	A1_SET;
	B1_SET;
	A2_SET;
	B2_SET;
	
	MODE_LOW;
}

//Initalizing analog
void init_analog() {
	analog1_SET;
	analog2_SET;
	analog3_SET;
	analog4_SET;
}

//Initalizing motor driver
void init_motor_driver() {
	init_motor(); // Pins for motor driver initialized
	
	init_Uart(); // Initializing UART communication
	
	LED_SET; // Setting the LED pin as output
	
	sei(); // Called last in init, initalizing interrupt
}


int main()
{
	init_motor_driver();
	//init_analog();

  while(1) {
   
    LED_LOW; // Turning LED off so it's only on when it's recieving data

    /******************************************** PLS READ THIS ********************************************
    * Forward and backward might be different depending on the wiring of the motor or orientation of it, 
      make your own guide rules (Good habit to put these kind of things in notebook).
    *
    * Input = 1 => Motor 1 stops
    * Input = 2 => Motor 1 drives forward
    * Input = 3 => Motor 1 drives backwards
    * Input = 4 => Motor 2 stops
    * Input = 5 => Motor 2 drives forward
    * Input = 6 => Motor 2 drives backwards
    */
    if(input == 1)
    {
      PWM1_LOW;
      AENBL_LOW;
    }
    else if(input == 2)
    {
      PWM1_LOW;
      AENBL_HIGH;
    }
    else if(input == 3)
    {
      PWM1_HIGH;
      AENBL_LOW;
    }
    else if(input == 4)
    {
      PWM2_LOW;
      BENBL_LOW;
    }
    else if(input == 5) 
    {
      PWM2_LOW;
      BENBL_HIGH;
    }
    else if(input == 6)
    {
      PWM2_HIGH;
      BENBL_LOW;
    }

    /*
    // Small delay
    for(int i = 0; i<1000;i++)
    {
      asm("nop");
    }
    */

    
  }
	
	return 0; // Main has to return something but it isin't reached
}
