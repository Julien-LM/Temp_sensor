/*
 * File:   main.c
 * Author: ju
 *
 * Created on November 28, 2017, 10:22 PM
 */

#define LED_RED     PORTBbits.RB4
#define LED_ORANGE  PORTBbits.RB5
#define LED_GREEN   PORTBbits.RB6
#define LED_BLUE    PORTBbits.RB7
#define LED_YELLOW  PORTAbits.RA5


#include <xc.h>
#include "constants.h"
#include "init.h"

int counter = 0;


void main(void) {
    
    init();
    
    LED_RED = 0;
    LED_ORANGE = 1;
    LED_GREEN = 0;
    LED_BLUE = 1;
    LED_YELLOW = 0;


    while(1) {
        //PORTCbits.RC3 = 0;
        //PORTCbits.RC6 = 1;
        //PORTCbits.RC7 = 0;
        NOP();
    }
    
    /*
     TODO: 
     *  - timer configuration
     *  - Blink led every second
     *  - Define pinout
     *  - UART configuration
     *  - Send a code every seconds
     *  
     */
    return;
}

void interrupt led_blinking(void) {
    // Timer2 interrupt flag
    // 100 Hz interrupt
    if(TMR2IF == 1) {
        TMR2IF = 0;
        
        counter++;
        if(counter >=  10000){
            counter = 0;
            if(RC7) {
                RC7 = 0;
            } else {
                RC7 = 0;
            }
            TXREG = 0x31;
        }
        

        //PORTC = 0xFF;
        //RC7 = 1;
        //RC6 = 1;
        //RC3 = 1;
        // LED blinking
    }
}



/*
#define _LEGACY_HEADERS

#include<pic.h>

void init(void);
void initEUART(void);
void initInterruptTimer(void);

char Data = 0;
char compt = 0;
char ok = 0;

void interrupt SendEUART(void)
{
	 if(RCIF == 1)
	 {
		Data = RCREG; // On stoke la Valeur se du registre du reception de l'EUSART
	 }

	 if(TMR0IF == 1)
	 {
		TMR0IF = 0; // reset du flag d'overflow du timer 0
		//if(ok == 0)
		//	RA2 = !RA2; // Aide au debugage
		
		if(Data == '1' && ok == 0)
		{// On a reçu l'identifiant corect provenant de la STM
			TMR0 = 0; // On remet les compteurs à zéro
			compt = 0;
			ok = 1; // => Confirme la réception d' l'id et autorise la réponse
			
		}
		if(ok == 1)
			TXREG = 0x31; // Chargement d'un valeur dans le registre d'envoi de l'EUSART
		compt++;
		if(compt == 15)
		{// Au bout de deux secondes, on arrête tout.
			compt = 0;
			Data = 0;
			ok=0;
		}	
	 }
}


void main(void)
{

 init();

 initEUART();
 initInterruptTimer();

 while(1)
	{
	}

}



void init(void)
{

 TRISA2 = 0; // configured as output
 ANSA2 = 0; // configured as digital I/O

}






void initInterruptTimer(void)
{	
 PSA = 0; //prescaler enable
 PS0 = 0; 
 PS1 = 0;
 PS2 = 1; // prescaler = 0
 TMR0CS = 0; // Source from internal intruction cyce clock (Fosc/4)

 TMR0IE = 1; //Timer0 Overflow Interrupt Enable 
 GIE = 1; // Globale Interrupt Enable

}*/
