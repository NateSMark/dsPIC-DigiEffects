/* 
 * File:   devboard_utils.h
 * Author: Nathan
 *
 * Created on February 9, 2020, 11:45 PM
 */

#include <xc.h>
#include <stdint.h>

#ifndef DEVBOARD_UTILS_H
#define	DEVBOARD_UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

    void _init_io( void );
    void _init_FRC( void );
    void _init_POSC( void );
    void _init_timers( void );
    void _delay_us( uint32_t );
    void _delay_ms( uint32_t );
    void _delay_inst( uint32_t );


#ifdef	__cplusplus
}
#endif

#endif	/* DEVBOARD_UTILS_H */

