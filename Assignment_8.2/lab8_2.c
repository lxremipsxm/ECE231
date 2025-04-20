#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>

#include "i2c.h"
#include "SSD1306.h"
#include "my_uart_lib.h"
#include "my_adc_lib.h"


int main(void){

    OLED_Init();
    unsigned int digiValue;
    DDRC = 0x00; //make port c all input

    uart_init();
    adc_init(); //default channel (PC0)


    while(1){

        char buffer[10];
        digiValue = get_adc();

        send_string("Digitized Value: ");
        send_string(itoa(digiValue, buffer, 10)); //convert to string
        uart_send(13); //carriage return
        uart_send(10); //end line feed

        OLED_GoToLine(1);
        OLED_DisplayString("Temperature");
        
        OLED_GoToLine(5);    
        OLED_DisplayNumber(10, digiValue, 4);

        _delay_ms(1000);
    }
}
