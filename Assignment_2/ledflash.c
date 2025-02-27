#include <stdio.h>
#include <unistd.h>
#include <time.h>
#define LED_PATH "/sys/class/leds/beaglebone:green:usr3/brightness"

int main(){
    FILE *led_file = fopen(LED_PATH, 'r+');

    if (led_file == NULL){
        printf("Error opening file");
    }

    for (int i=0;i<10;i++){

        usleep(1000000); //Added delay

        char value;
        fread(&value, 1, 1, led_file);
        
        if (value == '0'){
            fwrite('1', sizeof(char), 1, led_file);
        }
        else{
            fwrite('0', sizeof(char), 1, led_file);
        }
    }


    return 0;
}