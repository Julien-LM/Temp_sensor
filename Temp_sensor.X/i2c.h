/* 
 * File:   i2c.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on December 11, 2017, 2:42 PM
 */

#ifndef I2C_H
#define	I2C_H

#define _XTAL_FREQ 4000000


void I2C_Master_Wait()
{
  while (SSP1STATbits.R_nW || (SSP1CON2 & 0x1F));
}

void I2C_Master_Wait_Ack(void) {
    while(SSP1CON2bits.ACKSTAT) {
        LED_RED = 1;
    }
    LED_RED = 0;
}

void I2C_Master_Start()
{
    SSP1CON2bits.SEN = 1;
    while(SSP1CON2bits.SEN);
}

void I2C_Master_RepeatedStart()
{
    SSP1CON2bits.RSEN = 1;
    while(SSP1CON2bits.RSEN);
}

void I2C_Master_Stop()
{
    SSP1CON2bits.PEN = 1;
    while(SSP1CON2bits.PEN);
    __delay_ms(50);
}

void I2C_Master_Ack(void) {
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
    while(SSP1CON2bits.ACKEN);
}

void I2C_Master_nAck(void) {
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
    while(SSP1CON2bits.ACKEN);
}

void I2C_Master_Write(unsigned data)
{
  SSP1BUF = data;
  while(SSP1STATbits.BF);
  I2C_Master_Wait();
  I2C_Master_Wait_Ack();
}

unsigned short I2C_Master_Read(void)
{
  unsigned short temp;
  SSP1CON2bits.RCEN = 1;
  while(!SSP1STATbits.BF);
  temp = SSP1BUF;
  I2C_Master_Wait();
  return temp;
}

void read_temp(char *temp) {
    
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write(CONTROL_BYTE_WRITE);   // Write control byte + write
    I2C_Master_Write(READ_TEMP);            // Write read temp command
    I2C_Master_RepeatedStart();             // Start condition
    I2C_Master_Write(CONTROL_BYTE_READ);    // Write control byte + read
    temp[0] = I2C_Master_Read();
    I2C_Master_Ack();
    temp[1] = I2C_Master_Read();
    I2C_Master_nAck();
    I2C_Master_Stop();                      // Stop condition
}

char start_convert(void) {
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write(CONTROL_BYTE_WRITE);   // Write control byte + write
    I2C_Master_Write(START_CONVERT);        // Write start convert command
    I2C_Master_Stop();                      // Stop condition
    return(0x44);
}

char configuration_reg(void) {
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write(CONTROL_BYTE_WRITE);   // Write control byte + write
    I2C_Master_Write(ACCESS_CONFIG);        // Write access config command
    I2C_Master_Write(0x00);                 // Write config data
    I2C_Master_Stop();                      // Stop condition
    return(0x56);
}

#endif	/* I2C_H */

