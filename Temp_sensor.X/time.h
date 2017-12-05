/* 
 * File:   time.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on December 5, 2017, 1:00 PM
 */

#ifndef TIME_H
#define	TIME_H

void icremente_time(Time* time) {
    (*time).seconds++;
    if((*time).seconds == 60) {
        (*time).seconds = 0;
        (*time).minutes++;
        if((*time).minutes == 60) {
            (*time).minutes = 0;
            (*time).hours++;
            if((*time).hours == 24) {
                (*time).hours = 0;
                (*time).days++;
                if((*time).months == 2 && (*time).days == 29) {
                    (*time).days = 0;
                    (*time).months++;
                } else if((*time).months == 4 || (*time).months == 6 || (*time).months == 9 || (*time).months == 11) {
                    if((*time).days == 31) {
                        (*time).days = 0;
                        (*time).months++;
                    }
                } else {
                    if((*time).days == 32) {
                        (*time).days = 0;
                        (*time).months++;
                        if((*time).months == 13) {
                            (*time).years++;
                        }
                    }
                }
            }
        }
    }
}

void set_time(Time* time, char reception_buffer[]) {
    (*time).years = reception_buffer[1];
    (*time).months = reception_buffer[2];
    (*time).days = reception_buffer[3];
    (*time).hours = reception_buffer[4];
    (*time).minutes = reception_buffer[5];
    (*time).seconds = reception_buffer[6];
    return_UART_answer(SET_TIME, 0, 0);
}

void get_time(Time time) {
    unsigned char tab[6];
    tab[0] = time.years;
    tab[1] = time.months;
    tab[2] = time.days;
    tab[3] = time.hours;
    tab[4] = time.minutes;
    tab[5] = time.seconds;
    return_UART_answer(GET_TIME, tab, 6);
}

#endif	/* TIME_H */

