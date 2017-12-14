/* 
 * File:   UART.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on December 5, 2017, 12:57 PM
 */

#ifndef UART_H
#define	UART_H


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

#endif	/* UART_H */

