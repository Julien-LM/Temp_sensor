/* 
 * File:   mem_storage.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on December 18, 2017, 8:29 AM
 */

#ifndef MEM_STORAGE_H
#define	MEM_STORAGE_H

void get_data_number(MEM* mem) {
    char tab[2] = {0};
    tab[0] = (*mem).data_storage_index & 0x00FF;
    tab[1] = ((*mem).data_storage_index & 0xFF00) >> 8;
    return_UART_answer(GET_DATA_NUMBER, tab, 2);   
}

void add_data_char(char data, MEM* mem) {
    if(!(*mem).data_storage_overflow) {
        (*mem).data_storage[(*mem).data_storage_index] = data;
        (*mem).data_storage_index++;
    }
    if((*mem).data_storage_index == DATA_STORAGE_SIZE) {
        (*mem).data_storage_index = 0;
        (*mem).data_storage_overflow = 1;
    }
}

void add_data_long(long data, MEM* mem){
    add_data_char(data & 0x000000FF, mem);
    add_data_char((data & 0x0000FF00) >> 8, mem);
    add_data_char((data & 0x00FF0000) >> 16, mem);
    add_data_char((data & 0xFF000000) >> 24, mem);
}

void store_data(MEM* mem, Time time, unsigned char* temp_real) {
    if(!(*mem).time_info_storage) {
        (*mem).time_info_storage = 1;
        add_data_char(TIME_TRANSFERT_IND, mem);
        add_data_char(time.century, mem);
        add_data_char(time.years, mem);
        add_data_char(time.months, mem);
        add_data_char(time.days, mem);
        add_data_char(time.hours, mem);
        add_data_char(time.minutes, mem);
        add_data_char(time.seconds, mem);
    }
    if(!(*mem).sample_rate_info_storage) {
        (*mem).sample_rate_info_storage = 1;
        add_data_char(S_RATE_TRANSFERT_IND, mem);
        add_data_long((*mem).temp_sample_rate, mem);
    }
    add_data_char(temp_real[1], mem);
    add_data_char(temp_real[0], mem);
}

void get_temp(MEM* mem) {
    (*mem).data_storage_overflow = 0;
    return_UART_answer(GET_TEMP, (*mem).data_storage, (*mem).data_storage_index);
}

void configure_sensor(UART uart, MEM* mem) {
    char tab[2] = {0};
    (*mem).temp_sample_rate = ((uart.UART_reception_buffer[1] & 0x3F) *
            (((((uart.UART_reception_buffer[1] & 0x80) >> 7) * 3599)+1) *
            ((((uart.UART_reception_buffer[1] & 0x40) >> 6) * 59)+1)));
     
    tab[0] = (*mem).temp_sample_rate & 0x00FF;
    tab[1] = ((*mem).temp_sample_rate & 0xFF00) >> 8;
    
    return_UART_answer(CONFIGURE_SENSOR, tab, 2);
}

void clean_data(MEM* mem) {
    int i = 0;
    (*mem).data_storage_overflow = 0;
    for(i=0; i < DATA_STORAGE_SIZE; i++) {
        (*mem).data_storage[i] = 0;
    }
}

#endif	/* MEM_STORAGE_H */
