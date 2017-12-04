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

int counter = 0;


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
            TXREG = 0x31;
        }
    }
}
