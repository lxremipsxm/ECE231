/*Test ultrasound HC-SR04

10us trigger pulse on trig pin, waits for echo pulse on echo pin

Duration of pulse is twice time delay to target. 

FCPU = 16 MHz, 1024 divisor, then the clock is 16MHz/1024 = 15.625 kHz

Each clock pulse is therefore every 64 us.
*/


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "my_uart_lib.h"

#define TRIG PB2 //physical pin d10
#define ECHO PB1 //physical pin d9
#define RANGE_PER_CLOCK 1.098 


void timer0_init(void){
    TCCR0A = 0; //set timer mode to normal
    TCCR0B = 0x05; //1024 prescaler
}

int main(void){
    unsigned int  timeToRisingEdge, timeToFallingEdge;
    unsigned int pulseWidth;
    float range;

    uart_init();
    timer0_init();

    while(1){

        PORTB &= ~(1<<TRIG); //turn off pulse (or pre-pulse)
        _delay_us(5);
        PORTB |= (1<<TRIG);
        _delay_us(10);
        PORTB &= ~(1<<TRIG);

        TCNT0 = 0; //initialize timer

        while((PINB&(1<<ECHO))==0); //wait until the Echo pin receives a return pulse
        timeToRisingEdge = TCNT0;
        
        while(!(PINB&(1<<ECHO))==0); //wait till Echo pin goes LOW
        timeToFallingEdge = TCNT0;
        pulseWidth = timeToFallingEdge - timeToRisingEdge;

        range = pulseWidth*1.098; //distance to target


        char buf[20];
        char r[10];
        dtostrf(range,4,1,r);
        sprintf(buf, "Distance: %s", r);
        send_string(buf);
        send_string("\r\n");

        _delay_ms(100);

    }
}