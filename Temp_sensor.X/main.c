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
#include "UART.h"
#include "time.h"

unsigned char counter = 0;

// Time structure declaration
Time time;

// UART reception
unsigned char reception_buffer[RECEPTION_BUFFER_SIZE] = {0};
unsigned char reception_index = 0;
unsigned char parsing_in_progress = 0;

// Errors
unsigned char UART_reception_overflow = 0;

// Temperature sensor
char temp = 20;
unsigned char temp_real = 0;

void get_temp(void);
void parsing_done(void);
void check_errors(void);
void check_UART_errors(void);


void main(void) {
    
    char received_command = 0;    
    init();
    
    LED_RED = 0;
    LED_ORANGE = 0;
    LED_GREEN = 0;
    LED_BLUE = 0;

    while(1) {
        
        check_errors();
        
        if(UART_reception_overflow) {
            if(reception_buffer[reception_index] == START_OF_TEXT) {
                UART_reception_overflow = 0;
            }
        } else if(reception_buffer[reception_index - 1] == END_OF_TRANSMIT) {
            parsing_in_progress = 1;
            received_command = reception_buffer[0];
            if(received_command == GET_TEMP) {
                if(reception_index != GET_TEMP_SIZE + 2) {
                    return_UART_error(reception_buffer[0], WRONG_ARGUMENTS);
                } else {
                    get_temp();
                }
            } else if(received_command == GET_TIME) {
                NOP();
            } else if(received_command == SET_TIME) {
                if(reception_index != SET_TIME_SIZE + 2) {
                    return_UART_error(reception_buffer[0], WRONG_ARGUMENTS);
                } else {
                    set_time(time, reception_buffer);
                }
            } else if(received_command == CONFIGURE_SENSOR) {
                NOP();
            } else if(received_command == CLEAN_DATA) {
                NOP();
            } else if(received_command == GET_DATA_NUMBER) {
                NOP();
            } else {
                return_UART_error(reception_buffer[0], UNKNOWN_COMMAND);
            }
            parsing_done();
            
        }
    }

    return;
}

void __interrupt led_blinking(void) {
    
    char received_data;
    
    // Timer2 interrupt flag
    // 100 Hz interrupt
    if(PIR1bits.TMR2IF) {
        TMR2IF = 0;
        //LED_RED ? LED_RED = 0: LED_RED = 1;
        counter++;
        if(counter >=  100){
            counter = 0;
            LED_GREEN ? LED_GREEN = 0: LED_GREEN = 1;
            if(UART_reception_overflow) {
                LED_RED = 1;
            } else {
                LED_RED = 0;
            }
            if(RCSTAbits.OERR == 1) {
                LED_ORANGE = 1;
                RCSTAbits.CREN = 0;
                RCSTAbits.CREN = 1;
            } else {
                LED_ORANGE = 0;
            }
            //send_UART_char_tab("LIBK", sizeof("UART"));
            //send_UART_int(9676);
        }
    }
    // Timer1 interrupt flag, every second
    if(PIR1bits.TMR1IF) {
        TMR1IF = 0;
        TMR1H = 0xF0;
        LED_BLUE ? LED_BLUE = 0: LED_BLUE = 1;
        icremente_time(time);
    }
    if(PIR1bits.RCIF) {
        received_data = RCREG;
        if(parsing_in_progress) {
           return_UART_error(received_data, DEVICE_BUSY);
        } else {
            //LED_ORANGE ? LED_ORANGE = 0: LED_ORANGE = 1;
            reception_buffer[reception_index] = received_data;
            // Stop incrementing in overflow
            if(!UART_reception_overflow) {
                reception_index++;
            }
            if(reception_index == RECEPTION_BUFFER_SIZE) {
               UART_reception_overflow = 1;
               reception_index = 0;
               return_UART_error(reception_buffer[0], BUFFER_OVERFLOW);
           }
        }
    }
}

void get_temp(void) {
    unsigned char tab[2];
    tab[0] = temp;
    tab[1] = temp_real;
    return_UART_answer(GET_TEMP, tab, 2);
}

void parsing_done(void) {
    reception_index = 0;
    parsing_in_progress = 0;   
}

void check_errors(void){
    check_UART_errors();
}
void check_UART_errors(void){
    if(RCSTAbits.OERR == 1) {
        return_UART_error(reception_buffer[reception_index], OVERRUN_ERROR);
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    if(RCSTAbits.FERR == 1) {
        return_UART_error(reception_buffer[reception_index], FRAMING_ERROR);
    }
    
}