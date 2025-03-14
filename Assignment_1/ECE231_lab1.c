#include <stdio.h>
#include <time.h>

#define BUFFERSIZE 10

void calculate_mean(int buf[], struct timespec *timestamp, int *avg){
    
    int total=0;

    for(int i=0; i<BUFFERSIZE; i++){ //sum
        total += buf[i];
    }

    *avg = total / BUFFERSIZE; //int div for avg

    clock_gettime(CLOCK_MONOTONIC, timestamp); //store timestamp in var timestamp

    return;
}


int check_duplicate(int buf[], int size, int value){

    for (int i=0;i<size;i++){ //for each array item
        if (buf[i]==value){ //check if item already exists
            return 1;
        }
        
    }
    return 0; //No duplicstes detected
}


int main(){
    struct timespec timestamp;

    for(int j=0; j<2; j++){

        int avg;
        int input;
        int buffer[BUFFERSIZE];
        int index=0; 

        while(index<BUFFERSIZE){ //loop for input
            printf("Enter a number between 0 and 9000 (%d): ", index+1);
            scanf("%d", &input);

            if (input < 0 || input > 9000){ //range check
                printf("Value out of range. Re-enter a value in range.\n");
                continue;
            }
            
            if (check_duplicate(buffer, index, input)==1){ //duplicates check using helper function
                printf("Duplicates not allowed. Re-enter a unique value. \n");
                continue;
            }

            buffer[index] = input;
        
            index += 1;
        } 

        calculate_mean(buffer, &timestamp, &avg);

        printf("\nMean: %d \n", avg);
        printf("Timestamp(s): %ld \n", timestamp.tv_sec);
        printf("Timestamp(ns): %ld \n", timestamp.tv_nsec);
        
        
    }

    return 0;
}