#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/epoll.h>



void config_pin(char *pin_num, char *mode){

    if(!vfork()){
        int ret = execl("/usr/bin/config-pin", "config-pin", pin_num, mode, NULL); //child process

        if (ret < 0){
            printf("Failed to configure pin in PWM.\n");
            exit(-1);
        }
    }
}

void config_gpio_input(int gpio, char *pin_num){
    //configures gpio as an input pin
    config_pin(pin_num, "gpio_pu");

    char direction[40];
    sprintf(direction, "/sys/class/gpio/gpio%d/direction", gpio);
    
    FILE *fp = fopen(direction, "w");
    fwrite("in", sizeof(char), 2, fp);
    fclose(fp);
}

void config_interrupt(int gpio_num, char *pin_num){
    //configures pin with interrupt capabilities on the rising edge of the clock
    config_gpio_input(gpio_num, pin_num);

    char InterruptEdge[40];
    sprintf(InterruptEdge, "/sys/class/gpio/gpio%d/edge", gpio_num);

    FILE *edgefile = fopen(InterruptEdge, "w");

    fwrite("rising", sizeof(char), 7, edgefile);

    fclose(edgefile);

}

void capture_interrupt(int gpio_num, long timestamps[]) {

    /*Setup section*/

    char InterruptPath[40];
    sprintf(InterruptPath, "/sys/class/gpio/gpio%d/value", gpio_num); 

    int epfd;
    struct epoll_event ev; //setup for Linux poll

    FILE *epollfile = fopen(InterruptPath, "r");
    int filedesc  = fileno(epollfile);

    epfd = epoll_create(1); //creates epoll instance to monitor file contents
    ev.events = EPOLLIN | EPOLLET; //EPOLLIN detects new data; EPOLLET signals read event
    ev.data.fd = filedesc; //link interrupt file to epoll interface

    epoll_ctl(epfd, EPOLL_CTL_ADD, filedesc, &ev);


    /*Capture section*/

    int capture_interrupt;
    struct epoll_event ev_wait;
    struct timespec tm;

    for(int i=0; i < 5; i++){

        capture_interrupt = epoll_wait(epfd, &ev_wait, 1, -1);
        clock_gettime(CLOCK_MONOTONIC_RAW, &tm);
        
        &timestamps[i] = 
    }

    close(epfd);
    return 0;
}


int main(){

    char gpio_in_num[32] = "P8_09";
    int gpio_in_pin = 69;

    config_interrupt(gpio_in_pin, gpio_in_num); //config p8-09 as gpio input


    

    return 0;
}
