/* 
 * File:   ds1604.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on December 18, 2017, 8:26 AM
 */

#ifndef DS1624_H
#define	DS1624_H

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
    //__delay_ms(200);
    return(0x44);
}

void configuration_reg(void) {
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write(CONTROL_BYTE_WRITE);   // Write control byte + write
    I2C_Master_Write(ACCESS_CONFIG);        // Write access config command
    I2C_Master_Write(0x01);                 // Write config data
    I2C_Master_Stop();                      // Stop condition
    __delay_ms(50);
}

#endif	/* DS1624_H */

