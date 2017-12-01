/* 
 * File:   init.h
 * Author: ju
 *
 * Created on November 29, 2017, 3:11 PM
 */

#include <xc.h>
#include <stdlib.h>

#ifndef INIT_H
#define	INIT_H

void init_GPIO(void) {
    
    CPSCON0bits.CPSON = 0;   // CPS module is disabled
    
    TRISBbits.TRISB4 = 0;   // Configurated as an output
    TRISBbits.TRISB5 = 0;   // Configurated as an output
    TRISBbits.TRISB6 = 0;   // Configurated as an output
    TRISBbits.TRISB7 = 0;   // Configurated as an output
    
    ANSELBbits.ANSB4 = 0;   // Digital I/O
    ANSELBbits.ANSB5 = 0;   // Digital I/O
    
    TRISAbits.TRISA5 = 0;
    TRISAbits.TRISA4 = 0;
    
    ANSELAbits.ANSA4 = 0;   // Digital I/O


    
            
    //APFCON1bits.P1CSEL = 1;     // P1C function on RC1
    //APFCON1bits.CCP2SEL = 1;    // CCP2/P2A function is on RA5

}

void init_OSC(void) {
    OSCCONbits.IRCF = 7;    // 500kHz MF selected
    OSCCONbits.SCS = 2;     // Internal oscillator block
    // XTAL = 500KHz, efficient 125kHz
    
    PR2 = 0xFA;
    T2CONbits.TMR2ON = 1;   //Timer2 is on
    T2CONbits.T2CKPS = 0;   // Prescaler is 1
    T2CONbits.T2OUTPS = 4;  // 1:5 Postscaler*/
    // 100Hz interrupt

}

void init_interrupt(void){
    // General Interruption configuration
    INTCONbits.GIE = 1;        // Enables all active interrupts
    INTCONbits.PEIE = 1;       // Enables all active peripheral interrupts 
    
    // timer2 Interruption configuration
    PIE1bits.TMR2IE = 1;     // Enables the Timer2 to PR2 match interrupt
}

void init_UART(void)
{
    SSP1CON1bits.SSPEN = 0;
    SSP2CON1bits.SSPEN = 0;
    
    TXSTAbits.TX9 = 0;          // Selected 8-bits transmission
    TXSTAbits.TXEN = 1;         // Transmit enable
    TXSTAbits.SYNC = 0;         // Asynchronous mode
    TXSTAbits.BRGH = 1;         // Baud rate high speed
    
    RCSTAbits.SPEN = 1;         // Serial port enable
    RCSTAbits.RX9 = 0;          // Selected 8-bits reception
    RCSTAbits.CREN = 1;         // Continuous Receive Enable bit

    BAUDCONbits.BRG16 = 1;      // 16-bits Baud Rate Generator is used
    BAUDCONbits.SCKP = 0;       // Transmit non-inverted data to the TX/CK pin-
    SPBRG = 12;
    
    // BAUD RATE FORMULAS:
    // SYNC=0, BRG16=1, BRGH=1 => bd=Fosc/[4(SPBRG+1)]
    // 9600 = 500k/[4(X+1)] => SPBRG = 12
    
    APFCON0bits.RXDTSEL = 1;    // RX/DT function is on RC5
    APFCON0bits.TXCKSEL = 1;    // TX/CK function is on RC4

    
    /*TRISA4 = 0; // configured as output
 ANSA4 = 0; // configured as digital I/O
 TXCKSEL = 1; // TX/CK function is on RA4

 TRISA5 = 1; // configured as input
 RXDTSEL = 0; // RX/DT function is on RA1
 ANSA1 = 0; // configured as digital I/O
 TRISA1 = 1; // configured as input

 TX9 = 0; // 8-bits transmission
 TXEN = 1; // Transmit Enable
 SYNC = 0; // Asynchronus mode
 SENDB = 0; // ???
 BRGH = 1; //Baud Rate Generateur
 BRG16 = 1; //Baud Rate Generateur
 TRMT = 1; // ???

 SPBRG = 12; //Baud Rate Generateur

 SPEN = 1; // Serial Port Enable

 RX9 = 0; // Select 8 bits reception
 CREN = 1; // Continious Receive Enable Bit
 RCIE = 1;  
 PEIE = 1; // Enable Preripheral interruption */
 
}

void init(void){
    init_OSC();
    init_UART();
    init_interrupt();
    init_GPIO();
}

#endif	/* INIT_H */

