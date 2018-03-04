/* 
 * File:   mem_storage.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Requirements: Mem structure
 * 
 * Created on December 18, 2017, 8:29 AM
 */

#ifndef MEM_STORAGE_H
#define	MEM_STORAGE_H

#include "constants.h"
#include "../../General_lib/structures.h"
#include "../../General_lib/MAX24AA1025.h"

char temp_data[TEMPORARY_DATA_SIZE];


void get_data_number(MEM* mem) {
    char tab[4] = {0};
    tab[0] = (*mem).data_number & 0x000000FF;
    tab[1] = ((*mem).data_number & 0x0000FF00) >> 8;
    tab[2] = ((*mem).data_number & 0x00FF0000) >> 16;
    tab[3] = ((*mem).data_number & 0xFF000000) >> 24;
    return_UART_answer(GET_DATA_NUMBER, tab, 4);   
}

void add_data_char(char data, MEM* mem) {
    if(!(*mem).page_size_reach) {
        (*mem).data_storage[(*mem).data_storage_index] = data;
        (*mem).data_storage_index++;
    } else {
        (*mem).data_storage_tampon[(*mem).tampon_index] = data;
        (*mem).tampon_index++;
    }
    
    if((*mem).data_storage_index == DATA_STORAGE_SIZE) {
        (*mem).page_size_reach = 1;
    }
    (*mem).data_number++;
}

void write_data_storage_in_max24aa(MEM* mem) {
    int i;
    EEPROM_write_page((*mem).data_storage, (*mem).address_max24aa);
    (*mem).address_max24aa += 128;
    if((*mem).address_max24aa >= MAX_MAX24AA_SIZE) {
        (*mem).data_storage_overflow = 1;
    }
    (*mem).data_storage_index = 0;
    (*mem).page_size_reach = 0;
    
    // Write tampon data into data storage
    for(i=0; i<(*mem).tampon_index; i++) {
        (*mem).data_storage[(*mem).data_storage_index] = 
                (*mem).data_storage_tampon[(*mem).data_storage_index];
        (*mem).data_storage_index++;
    }
    (*mem).tampon_index = 0;
}

void add_data_long(long data, MEM* mem){
    add_data_char((data & 0xFF000000) >> 24, mem);
    add_data_char((data & 0x00FF0000) >> 16, mem);
    add_data_char((data & 0x0000FF00) >> 8, mem);
    add_data_char(data & 0x000000FF, mem);
}

void store_data(MEM* mem, unsigned char* temp_real) {
    add_data_char(temp_real[0], mem);
    add_data_char(temp_real[1], mem);
}

void store_time(MEM* mem, Time time) {
    add_data_char(TIME_TRANSFERT_IND, mem);
    add_data_char(time.century, mem);
    add_data_char(time.years, mem);
    add_data_char(time.months, mem);
    add_data_char(time.days, mem);
    add_data_char(time.hours, mem);
    add_data_char(time.minutes, mem);
    add_data_char(time.seconds, mem);
}

void store_sample_rate(MEM* mem) {
    add_data_char(S_RATE_TRANSFERT_IND, mem);
    add_data_long((*mem).temp_sample_rate, mem);
}

void clean_data(MEM* mem, Time time) {

    (*mem).address_max24aa = 0;     // Clear max24aa mem index
    (*mem).data_storage_index = 0;  // Clear local data storage

    (*mem).data_storage_overflow = 0;
    
    store_time(mem, time);
    store_sample_rate(mem);
    return_UART_answer(CLEAN_DATA, 0, 0);
}

void get_temp(MEM* mem, Time time) {    
    int i=0;
    
    send_UART_char(ACKNOWLEDGE);
    send_UART_char(GET_TEMP);
    
    while(i < (*mem).address_max24aa) {
        EEPROM_read_sequential(i, temp_data, TEMPORARY_DATA_SIZE);
        send_UART_char_tab(temp_data, TEMPORARY_DATA_SIZE);
        i+=TEMPORARY_DATA_SIZE;
    }
    send_UART_char_tab((*mem).data_storage, (*mem).data_storage_index);
    end_of_transmit();
    
    if((*mem).data_storage_overflow == 1) {
        clean_data(mem, time);
    }
}

void get_config_sensor(MEM* mem) {
    char tab[2] = {0};
    
    tab[0] = (*mem).temp_sample_rate & 0x00FF;
    tab[1] = ((*mem).temp_sample_rate & 0xFF00) >> 8;
    
    return_UART_answer(CONFIGURE_SENSOR, tab, 2);
}

void configure_sensor(UART uart, MEM* mem) {
    (*mem).temp_sample_rate = ((uart.UART_reception_buffer[1] & 0x3F) *
            (((((uart.UART_reception_buffer[1] & 0x80) >> 7) * 3599)+1) *
            ((((uart.UART_reception_buffer[1] & 0x40) >> 6) * 59)+1)));
    get_config_sensor(mem);
}

void init_mem(MEM* mem) {
    (*mem).address_max24aa = 0;
    (*mem).temp_sample_rate = 5;
    (*mem).data_storage_overflow = 0;
    (*mem).data_storage_index = 0;
    (*mem).page_size_reach = 0;
    (*mem).tampon_index = 0;
    (*mem).data_number = 0;
}

#endif	/* MEM_STORAGE_H */

