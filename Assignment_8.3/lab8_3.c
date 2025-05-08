/* seven_counter_main.c  This code demonstrates the use of a 4 digit 
7 segment LED. This version counts 0-1000 repeatedly and uses retinal
persistence to create an always-on effect in the digits
D. McLaughlin 3/16/22 ECE-231 Demo */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h> 

#define PERSISTENCE 5
#define TRIG PC1 
#define ECHO PC0    
#define RANGE_PER_CLOCK 1.098



unsigned char ledDigits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

void timer0_init(){
    TCCR0A = 0;         // Timer1 normal mode
    TCCR0B = 5;
    TCNT0=0;            // Start timer at 0

}

int main(void){
    unsigned char rising_edge_clocks, falling_edge_clocks, echo_width_clocks;
    int value;
    unsigned char DIG1, DIG2, DIG3, DIG4;

    DDRD = 0xFF;   
    DDRB = 0xFF;    
    PORTB = 0xFF;   

    DDRC = 1<<TRIG;         
    PORTC &= ~(1<<TRIG);    
    timer0_init();          

    while(1){
        TCNT0 = 0;          // Load counter with 0
        PORTC |= 1<<TRIG; 
        _delay_us(10);     
        PORTC &= ~(1<<TRIG);

        // Wait till the ECHO goes high
        while ((PINC & (1<<ECHO)) ==0);
        rising_edge_clocks = TCNT0; //timestamp

        // wait till the ECHO pulse goes low
        while (!(PINC & (1<<ECHO))==0);
        falling_edge_clocks = TCNT0;
        
        if (falling_edge_clocks > rising_edge_clocks){
            // Compute target range, display on leds
            echo_width_clocks = falling_edge_clocks - rising_edge_clocks;
            value = echo_width_clocks * RANGE_PER_CLOCK;
        }

        DIG4 = value%10;                // 1st digit
        DIG3= (value/10)%10;            // 2nd digit
        DIG2 = (value/100)%10;          // 3rd digit
        DIG1 = (value/1000);            // 4th digit

        PORTD = ledDigits[DIG4];
        PORTB = ~(1<<1);           
        _delay_ms(PERSISTENCE);     
    
        PORTD = ledDigits[DIG3];   
        PORTB = ~ (1<<2);           
        _delay_ms(PERSISTENCE);
        
        
        PORTD = ledDigits[DIG2];
        PORTB = ~ (1<<3);           
        _delay_ms(PERSISTENCE);
        
        
        PORTD = ledDigits[DIG1];
        PORTB = ~ (1<<4);       
        _delay_ms(PERSISTENCE);
       
    }
}
