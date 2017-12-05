/* 
 * File:   constants.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on November 29, 2017, 3:10 PM
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#define RECEPTION_BUFFER_SIZE   20

// ASCII definition
#define END_OF_TRANSMIT         0x34
#define START_OF_TEXT           0x32
#define ACKNOWLEDGE             0x36
#define LINE_FEED               0x0A
#define CARRIAGE_RETURN         0x0D
#define NEG_ACKNOWLEDGE         0x35
#define BACKSPACE               0x08

// Errors codes
#define WRONG_ARGUMENTS         0x41
#define UNKNOWN_COMMAND         0x42
#define BUFFER_OVERFLOW         0x43
#define DEVICE_BUSY             0x44
#define FRAMING_ERROR           0x45
#define OVERRUN_ERROR           0x46

// Communication Protocol
#define GET_TEMP                0x31
#define GET_TIME                0x38
#define SET_TIME                0x39
#define CONFIGURE_SENSOR        0x20
#define CLEAN_DATA              0x21
#define GET_DATA_NUMBER         0x22

// Commands args size
#define GET_TEMP_SIZE            0x00
#define GET_TIME_SIZE            0x00
#define SET_TIME_SIZE            0x06
#define CONFIGURE_SENSOR_SIZE    0x02
#define CLEAN_DATA_SIZE          0x00
#define GET_DATA_NUMBER_SIZE     0x00

#define LED_RED     PORTBbits.RB4
#define LED_ORANGE  PORTBbits.RB5
#define LED_GREEN   PORTBbits.RB6
#define LED_BLUE    PORTBbits.RB7

typedef struct Time_struct Time;
struct Time_struct {
    unsigned short years;
    unsigned char months;
    unsigned char days;
    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;
};

// PIC16F1829 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF      // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#endif	/* CONSTANTS_H */

