#include <stdio.h>
#include <time.h>

#define BUFFERSIZE 10

void calculate_mean(int buf[], struct timespec *timestamp, int *avg){
    
    int total=0;

    for(int i=0; i<BUFFERSIZE; i++){ //sum
        total += buf[i];
    }

    *avg = total / BUFFERSIZE; //int div for avg

    clock_gettime(CLOCK_MONOTONIC, timestamp);

    return;
}



int main(){
    struct timespec timestamp;

    for(int j=0;j<2;j++){

        int avg;

        int buffer[BUFFERSIZE]; 

        for(int i=0; i<BUFFERSIZE;i++){ //loop for input
            printf("Enter a number between 0 and 9000 (%d): ", i+1);
            scanf("%d", &buffer[i]);
        } 

        calculate_mean(buffer, &timestamp, &avg);

        printf("\nMean: %d \n", avg);
        printf("Timestamp(s): %ld \n", timestamp.tv_sec);
        printf("Timestamp(ns): %ld \n", timestamp.tv_nsec);
        
        
    }

    return 0;
}