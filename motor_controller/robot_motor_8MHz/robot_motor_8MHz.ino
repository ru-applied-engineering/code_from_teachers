/*
 * keyra_motora.ino
 *
 * Created: 27-Apr-21
 * Author : Kristófer Ingi Maack
 */ 

#include <avr/io.h>
#include <string.h>
#include "uart.h"
#include <stdint.h>

#define F_CPU 8000000UL // Setting CPU frequency to 8MHz

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


#define PWM_MAX_VALUE 255
#define SER_BUFFER_SIZE 30
#define CMD_BUFFER_SIZE 30

volatile uint8_t input = 0;

char ser_data[SER_BUFFER_SIZE] = "";
volatile uint8_t ser_buff_index = 0;
const char cmd_start[5] = "((";
const char cmd_stop[5] = "))";
const char cmd_reset[5] = "//";
const uint8_t cmd_marker_length = 2;
char cmd_buffer[CMD_BUFFER_SIZE] = "";

volatile int ser_rx_cmd = 0; // in process of receiving new cmd: 1, else 0
volatile uint8_t ser_new_cmd = 0; // New data : 1



volatile uint8_t L_M_Duty = 0;
volatile uint8_t R_M_Duty = 0;
volatile uint8_t L_M_Dir = 0;
volatile uint8_t R_M_Dir = 0;



char ser_cmd[20];



volatile uint8_t ser_cmd_count = 0;
volatile int ser_data_count = 0;

volatile int ser_rx_data = 0; // Re


volatile uint8_t duty = 0;
volatile uint8_t print_ser = 0;

volatile char ser_motor_L_R_char = 'M';
volatile char ser_motor_direction_char = 0;
volatile uint8_t ser_motor_duty = 0;








// Interrupt on RX
ISR (USART_RX_vect)
{
  //if(ser_new_cmd == 1) return;
  
  print_ser = 1;
	//input = UDR0;

  ser_data[ser_buff_index] = UDR0;
  ser_buff_index += 1;
  ser_data[ser_buff_index] = '\n'; //Append NL.
  ser_data[ser_buff_index+1] = '\0'; //Append null char.
  //Check if serial buff index i out of range, incl. null char.
  if(ser_buff_index > (SER_BUFFER_SIZE-3)) {
     ser_buff_index = 0;
  }
  

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
 
  //Setup PWM module as 8-bit Fast PWM
  //Peripheral clock CLK/8, 8-bit
  ICR1 = 0xFF; //Top value for 16-bit counter.
  TCCR0A |= (1 << WGM00) | (1 << WGM01);
  TCCR1A |= (1 << WGM10);
  TCCR0B |= (1 << CS01);
  TCCR1B |= (1 << WGM12) | (1 << CS11);
  
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


uint8_t perc_to_duty(uint8_t duty_perc_s){
  double duty_perc_dob = (double)duty_perc_s;
  
  if(duty_perc_dob > 100) {
    duty_perc_dob = 100;
  }
  if(duty_perc_dob < 0) {
    duty_perc_dob = 0;
  }
  uint16_t duty_tmp_16 = (uint16_t)((duty_perc_dob/100.0)*PWM_MAX_VALUE);
  
  if(duty_tmp_16 > PWM_MAX_VALUE) {
    duty_tmp_16 = PWM_MAX_VALUE;
  }
  return (uint8_t)duty_tmp_16;
}



void SET_L_dir_duty(uint8_t dir, uint8_t duty_perc_s){  //Forward: 1; Reverse: 0
  uint8_t duty_tmp = perc_to_duty(duty_perc_s);
  if(dir == 1){
    //Forward
    TCCR0A &= ~((1 << COM0B1) | (1 << COM0B0));
    TCCR0A |= ((1 << COM0A1));
    OCR0A = duty_tmp;
    PWM1_LOW;
    
  }
  if(dir == 0){   
    //Reverse 
    TCCR0A |= ((1 << COM0B1));
    TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0));
    OCR0B = duty_tmp;
    AENBL_LOW; 
    
  }
}


void SET_R_dir_duty(uint8_t dir, uint8_t duty_perc_s){  //Forward: 1; Reverse: 0
  uint8_t duty_tmp = perc_to_duty(duty_perc_s);
  if(dir == 1){
    //Forward
    TCCR1A |= ((1 << COM1A1));
    TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
    OCR1A = duty_tmp;
    BENBL_LOW;

  }
  if(dir == 0){
    //Reverse
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
    TCCR1A |= ((1 << COM1B1));
    OCR1B = duty_tmp;
    PWM2_LOW;
  
  }
}

void STOP_motors(){
  //Stop both motors
  TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
  TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
  TCCR0A &= ~((1 << COM0B1) | (1 << COM0B0));
  TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0));
  PWM1_LOW;
  AENBL_LOW;
  PWM2_LOW;
  BENBL_LOW;
}

//Checks for new cmd and returns ptr to char str with cmd.
//Call free(str) when cmd has been used.
char* check_for_new_cmd(char *data_in){
  
  //Reset cmd
  if(strstr(data_in, cmd_reset)){
    LED_LOW;
    memset(data_in, 0, SER_BUFFER_SIZE-1); //Delete all data in cmd buffer
    ser_rx_cmd = 0;
    ser_buff_index = 0;
    ser_data[0] = 0;
    return 0;
  }
  
  //Check if new command is arriving
  if(!ser_rx_cmd && (strstr(data_in, cmd_start))){

    ser_rx_cmd = 1;
    return 0;
  }

  //Check if cmd has arrived
  if(ser_rx_cmd && (strstr(data_in, cmd_stop))){

    char* tmp_cmd_begins = strstr(data_in, cmd_start) + cmd_marker_length; //Cmd starts after second start char.
    
    char* tmp_cmd_ends = strstr(data_in, cmd_stop); //

    int tmp_cmd_size = tmp_cmd_ends - tmp_cmd_begins;

    char* tmp_cmd = (char*) malloc(tmp_cmd_size + 2);

    strncpy(tmp_cmd, tmp_cmd_begins, tmp_cmd_size);
    tmp_cmd[tmp_cmd_size] = '\0'; //Append null char
 
    memset(data_in, 0, SER_BUFFER_SIZE-1); //Delete all data in cmd buffer
    
    ser_new_cmd = 1;
    ser_rx_cmd = 0;
    ser_buff_index = 0;
    ser_data[0] = 0;

    return tmp_cmd;  //return the cmd.
  }
  
  return 0;
  
}



int main()
{
	init_motor_driver();
	//init_analog();
  memset(ser_data, 0, SER_BUFFER_SIZE-1); //Delete all data in cmd buffer
  

  
  ser_data[0] = 0;
  char* cmd_buffer_tmp;
  uint8_t m_direction = 255;
  char* ptr;
  while(1) {

    if(cmd_buffer_tmp = check_for_new_cmd(ser_data)){
      
      ser_motor_L_R_char = cmd_buffer_tmp[0];
      ser_motor_direction_char = cmd_buffer_tmp[1];
      ser_motor_duty = (uint8_t)strtol(&cmd_buffer_tmp[2], &ptr, 10);
      //UART_Transmit_String(cmd_buffer_tmp);
      free(cmd_buffer_tmp); //Free the allocated memory
      
      
      if(ser_motor_direction_char == 'F') {
        m_direction = 1;
      }
      else if(ser_motor_direction_char == 'R') {
        m_direction = 0;
      }
       else {
        STOP_motors();
      }
      switch (ser_motor_L_R_char) {
        case 'A':
          
          SET_L_dir_duty(m_direction, ser_motor_duty);
          SET_R_dir_duty(m_direction, ser_motor_duty);
          
          break;
        case 'R':
          SET_R_dir_duty(m_direction, ser_motor_duty);
          break;
        case 'L':
          SET_L_dir_duty(m_direction, ser_motor_duty);
          break;
        default:
          STOP_motors();
      }
      
      ser_new_cmd = 0; // Done processing data;
    }
 
    
    
  }
	
	return 0; // Main has to return something but it isin't reached
}
