/*
Siddharth Vasudevan, ECE231 SP25 Lab 8.2
lab8_2.c
4/16/2025

Description: Implements a digital thermometer 

Connections:
*/

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>


int main (void){
    UCSR0B = 1<<TXEN0; //enable Tx

    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);

    UBRR0L = 103;

    while(1){

    while ((UCSR0A & (1<<UDRE0)) == 0){
        
    }

    UDR0='B';
        _delay_ms(500);

}

}