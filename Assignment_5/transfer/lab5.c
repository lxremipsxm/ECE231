#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/epoll.h>
#include <pthread.h>
#define SIZE 10

/*Shared Variables between Threads*/
struct data_type {
    struct timespec timestamp;
    pthread_t thread_id;
};

struct data_type buffer[SIZE];
int count = 0; //number of values in buffer
int put = 0; //index to write to buffer
int get = 0; //index to read to buffer

pthread_mutex_t lock; //global mutex lock


void config_pin(char *pin_num, char *mode){
    if(!vfork()){
        int ret = execl("/usr/bin/config-pin", "config-pin", pin_num, mode, NULL);
    }
}

void config_gpio_input(int gpio_num, char *pin_num){

    config_pin(pin_num, "gpio_pu");
    char GPIODir[40];
    sprintf(GPIODir, "/sys/class/gpio/gpio%d/direction", gpio_num);

    FILE *fp;
    fp = fopen(GPIODir, "w");

    fwrite("in", sizeof(char), 2, fp);
    fclose(fp);

}

void config_rising_interrupt(int gpio_num, char *pin_num){
    config_gpio_input(gpio_num, pin_num);

    char Edge[40];
    sprintf(Edge, "/sys/class/gpio/gpio%d/edge", gpio_num);

    FILE *fp;
    fp = fopen(Edge, "w");

    fwrite("rising", sizeof(char), 7, fp);
    fclose(fp);
    
}


void *input_thread(void *var){

    /*Setup pins and path*/
    char pin_num[32] = "P8_09";
    int gpio_num = 69;
    config_rising_interrupt(gpio_num, pin_num); //configure rising edge on gpio69
    char InterruptPath[40];
    sprintf(InterruptPath, "/sys/class/gpio/gpio%d/value", gpio_num);
    int max = *((int*) var);


    /*Setup capture*/
    int epfd; //for epoll instance
    struct epoll_event ev;

    FILE *fp;
    fp = fopen(InterruptPath, "r");
    int fd = fileno(fp); //file descriptor
    
    epfd = epoll_create(1); //create epoll instance
    ev.events = EPOLLIN | EPOLLET; // detects changes, signals read events
    
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev); //Register interrupt file with epoll interface


    /*Begin Capture*/
    int capture;
    struct epoll_event ev_wait;


    int num = 0; //indicates how many values have been passed into the buffer OVERALL

    while (num <= max){
        /*Critical Section begins*/
        pthread_mutex_lock(&lock);

        while(count == SIZE){
            pthread_mutex_unlock(&lock);
            pthread_mutex_lock(&lock);
        }

        capture = epoll_wait(epfd, &ev_wait, 1, -1);

        clock_gettime(CLOCK_MONOTONIC, &buffer[put].timestamp);
        buffer[put].thread_id = pthread_self();

        //adjust number of items and put index
        printf("Current put index: %d, Total # of interrupts detected: %d \n", put, num);
        count++; 
        put++; 
        
        if (put == SIZE){ //end of buffer reached
            put = 0;
        }

        num++; //increment num
        pthread_mutex_unlock(&lock);
        /*Critical Section ends*/
    }
}


void *output_thread(void *var){

    FILE *fp;
    fp = fopen("Siddharth_Vasudevan_data.txt", "a"); //open file in append mode

    int num = 0;
    int max = *((int*) var);

    while (num <= max) {
        /*Critical Section begins*/
        pthread_mutex_lock(&lock);

        while (count == 0){
            pthread_mutex_unlock(&lock);
            pthread_mutex_lock(&lock);
        }

        char data_to_file[80];
        sprintf(data_to_file, "Timestamp(s): %ld, Timestamp(ns): %ld, Thread ID: %d \n", 
            buffer[get].timestamp.tv_sec, buffer[get].timestamp.tv_nsec, buffer[get].thread_id);

        printf("%s", data_to_file); //print to terminal
        fprintf(fp, data_to_file);//write to file
        
        count--;
        get++;
        num++;

        if (get == SIZE){
            get = 0;
        }
        
        pthread_mutex_unlock(&lock);
        /*Critical Section ends*/
    }
}


int main() {

    int num = 50;
    printf("Initializing threads.\n");

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("Mutex not initialized\n");
        return 1;
    }

    pthread_t in;
    pthread_t out;


    pthread_create(&in, NULL, input_thread, (void*)(&num));
    pthread_create(&out, NULL, output_thread, (void*)(&num));

    pthread_join(in, NULL);
    pthread_join(out, NULL);

    pthread_mutex_destroy(&lock);

    printf("All threads have finished executing\n");

    pthread_exit(0);
    return 0;
}