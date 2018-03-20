/*
 * File:   main.c
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on November 28, 2017, 10:22 PM
 */

#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "constants.h"
#include "../../General_lib/structures.h"
#include "init.h"
#include "UART.h"
#include "../../General_lib/time.h"
#include "../../General_lib/i2c.h"
#include "mem_storage.h"
#include "../../General_lib/ds1624.h"
#include "../../General_lib/MAX24AA1025.h"

// Interrupt counter
unsigned char counter = 0;
unsigned short temp_convert_count = 0;

// Time structure declaration
Time time;
UART uart;
MEM mem;

// Temperature sensor
unsigned char temp_real[2] = {0xFF, 0xFF};

// Used only for debugging new feature
unsigned char debug_values[DEBUG_VALUE_NUMBER] = {0x28};

void get_real_time_info(void);
void get_debug_value(void);
void parsing_received_data(void);

void main(void) {
    init();
    init_time(&time);
    init_mem(&mem);
    
    store_time(&mem, time);
    store_sample_rate(&mem);                    
    
    LED_RED = 0;
    LED_ORANGE = 0;
    LED_GREEN = 0;
    LED_BLUE = 0;

    while(1) {
        parsing_received_data();
    }
    return;
}

void parsing_received_data(void) {
    
    char received_command = 0;

    check_UART_errors(uart);
        
    if(uart.UART_reception_overflow) {
        if(uart.UART_reception_buffer[uart.UART_reception_index] == START_OF_TEXT) {
            uart.UART_reception_overflow = 0;
        }
    } else if(uart.UART_reception_buffer[uart.UART_reception_index - 1] == END_OF_TRANSMIT) {
        uart.UART_parsing_in_progress = 1;
        received_command = uart.UART_reception_buffer[0];

        if(received_command == GET_TEMP) {
            if(check_arg_size(GET_TEMP_SIZE, uart)) {
                get_temp(&mem, time);
            }
        } else if(received_command == GET_TIME) {
            if(check_arg_size(GET_TIME_SIZE, uart)) {
                get_time(time);
            }
        } else if(received_command == SET_TIME) {
            if(check_arg_size(SET_TIME_SIZE, uart)) {
                set_time(&time, uart.UART_reception_buffer);
                store_time(&mem, time);
            }
        } else if(received_command == CONFIGURE_SENSOR) {
            if(check_arg_size(CONFIGURE_SENSOR_SIZE, uart)) {
                configure_sensor(uart, &mem);
                store_sample_rate(&mem);
                temp_convert_count = 0;
            }
        } else if(received_command == CLEAN_DATA) {
            if(check_arg_size(CLEAN_DATA_SIZE, uart)) {
                clean_data(&mem, time);
            }
        } else if(received_command == GET_DATA_NUMBER) {
            if(check_arg_size(GET_DATA_NUMBER_SIZE, uart)) {
                get_data_number(&mem);
            }
        } else if(received_command == GET_DEBUG_VALUES) {
            if(check_arg_size(GET_DEBUG_VALUES_SIZE, uart)) {
                get_debug_value();
            }
        } else if(received_command == PING) {
            if(check_arg_size(PING_NUMBER_SIZE, uart)) {
                ping();
            }
        } else if(received_command == GET_REAL_TIME_INFO) {
            if(check_arg_size(GET_REAL_TIME_INFO_S, uart)) {
                get_real_time_info();
            }
        } else {
            return_UART_error(uart.UART_reception_buffer[0], UNKNOWN_COMMAND);
        }
        parsing_done(&uart);
    }
}

void __interrupt led_blinking(void) {
    
    char received_data;

    // Timer1 interrupt flag, every second
    // Timer triggered by quartz
    if(PIR1bits.TMR1IF) {
        TMR1IF = 0;
        TMR1H = 0xF0;
        // CLear watchdog
        CLRWDT();
        // LED blue blinking
        //LED_BLUE ? LED_BLUE = 0: LED_BLUE = 1;
        // Increment time
        icremente_time(&time);
        // Temperature sensor conversion
        if(temp_convert_count == mem.temp_sample_rate-1) {
            start_convert();
        }
        temp_convert_count++;
        // Read and store data routine
        if(temp_convert_count == mem.temp_sample_rate) {
            temp_convert_count = 0;
            read_temp(temp_real);
            if(temp_real[0] != 0xFF) {
                store_data(&mem, temp_real);
            }
        }
        
        if(mem.page_size_reach) {
            write_data_storage_in_max24aa(&mem);
        }
    }
    
    if(PIR1bits.RCIF) {
        received_data = RCREG;
        if(uart.UART_parsing_in_progress) {
           return_UART_error(received_data, DEVICE_BUSY);
        } else {
            //LED_ORANGE ? LED_ORANGE = 0: LED_ORANGE = 1;
            uart.UART_reception_buffer[uart.UART_reception_index] = received_data;
            // Stop incrementing in overflow
            if(!uart.UART_reception_overflow) {
                uart.UART_reception_index++;
            }
            if(uart.UART_reception_index == RECEPTION_BUFFER_SIZE) {
               uart.UART_reception_overflow = 1;
               uart.UART_reception_index = 0;
               return_UART_error(uart.UART_reception_buffer[0], BUFFER_OVERFLOW);
           }
        }
    }
}

void get_real_time_info(void) {
    return_UART_answer(GET_REAL_TIME_INFO, temp_real, 2);
    get_data_number(&mem);
    get_time(time);
    get_config_sensor(&mem);
}

void get_debug_value() {
    //ADCON0bits.GO_nDONE = 1;
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE) {
        NOP();
    }
    unsigned char tab[1];
    
    tab[0] = ADRESH;

    return_UART_answer(GET_DEBUG_VALUES, tab, 1);

    
    /*EEPROM_read_sequential(0x0007D, debug_values, 5);
    return_UART_answer(GET_DEBUG_VALUES, debug_values, DEBUG_VALUE_NUMBER);*/
}