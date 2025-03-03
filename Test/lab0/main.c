#include <stdio.h>
#include <unistd.h>

#define LED_PATH "/sys/class/leds/beaglebone:green:usr3/brightness"


int main(int argc, const char *argv[]){

    for (int i = 0; i < 10; i++) {
        FILE *led_handle = fopen(LED_PATH, "w");
        if (led_handle == NULL) {
            perror("Error opening LED file");
            return 1;
        }
    
        if (fwrite("1", sizeof(char), 1, led_handle) != 1) {
            perror("Error writing to LED file");
            fclose(led_handle);
            return 1;
        }
        fclose(led_handle);
        usleep(1000000);
    
        led_handle = fopen(LED_PATH, "w");
        if (led_handle == NULL) {
            perror("Error opening LED file");
            return 1;
        }
    
        if (fwrite("0", sizeof(char), 1, led_handle) != 1) {
            perror("Error writing to LED file");
            fclose(led_handle);
            return 1;
        }
        fclose(led_handle);
        usleep(1000000);
    }
}    