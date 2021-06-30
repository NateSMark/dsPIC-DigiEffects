/* 
 * File:   AD73311L_drv.h
 * Author: Nathan
 *
 * Created on November 4, 2019, 12:54 AM
 */

#ifndef AD73311L_DRV_H
#define	AD73311L_DRV_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <dsp.h>

#define Fs   22000

// prototypes
void ad73311_init( bool );
uint16_t read_ctrla( void );
uint16_t read_ctrlb( void );
uint16_t read_ctrlc( void );
uint16_t read_ctrld( void );
uint16_t read_ctrle( void );
uint16_t read_ctrlf( void );
int16_t transmit_sample( int16_t );
void ad73311_reset( void );

#endif	/* AD73311L_DRV_H */

