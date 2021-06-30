/* 
 * File:   i2cdrv.h
 * Author: Nathan
 *
 * Created on February 20, 2020, 11:18 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define FCY 40000000
#define FSCL 13158
#define BAUDRATE    115200
#define BRGVAL      300 // ( (1 / FSCL - 120) * FCY / 2 ) - 2

// Macros for I2C1 pins
#define I2C_CLOCK   		PORTCbits.RC5
#define I2C_DATA    		PORTCbits.RC4

#ifndef I2CDRV_H
#define	I2CDRV_H

#ifdef	__cplusplus
extern "C" {
#endif

void _init_i2c1( void );
uint8_t i2c1_send_data( uint8_t, bool, bool );
uint16_t i2c1_recieve_data( void );

#ifdef	__cplusplus
}
#endif

#endif	/* I2CDRV_H */

