//Libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

//User Defined Libraries
#include "my_adc_lib.h"
#include "my_uart_lib.h"


int main(void){
    unsigned int digiValue;
    DDRC = 0x00; //make port c all input

    uart_init();
    adc_init(); //default channel (PC0)

    while (1){
        char buffer[10];
        digiValue = get_adc();

        send_string("Digitized Value: ");
        send_string(itoa(digiValue, buffer, 10)); //convert to string
        uart_send(13); //carriage return
        uart_send(10); //end line feed
        _delay_ms(1000);
        
    }

}