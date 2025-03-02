#include <stdio.h>
#include <unistd.h>


void config_pin(char* pin_num, char* mode){

    vfork();
    int ret = execl("/usr/bin/config_pin", "config_pin", pin_num, mode, NULL);

    if (ret < 0){
        printf("Failed to configure pin in PWM.\n");
        exit(-1);
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

    char gpio_pin[32] = "P8_08";
    int gpio_num = 67;

    config_gpio_input(gpio_num, gpio_pin);

    char valuePath[40];
    sprintf(valuePath, "/sys/class/gpio/gpio%d/value", gpio_num);

    sleep(1);

    long count = 0;
    int state;
    FILE *fp;
    

    while(1){
        fp = fopen(valuePath, "r");
        

        fscanf(fp, "%d", &state);
        fclose(fp);

        if(state==0){
            count++;
            printf("State: %ld \n", count);
        }
    }


    return 0;
}