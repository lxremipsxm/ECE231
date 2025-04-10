/* 
Siddharth Vasudevan, ECE231 SP25 Lab 8.1

Connections:
PB5 - Physical Pin D13: Green LED Anode
PB4 - Physical Pin D12: Red LED Anode
PC0 - Physical Pin A0: Left Button (For red)
PC1 - Physical Pin A1: Right Button (For green)

5Hz rate should flash once every 0.2s
*/

#include <avr/io.h>
#include <util/delay.h>


int main(void){

    DDRC&= ~(1 <<PC0);
    DDRC&= ~(1 <<PC1);

    DDRB = 0xFF;

    while(1){

        while (!(PINC&(1<<PC0))){
            
            PORTB |= (1<<PB4);
            _delay_ms(200);
            PORTB |= ~(1<<PB4);
            _delay_ms(200);

        }

        while (!(PINC&(1<<PC1))){

            PORTB |= (1<<PB5);
            _delay_ms(200);
            PORTB |= ~(1<<PB5);
            _delay_ms(200);

        }
    }
}
