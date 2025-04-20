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
    PORTC |= (1<<PC1); //enable pull up on button pin PC1

    uart_init();
    adc_init(); //default channel (PC0)
    int c_flag = 1; //celsius flag (supposed to represent bool)

    while(1){

        //toggle celsius flag
        if (c_flag == 1 && !(PINC & (1<<PC1))){
            c_flag = 0; 
        }
        
        else if (c_flag == 0 && !(PINC & (1<<PC1))){
            c_flag = 1;
        }

        digiValue = get_adc();

        
        float temp_c = (((digiValue/1023.0) * 5.0)-0.5)*100 ; //scaling to convert to celsius, 10mV/C
        float temp_f = (temp_c * 9/5) + 32;

        OLED_GoToLine(1);
        OLED_DisplayString("Temperature");

        uart_send(13); //carriage return
        uart_send(10); //end line feed

        char output[32];
        char temp_str[10];

        if (c_flag == 1) {
            
            dtostrf(temp_c, 4,1, temp_str);
            sprintf(output, "Temperature: %s C", temp_str);
            
            OLED_GoToLine(5);    
            OLED_DisplayString(temp_str);
            OLED_DisplayString(" C");

        }

        else {
            
            dtostrf(temp_f, 4,1, temp_str);
            sprintf(output, "Temperature: %s F", temp_str);
            
            OLED_GoToLine(5);    
            OLED_DisplayString(temp_str);
            OLED_DisplayString(" F");

        }

        send_string(output);

        _delay_ms(300);
        
    }
}
