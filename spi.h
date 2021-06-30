/* 
 * File:   spi1drv.h
 * Author: Nathan
 *
 * Created on September 7, 2019, 12:28 AM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef SPI1DRV_H
#define	SPI1DRV_H

void _spi1_master_init( void );
void _spi1_slave_init( void );
void _spi2_master_init( void );
void _spi2_slave_init( void );
void _spi1_disable( void );
int16_t spi1_transmit( int16_t );
inline int16_t spi2_transmit( int16_t );
void spi1_select( bool );


#endif	/* SPI_DRIVER_H */

