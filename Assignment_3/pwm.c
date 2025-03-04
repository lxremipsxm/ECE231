#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


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

void set_dutycycle(char* pwmchip, char* channel, char* d_cycle){

    //export filepath
    char dutycycle[60];
    sprintf(dutycycle, "/sys/class/pwm/%s/pwm-4:%s/duty_cycle", pwmchip, channel);

    //The pwmchip for P9_16 is pwmchip7, channel 1

    FILE* dcfile = fopen(dutycycle, "w");

    fwrite(d_cycle, sizeof(char), strlen(d_cycle), dcfile);
    fclose(dcfile);
}

void set_period(char* pwmchip, char* channel, char* period){

    long d_cycle_int, t_int;

    char d_cycle[60];
    char d_cycle_str[20];

    sprintf(d_cycle, "/sys/class/pwm/%s/pwm-4:%s/duty_cycle", pwmchip, channel);

    FILE* tfile = fopen(d_cycle, "r");
    fscanf(tfile, "%ld", &d_cycle_int);
    fclose(tfile);

    t_int = atol(period);

    if (d_cycle_int>= t_int){
        d_cycle_int = t_int/2;

        sprintf(d_cycle_str, "%ld", d_cycle_int);

        set_dutycycle(pwmchip, channel, d_cycle_str);
    }

    char pwm_period[50];
    sprintf(pwm_period, "/sys/class/pwm/%s/pwm-4:%s/period", pwmchip, channel);
    tfile = fopen(pwm_period, "w");
    fwrite(period, sizeof(char), strlen(period), tfile);
    fclose(tfile);
}


void start_pwm(char* pin_num, char* pwmchip, char* channel, char* period, char* d_cycle){
    config_pin(pin_num, "pwm");

    FILE* buzzfile;

    set_period(pwmchip, channel, period);
    set_dutycycle(pwmchip, channel, d_cycle);

    char enable[40];
    sprintf(enable, "/sys/class/pwm/%s/pwm-4:%s/enable", pwmchip, channel);

    buzzfile = fopen(enable, "w");
    fwrite("1", sizeof(char), 1, buzzfile);
    fclose(buzzfile);
}

void stop_pwm(char* pin_num, char* pwmchip, char* channel){
    char disable[40];
    sprintf(disable, "/sys/class/pwm/%s/pwm-4:%s/enable", pwmchip, channel);

    FILE* buzzfile = fopen(disable, "w");
    fwrite("0", sizeof(char), 1, buzzfile);
    fclose(buzzfile);

}

int main(){  

    char buzz_pin[32] = "P9_16";
    char pwmchip[32] = "pwmchip4";
    char channel[32] = "1";
    char period[32] = "1000000000";
    char d_cycle[32] = "500000000";

    stop_pwm(buzz_pin, pwmchip, channel);
    start_pwm(buzz_pin, pwmchip, channel, period, d_cycle);

    sleep(1);

    stop_pwm(buzz_pin, pwmchip, channel);


    // char b1_pin[32] = "P8_08";
    // int b1_num = 67;


    // char b2_pin[32] = "P8_09";
    // int b2_num = 69;

    // config_gpio_input(b1_num, b1_pin);
    // config_gpio_input(b2_num, b2_pin);

    // char valuePath1[40];
    // sprintf(valuePath1, "/sys/class/gpio/gpio%d/value", b1_num);

    // char valuePath2[40];
    // sprintf(valuePath2, "/sys/class/gpio/gpio%d/value", b2_num);


    // sleep(1);

    // long count = 0;
    // int state1;
    // int state2;
    // FILE *fp1;
    // FILE *fp2;
    

    // while(count < 10){

    //     fp1 = fopen(valuePath1, "r");
    //     fp2 = fopen(valuePath2, "r");

    //     if (fp1 == NULL || fp2 == NULL) {
    //         printf("File couldn't be opened");
    //         exit(-1);
    //     }
        
    //     fscanf(fp1, "%d", &state1);
    //     fscanf(fp2, "%d", &state2);

    //     fclose(fp1);
    //     fclose(fp2);

    //     if(state1 == 0){
    //         count++;
    //     }

    //     printf("State (08): %d | State (09): %d \n", state1, state2);
    //     sleep(1);
    // }

    return 0;
}