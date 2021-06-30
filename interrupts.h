/* 
 * File:   interrupts.h
 * Author: Nathan
 *
 * Created on February 15, 2020, 12:17 AM
 */

#include <xc.h>
#include <stdint.h>
#include <libq.h>

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif

void _enable_interrupts( void );
void _disable_interrupts( void );
void _init_CN_interrupt( void );
void _init_T1_interrupt( void );
void _init_U1_interrupt( void );
void _init_SPI1Err_interrupt( void );
void _init_SPI2Err_interrupt( void );


#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPTS_H */

