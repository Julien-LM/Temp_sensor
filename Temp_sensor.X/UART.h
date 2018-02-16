/* 
 * File:   UART.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Requirements: UART structure
 * 
 * Created on December 5, 2017, 12:57 PM
 */

#ifndef UART_H
#define	UART_H

#include "../../General_lib/structures.h"

void send_UART_char(char data) {
    while(!TXSTAbits.TRMT);
    TXREG = data;
}

void end_of_transmit(void) {
    send_UART_char(END_OF_TRANSMIT);
}

void send_UART_char_tab(char data[], unsigned short data_size) {
    unsigned short i=0;

    for(i=0; i<data_size; i++) {
        send_UART_char(data[i]);
    }
}

/*void send_UART_int(unsigned int data) {
    unsigned char output_sprintf[5] = {0};

    sprintf(output_sprintf, "%u", data);
    send_UART_char_tab(output_sprintf, sizeof(output_sprintf));
}*/

void return_UART_answer(char command, char data[], unsigned short data_size) {
    send_UART_char(ACKNOWLEDGE);
    send_UART_char(command);
    send_UART_char_tab(data, data_size);
    end_of_transmit();
    }

void return_UART_error(char command, char error_code) {
    send_UART_char(NEG_ACKNOWLEDGE);
    send_UART_char(command);
    send_UART_char(error_code);
    end_of_transmit();
}

char check_arg_size(char arg_size, UART uart) {
    if(uart.UART_reception_index != arg_size + 2) {
        return_UART_error(uart.UART_reception_buffer[0], WRONG_ARGUMENTS);
        return 0;
    } 
    return 1;
}

void ping(void) {
    unsigned char tab[1];
    tab[0] = PING;
    return_UART_answer(PING, tab, 1);
}

void parsing_done(UART* uart) {
    (*uart).UART_reception_index = 0;
    (*uart).UART_parsing_in_progress = 0;
}

void check_UART_errors(UART uart){
    if(RCSTAbits.OERR == 1) {
        return_UART_error(uart.UART_reception_buffer[uart.UART_reception_index], OVERRUN_ERROR);
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    if(RCSTAbits.FERR == 1) {
        return_UART_error(uart.UART_reception_buffer[uart.UART_reception_index], FRAMING_ERROR);
        LED_RED = 1;
    }
}

#endif	/* UART_H */

