/* 
Siddharth Vasudevan, ECE231 SP25 Lab 8.1

Connections:
PB5 - Physical Pin D11: Green LED Anode
PB4 - Physical Pin D12: Red LED Anode
PC0 - Physical Pin A0: Left Button (For red)
PC1 - Physical Pin A1: Right Button (For green)

5Hz rate should flash once every 0.2s (200ms)
*/

#include <avr/io.h>
#include <util/delay.h>


int main(void){

    //Set port C pins c0 and c1 to input
    DDRC&= ~(1 <<PC0);
    DDRC&= ~(1 <<PC1);

    //Enable pull-up resistors on butons
    PORTC |= (1 << PC0);  
    PORTC |= (1 << PC1);  

    //outputs on port b
    DDRB = 0xFF;

    while(1){

        while (!(PINC & (1<<PC0))){ //while pc0 is high
            
            PORTB |= (1<<PB4); //bitwise or on bit 4
            _delay_ms(200);
            PORTB &= ~(1<<PB4); //bitwise and on bit 4
            _delay_ms(200);

        }

        while (!(PINC & (1<<PC1))){ //while pc1 is high

            PORTB |= (1<<PB3); //bitwise or on bit 5
            _delay_ms(200);
            PORTB &= ~(1<<PB3); //bitwise and on bit 5
            _delay_ms(200);

        }
    }

    return 0;
}
