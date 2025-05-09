/* 
Siddharth Vasudevan, ECE231 SP25 Lab 8.2
lab8_2.c
4/22/2025

Description: ATmega takes input from a TMP36 sensor, reads it using an ADC, performs conversions with a button,
then displays the information both through the serial port and an OLED. A "too hot" LED indicates if 
the temperature exceeds a preset maximum value.

Connections:
PB3 - Physical Pin D11 - LED
SCL, SDA - OLED
PC0 - Physical Pin A0 - ADC/TMP36
PC1 - Physical Pin A1 - Button

*/


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
    DDRB = 0xFF; //for led

    uart_init();
    adc_init(); //default channel (PC0)
    float too_hot = 80; //"too hot" indicator in F

    while(1){

        digiValue = get_adc();

        float voltage_reading = (digiValue/1023.0) * 5.0; //conversion to voltage from analog
        
        float temp_c = (voltage_reading-0.75)*100+25; //scaling to convert to celsius, 10mV/C, 750mV offset


        float temp_f = (temp_c * 9.0/5.0) + 32.0;

        OLED_GoToLine(1);
        OLED_DisplayString("Temperature");

        uart_send(13); //carriage return
        uart_send(10); //end line feed

        char output[32];
        char temp_str[10];

        if (PINC & (1<<PC1)) {
            
            dtostrf(temp_f, 4,1, temp_str);
            sprintf(output, "Temperature: %s F", temp_str);
            
            OLED_GoToLine(5);    
            OLED_DisplayString(temp_str);
            OLED_DisplayString(" F");
        }

        else {
            
            dtostrf(temp_c, 4,1, temp_str);
            sprintf(output, "Temperature: %s C", temp_str);
            
            OLED_GoToLine(5);    
            OLED_DisplayString(temp_str);
            OLED_DisplayString(" C");

        }

        if (temp_f >= too_hot){

            PORTB |= (1<<PB3);
        }
        else {
            PORTB &= ~(1<<PB3); 
        }

        send_string(output);

        _delay_ms(300);
        
    }
}
