#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "i2c.h"
#include "SSD1306.h"


int main(void){

    char s[10] = "8=====D";
    OLED_Init();


    while(1){
        OLED_GoToLine(4);
        OLED_DisplayString(s);
        _delay_ms(1000);
    }
}
