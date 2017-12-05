/*
 * File:   main.c
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on November 28, 2017, 10:22 PM
 */

#include <xc.h>
#include <stdio.h>
#include "constants.h"
#include "init.h"

unsigned char counter = 0;

// Time values
unsigned short years = 0;
unsigned char months = 0;
unsigned char days = 0;
unsigned char hours = 0;
unsigned char minutes = 0;
unsigned char seconds = 0;

// UART reception
char read_data;
char reception_buffer[RECEPTION_BUFFER_SIZE] = {0};
char reception_index = 0;

// Errors
char UART_reception_overflow = 0;


void icremente_time(void);
void send_UART_char(unsigned char data);
void send_UART_char_tab(unsigned char data[], unsigned char data_size);
void send_UART_int(unsigned int data);
void return_UART_answer(char command, char data[]);
void return_UART_error(char command, char error_code);

void main(void) {
    
    char received_command = 0;
    
    init();
    
    LED_RED = 0;
    LED_ORANGE = 0;
    LED_GREEN = 0;
    LED_BLUE = 0;

    while(1) {
        if(reception_buffer[reception_index] == END_OF_TRANSMIT) {
            received_command = reception_buffer[0];
            
        }
    }

    return;
}

void __interrupt led_blinking(void) {
    // Timer2 interrupt flag
    // 100 Hz interrupt
    if(PIR1bits.TMR2IF) {
        TMR2IF = 0;
        //LED_RED ? LED_RED = 0: LED_RED = 1;
        counter++;
        if(counter >=  100){
            counter = 0;
            LED_GREEN ? LED_GREEN = 0: LED_GREEN = 1;
            //send_UART_char_tab("lig", sizeof("lig"));
            //send_UART_int(9676);
        }
    }
    // Timer1 interrupt flag, every second
    if(PIR1bits.TMR1IF) {
        TMR1IF = 0;
        TMR1H = 0xF0;
        LED_BLUE ? LED_BLUE = 0: LED_BLUE = 1;
        //send_char_UART(0x37);
        icremente_time();
    }
    if(PIR1bits.RCIF) {
       LED_ORANGE ? LED_ORANGE = 0: LED_ORANGE = 1;
       reception_buffer[reception_index] = RCREG;
       reception_index++;
       if(reception_index == RECEPTION_BUFFER_SIZE) {
           reception_index = 0;
           UART_reception_overflow = 1;
       }
    }
}

void icremente_time(void) {
    seconds++;
    if(seconds == 60) {
        seconds = 0;
        minutes++;
        if(minutes == 60) {
            minutes = 0;
            hours++;
            if(hours == 24) {
                hours = 0;
                days++;
                if(months == 2 && days == 29) {
                    days = 0;
                    months++;
                } else if(months == 4 || months == 6 || months == 9 || months == 11) {
                    if(days == 31) {
                        days = 0;
                        months++;
                    }
                } else {
                    if(days == 32) {
                        days = 0;
                        months++;
                        if(months == 13) {
                            years++;
                        }
                    }
                }
            }
        }
    }
}

void send_UART_char(char data) {
    while(!TXSTAbits.TRMT);
    TXREG = data;
}


void send_UART_char_tab(char data[], char data_size) {
    unsigned int i=0;

    for(i=0; i<data_size; i++) {
        send_UART_char(data[i]);
    }
    send_UART_char(LINE_FEED);
    send_UART_char(CARRIAGE_RETURN);
}

void send_UART_int(unsigned int data) {
    unsigned char output_sprintf[5] = {0};

    sprintf(output_sprintf, "%u", data);
    send_UART_char_tab(output_sprintf, sizeof(output_sprintf));
}

void return_UART_answer(char command, char data[]) {
    NOP();
}

void return_UART_error(char command, char error_code) {
    NOP();
}
