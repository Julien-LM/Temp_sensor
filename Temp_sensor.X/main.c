/*
 * File:   main.c
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on November 28, 2017, 10:22 PM
 */

#define LED_RED     PORTBbits.RB4
#define LED_ORANGE  PORTBbits.RB5
#define LED_GREEN   PORTBbits.RB6
#define LED_BLUE    PORTBbits.RB7

#include <xc.h>
#include "constants.h"
#include "init.h"

char counter = 0;

short years = 0;
char months = 0;
char days = 0;
char hours = 0;
char minutes = 0;
char seconds = 0;

void icremente_time(void);
void send_UART_char(char data);
void send_UART_char_tab(char data[]);

char data_tested[3] = {'a', 'b', 'c'};

void main(void) {
    
    init();
    
    LED_RED = 0;
    LED_ORANGE = 0;
    LED_GREEN = 0;
    LED_BLUE = 0;

    while(1) {

        NOP();
    }

    return;
}

void interrupt led_blinking(void) {
    // Timer2 interrupt flag
    // 100 Hz interrupt
    if(TMR2IF == 1) {
        TMR2IF = 0;
        LED_RED ? LED_RED = 0: LED_RED = 1;
        counter++;
        if(counter >=  100){
            counter = 0;
            LED_GREEN ? LED_GREEN = 0: LED_GREEN = 1;
            send_UART_char_tab(data_tested);
        }
    }
    
    // Timer1 interrupt flag, every second
    if(TMR1IF == 1) {
        TMR1IF = 0;
        TMR1H = 0xF0;
        LED_BLUE ? LED_BLUE = 0: LED_BLUE = 1;
        //send_char_UART(0x37);
        icremente_time();
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
    TXREG = data;
}


void send_UART_char_tab(char data[]) {
    int i=0;
    for(i=0; i<3; i++) {
        send_UART_char(data[i]);
    }
}

