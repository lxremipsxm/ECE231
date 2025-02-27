#include <stdio.h>
#include <unistd.h>
#include <time.h>
#define LED_PATH "/sys/class/leds/beaglebone:green:usr3/brightness"

int main(){
    struct timespec start;
    struct timespec end; 
    long duration;

    
    /*File Setup Section*/

    FILE *led_file = fopen(LED_PATH, "r+");
    FILE *time_diff = fopen("time_diff_file.txt", "a");

    if (led_file == NULL || time_diff == NULL){
        printf("Error opening file");
    }


    /*Main Section - Toggling LED on/off */

    for (int i=0;i<300;i++){

        clock_gettime(CLOCK_MONOTONIC, &start); //store at pointer for start

        usleep(1000000); //Added delay

        char value;
        fread(&value, 1, 1, led_file);
        fseek(led_file, 0, SEEK_SET);

        if (value == '0'){
            fwrite("1", sizeof(char), 1, led_file);
        }
        else {
            fwrite("0", sizeof(char), 1, led_file);
        }
        fseek(led_file, 0, SEEK_SET);

        clock_gettime(CLOCK_MONOTONIC, &end); //store at poitner for end timestamp

        /*Writing to file*/
        duration = end.tv_nsec - start.tv_nsec;
        fprintf(time_diff, "%ld \n", duration);
    }

    /*Closing files*/
    fclose(time_diff);
    fclose(led_file);

    return 0;
}