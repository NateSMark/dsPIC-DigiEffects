/*
 * File:   AD73311L_drv.c
 * Author: Nathan
 *
 * Created on November 4, 2019, 12:55 AM
 */

#include "AD73311L_drv.h"
#include "spi1drv.h"

extern void delay( uint16_t );

// Macros
#define BYTES_TO_WORD(byte_upper, byte_lower) (( (((uint16_t)(byte_upper) << 8) & 0xFF00) + ((uint16_t)(byte_lower) & 0x00FF) )) 

uint16_t init_routine[7];

void ad73311_init( bool test )
{
    
    /*
     *
     * This register, along with the setup of the MCLCK (i.e. Fcy in this case,
     * output from the OSC2 pin, determined by FRC w/ PLL from dsPIC), determine
     * the effective sample rate. Calculations for Fcy can be found in the main
     * function in main.c. For Fcy = 11MHz / 256 = 44.3kHz XXX not accurate
     * 
     */
    
    uint8_t ctrl_byte = 0x84;
    uint8_t data_byte = 0x00; /* Control register E
                               * 
                               * DAC advance off; don't bypass interpolator 
                               * 
                               * 
                               */
    init_routine[0] = (BYTES_TO_WORD(ctrl_byte, data_byte));
    
    ctrl_byte = 0x83;
    data_byte = 0x04;         /* Control register D
                               * 
                               * OGS 
                               * IGS
                               * Mute
                               * 
                               */
    init_routine[2] = (BYTES_TO_WORD(ctrl_byte, data_byte));
    
    ctrl_byte = 0x85;
    data_byte = 0x20;         /* Control register F
                               * 
                               * single-ended input
                               * 
                               */
    init_routine[1] = (BYTES_TO_WORD(ctrl_byte, data_byte));
              
    ctrl_byte = 0x82;
    data_byte = 0x09;         /* Control register C
                               * 
                               * Power on; power to ADC & DAC, REF on
                               * 
                               */
    init_routine[3] = (BYTES_TO_WORD(ctrl_byte, data_byte));
    
    ctrl_byte = 0x81;
    data_byte = 0xBE;        /* Control register B
                               * 
                               * MCLK/1
                               * SCLCK = MCLCK
                               * Sample rate divider => SCLCK/256
                               * Frame Syncs are generated every
                               * 256th master clock edge 
                               * 
                               */
    init_routine[4] = (BYTES_TO_WORD(ctrl_byte, data_byte));
    
    if (!test) {
        ctrl_byte = 0x80; 
        data_byte = 0x01;     /* Control register A
                               * 
                               * Data Mode
                               * 
                               */  
        init_routine[5] = (BYTES_TO_WORD(ctrl_byte, data_byte));
    }
    
    SPI1BUF = init_routine[0];
    
    // initialize chip from reset
    PORTBbits.RB14 = 1;
    
    while( !SPI1STATbits.SPIRBF );
    
    transmit_sample( init_routine[1] );
    transmit_sample( init_routine[2] );
    transmit_sample( init_routine[3] );
    transmit_sample( init_routine[4] );
    

    if (!test) {
        transmit_sample( init_routine[5] );
    }

}

int16_t transmit_sample( int16_t data )
{
    
    SPI1BUF = data;
    
    while( !PORTBbits.RB15 );
    
    while( !SPI1STATbits.SPIRBF );            // wait for the data to be sent out 
    
    return SPI1BUF;
    
}

/*
 AD73311L Control register reads
 */

uint16_t read_ctrla ( void )
{
    
    uint16_t reg_data;
    
    reg_data = transmit_sample(0xC0FF);
    
    return reg_data;
    
}

uint16_t read_ctrlb ( void )
{
    
    uint16_t reg_data;
    
    reg_data = transmit_sample(0xC1FF);
    
    return reg_data;
    
}

uint16_t read_ctrlc ( void )
{
    
    uint16_t reg_data;
    
    reg_data = transmit_sample(0xC2FF);
    
    return reg_data;
    
}

uint16_t read_ctrld ( void )
{
    
    uint16_t reg_data;
    
    reg_data = transmit_sample(0xC3FF);
    
    return reg_data;
    
}

uint16_t read_ctrle ( void )
{
    
    uint16_t reg_data;
    
    reg_data = transmit_sample(0xC4FF);
    
    return reg_data;
    
}

uint16_t read_ctrlf ( void )
{
    
    uint16_t reg_data;
    
    reg_data = transmit_sample(0xC5FF);
    
    return reg_data;
    
}


