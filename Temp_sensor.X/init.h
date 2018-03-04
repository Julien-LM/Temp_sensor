/* 
 * File:   init.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on November 29, 2017, 3:11 PM
 */

#ifndef INIT_H
#define	INIT_H

#include <xc.h>
#include <stdlib.h>

void configuration_reg(void);

void init_GPIO(void) {
    CPSCON0bits.CPSON = 0;      // CPS module is disabled
    
    TRISCbits.TRISC0 = 0;       // Configurated as an output
    TRISCbits.TRISC1 = 0;       // Configurated as an output
    TRISCbits.TRISC2 = 0;       // Configurated as an output
    TRISCbits.TRISC3 = 0;       // Configurated as an output
    
    ANSELCbits.ANSC0 = 0;       // Digital I/O
    ANSELCbits.ANSC1 = 0;       // Digital I/O
    ANSELCbits.ANSC2 = 0;       // Digital I/O
    ANSELCbits.ANSC3 = 0;       // Digital I/O
}

void init_ADC(void) {
    TRISAbits.TRISA2 = 1;       // Configured as input 
    ANSELAbits.ANSA2 = 1;       // Analog input
    
    ADCON0bits.CHS = 0b00010;   // AN2
    ADCON0bits.ADON = 1;        // ADC is enabled
    
    ADCON1bits.ADFM = 0;        // Left justified. Six Least Significant bits of ADRESL are set to ?0?
    ADCON1bits.ADCS = 0b101;    // FOSC/16
    ADCON1bits.ADNREF = 0;      // V REF - is connected to GND
    ADCON1bits.ADPREF = 0;      // V REF + is connected to VDD
}

void init_OSC(void) {
    OSCCONbits.IRCF = 0b1101;   // 4MHz HF selected
    OSCCONbits.SCS = 2;         // Internal oscillator block
    // XTAL = 500KHz, efficient 125kHz
}

void init_timer1(void) {
    T1CONbits.TMR1ON = 1;       // Enable timer1
    T1CONbits.T1CKPS = 3;       // 1:8 Prescale value
    T1CONbits.TMR1CS = 2;       // Timer1 clock source is Crystal oscillator
    T1CONbits.T1OSCEN = 1;      // Dedicated Timer1 oscillator circuit enable
    T1CONbits.nT1SYNC = 1;      // Do not synchronize external clock input
    
    T1GCONbits.TMR1GE = 0;      // Timer 1 counts regardless of Timer1 gate fct
}

void init_interrupt(void) {
    // General Interruption configuration
    INTCONbits.GIE = 1;        // Enables all active interrupts
    INTCONbits.PEIE = 1;       // Enables all active peripheral interrupts 
    
    // timer Interruption configuration
    PIE1bits.TMR1IE = 1;     // Enable the Timer1 Gate Acquisition interrupt
    
    // USART Receive Interrupt Enable bit
    PIE1bits.RCIE = 1;      // Enables the USART receive interrupt
}

void init_UART(void) {
    TXSTAbits.TX9 = 0;          // Selected 8-bits transmission
    TXSTAbits.TXEN = 1;         // Transmit enable
    TXSTAbits.SYNC = 0;         // Asynchronous mode
    TXSTAbits.BRGH = 1;         // Baud rate high speed
    
    RCSTAbits.SPEN = 1;         // Serial port enable
    RCSTAbits.RX9 = 0;          // Selected 8-bits reception
    RCSTAbits.CREN = 1;         // Continuous Receive Enable bit

    BAUDCONbits.BRG16 = 0;      // 8-bits Baud Rate Generator is used
    BAUDCONbits.SCKP = 0;       // Transmit non-inverted data to the TX/CK pin-
    SPBRG = 12;
    
    // BAUD RATE FORMULAS:
    // SYNC=0, BRG16=1, BRGH=1 => bd=Fosc/[4(SPBRG+1)]
    // 9600 = 500k/[4(X+1)] => SPBRG = 12
    
    ANSELBbits.ANSB5 = 0;   // Digital I/O

    APFCON0bits.RXDTSEL = 0;    // RX/DT function is on RB5
    APFCON0bits.TXCKSEL = 0;    // TX/CK function is on RB7
}

void init_MSSP1(void) {
    // Clock rate BRG
    SSP1ADD = 0x09;
    
    TRISBbits.TRISB4 = 1;   // Set pin to be open-drain
    TRISBbits.TRISB6 = 1;   // Set pin to be open-drain
    //ANSELBbits.ANSB4 = 0;
    
    SSP1STATbits.SMP = 1;   // Slew rate control disabled for standard speed

    SSP1CON1bits.SSPEN = 1; // Enable the serial port and configure SDA1 & SCL1
    SSP1CON1bits.SSPM = 8;  // I2C master mode, clock = Fosc/(4*BRG+1)
    
    SSP1CON2bits.SEN = 0;   // Start condition Idle
    
    configuration_reg();

}

void init_WDT(void) {
    WDTCONbits.WDTPS = 0b01111; // 32s WDT
}

void init(void){
    init_OSC();
    init_timer1();
    init_UART();
    init_interrupt();
    init_GPIO();
    init_MSSP1();
    init_WDT();
    init_ADC();
}

#endif	/* INIT_H */

