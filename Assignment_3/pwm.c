#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void config_pin(char* pin_num, char* mode){

    if(!vfork()){
        int ret = execl("/usr/bin/config-pin", "config-pin", pin_num, mode, NULL);

        if (ret < 0){
            printf("Failed to configure pin in PWM.\n");
            exit(-1);
        }
    }
}


void config_gpio_input(int gpio, char* pin_num){

    config_pin(pin_num, "gpio_pu");

    char direction[40];
    sprintf(direction, "/sys/class/gpio/gpio%d/direction", gpio);
    
    FILE* fp = fopen(direction, "w");
    fwrite("in", sizeof(char), 2, fp);
    fclose(fp);
}


int main(){

    printf("Test");

    char b1_pin[32] = "P8_08";
    int b1_num = 67;


    char b2_pin[32] = "P8_09";
    int b2_num = 69;

    config_gpio_input(b1_num, b1_pin);
    config_gpio_input(b2_num, b2_pin);

    char valuePath1[40];
    sprintf(valuePath1, "/sys/class/gpio/gpio%d/value", b1_num);

    char valuePath2[40];
    sprintf(valuePath2, "/sys/class/gpio/gpio%d/value", b2_num);


    sleep(1);

    long count = 0;
    int state1;
    int state2;
    FILE *fp1;
    FILE *fp2;
    

    while(count < 10){

        fp1 = fopen(valuePath1, "r");
        fp2 = fopen(valuePath2, "r");

        if (fp1 == NULL || fp2 == NULL) {
            printf("File couldn't be opened");
            exit(-1);
        }
        
        fscanf(fp1, "%d", &state1);
        fscanf(fp2, "%d", &state2);

        fclose(fp1);
        fclose(fp2);

        if(state1 == 0){
            count++;
        }

        printf("State (08): %d | State (09): %d \n", state1, state2);
        sleep(1);
    }

    return 0;
}