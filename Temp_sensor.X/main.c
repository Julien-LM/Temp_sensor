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
#include "i2c.h"

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
unsigned char temp_real[2] = {0};
unsigned short temp_convert_count = 0;

unsigned short temp_sample_rate = 5;

void get_temp(void);
void configure_sensor(void);
void clean_data(void);
void get_data_number(void);
void ping(void);
void parsing_done(void);
void check_errors(void);
void check_UART_errors(void);
char check_arg_size(char arg_size);


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
                if(check_arg_size(GET_TEMP_SIZE)) {
                    get_temp();
                }
            } else if(received_command == GET_TIME) {
                if(check_arg_size(GET_TIME_SIZE)) {
                    get_time(time);
                }
            } else if(received_command == SET_TIME) {
                if(check_arg_size(SET_TIME_SIZE)) {
                    set_time(&time, reception_buffer);
                }
            } else if(received_command == CONFIGURE_SENSOR) {
                if(check_arg_size(CONFIGURE_SENSOR_SIZE)) {
                    configure_sensor();
                }
            } else if(received_command == CLEAN_DATA) {
                if(check_arg_size(CLEAN_DATA_SIZE)) {
                    clean_data();
                }
            } else if(received_command == GET_DATA_NUMBER) {
                if(check_arg_size(GET_DATA_NUMBER_SIZE)) {
                    get_data_number();
                }
            } else if(received_command == PING) {
                if(check_arg_size(PING_NUMBER_SIZE)) {
                    ping();
                }
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
        counter++;
        if(counter >=  100){
            counter = 0;
            //LED_ORANGE ? LED_ORANGE = 0: LED_ORANGE = 1;
        }
    }
    // Timer1 interrupt flag, every second
    // Timer triggered by quartz
    if(PIR1bits.TMR1IF) {
        TMR1IF = 0;
        TMR1H = 0xF0;
        LED_BLUE ? LED_BLUE = 0: LED_BLUE = 1;
        icremente_time(&time);
        // Temperature sensor conversion
        if(temp_convert_count == temp_sample_rate-1) {
            start_convert();
        }
        temp_convert_count++;
        if(temp_convert_count == temp_sample_rate) {
            temp_convert_count = 0;
            read_temp(temp_real);
            LED_RED = 0;
        }
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
    return_UART_answer(GET_TEMP, temp_real, 2);
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
        LED_RED = 1;
    }
}

char check_arg_size(char arg_size) {
    if(reception_index != arg_size + 2) {
        return_UART_error(reception_buffer[0], WRONG_ARGUMENTS);
        return 0;
    } 
    return 1;
}

void configure_sensor(void) {
    temp_sample_rate = reception_buffer[1];
    return_UART_answer(CONFIGURE_SENSOR, 0, 0);
}

void clean_data(void) {
    NOP();
}

void get_data_number(void) {
    NOP();
}

void ping(void) {
    unsigned char tab[1];
    tab[0] = PING;
    return_UART_answer(PING, tab, 1);
}