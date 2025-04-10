/* 
Siddharth Vasudevan, ECE231 SP25 Lab 8.1
lab8_1.c
4/10/2025

Description: This code enables the ATmega to take input from three buttons and flash two leds
in different permuations, while preventing simultaneous button presses.

Connections:
PB5 - Physical Pin D11: Green LED Anode
PB4 - Physical Pin D12: Red LED Anode
PC0 - Physical Pin A0: Leftmost Button (For red)
PC1 - Physical Pin A1: Middle Button (For green)
PC2 - Physical Pin A2: Rightmost Button (for simultaneous activation)

5Hz rate should flash once every 0.2s (200ms)
*/

#include <avr/io.h>
#include <util/delay.h>


int main(void){

    //Set port C pins c0, c1, and c2 as input at once
    DDRC&= ~(1 <<0b00000111); 

    //Enable pull-up resistors on butons
    PORTC |= (1 << PC0);  
    PORTC |= (1 << PC1);
    PORTC |= (1 << PC2);  

    //outputs on port b
    DDRB = 0xFF;

    while(1){

        while (!(PINC & (1<<PC0))){ //while pc0 is high
            
            PORTB |= (1<<PB4); //bitwise or on bit 4
            _delay_ms(200);
            PORTB &= ~(1<<PB4); //bitwise and on bit 4
            _delay_ms(200);

            while (!(PINC & (1<<PC1)) || !(PINC & (1<<PC2))){
                PORTB &= ~(1<<PB4);
            }


        }

        while (!(PINC & (1<<PC1))){ //while pc1 is high

            PORTB |= (1<<PB3); //bitwise or on bit 3
            _delay_ms(200);
            PORTB &= ~(1<<PB3); //bitwise and on bit 3
            _delay_ms(200);

            while (!(PINC & (1<<PC0)) || !(PINC & (1<<PC2))){
                PORTB &= ~(1<<PB3);
            }

        }

        while (!(PINC & (1<<PC2))){ //while pc2 is high

            PORTB |= (1<<PB3); 
            PORTB |= (1<<PB4);
            _delay_ms(200);
            PORTB &= ~(1<<PB3);
            PORTB &= ~(1<<PB4);
            _delay_ms(200);

            while (!(PINC & (1<<PC0)) || !(PINC & (1<<PC1))){
                PORTB &= ~(1<<PB3);
                PORTB &= ~(1<<PB4);
            }

        }

    }

    return 0;
}
