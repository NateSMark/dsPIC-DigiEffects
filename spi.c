
/*! @file   spi1drv.c
 *  @brief  API for SPI initialization and data transmission.
 * 
 * 
 *  @author N.Mark 
 *  @date   September 06, 2019
 *  @bug    No known bugs
 * 
 *  @VERSION: 1.1
 */

#include <p33EP256GP504.h>

#include "spi.h"

void _spi1_master_init( void ) 
{

    SPI1BUF = 0;
    
    SPI1STAT = 0x0;    // disable the SPI module (just in case)
    SPI1CON1 = 0x04FA; /* 0000_0100_0000_0000
                        * UNIMPLEMENTED<2:0>: 000
                        * DISSCK: 0 - Internal SPI clk enabled
                        * DISSDO: 0 - SDO in use by module
                        * MODE16: 1 = word-wide communication
                        * SMP: 0 - Data sampled at middle of data
                        * CKE: 0 - Output data changes on idle to active clk state
                        * SSEN: 0 - SS1 controlled by port
                        * CKP: 1 - Idle clk state is High framesync N/A
                        * MSTEN: 1 - Master mode
                        * SPRE<2:0>: 110 - 4:1 secondary prescale
                        * PPRE<1:0>: 10 - 8:1 primary prescale
                        */
    
    SPI1CON2bits.FRMEN = 0;          // FRAMEN = 0, SPIFSD = 0,
    SPI1CON2bits.SPIFSD = 0;
    SPI1CON2bits.FRMPOL = 0;         //Polarity of Frame sync pulse
    
    SPI1STAT = 0x8000; /* 1000_0000_0000_0000
                        * SPIEN: 1 - Enable SPI1
                        * UNIMPLEMENTED: 0
                        * SPISIDL: 0 - Idle mode disregarded
                        * UNIMPLEMENTED<1:0>: 00
                        * SPIBEC<2:0>: 000 - Pending SPI transfers
                        * SRMPT: 0 - SPI1 empty and ready-to-send
                        * SPITBF: 0 - TX buffer empty
                        * SPIRBF: 0 - RX buffer empty
                        */
    
    return;
    
}

void _spi2_master_init( void ) 
{

    SPI2BUF = 0;
    
    SPI2STAT = 0x0;    // disable the SPI module (just in case)
    SPI2CON1 = 0x04FA; /* 0000_0100_0000_0000
                        * UNIMPLEMENTED<2:0>: 000
                        * DISSCK: 0 - Internal SPI clk enabled
                        * DISSDO: 0 - SDO in use by module
                        * MODE16: 1 = word-wide communication
                        * SMP: 0 - Data sampled at middle of data
                        * CKE: 0 - Output data changes on idle to active clk state
                        * SSEN: 0 - N/A Master
                        * CKP: 0 - Idle clk state is low N/A framesync
                        * MSTEN: 1 - Master mode
                        * SPRE<2:0>: 110 - 4:1 secondary prescale
                        * PPRE<1:0>: 10 - 8:1 primary prescale
                        */
    
    SPI2CON2bits.FRMEN = 1;          // FRAMEN = 0, SPIFSD = 0,
    SPI2CON2bits.SPIFSD = 0;
    SPI2CON2bits.FRMPOL = 1;         //Polarity of Frame sync pulse
    SPI2CON2bits.FRMDLY = 0;
    
    SPI2STATbits.SPIEN = 1; // enable SPI2
    
    return;
    
}

void _spi1_disable( void )
{
    
    SPI1STAT = 0x00; // disable spi1
    return;
    
}

void _spi1_slave_init( void ) 
{
    
    SPI1BUF = 0;
    
    SPI1STAT = 0x0;    // disable the SPI module (just in case)
    SPI1CON1 = 0x0440; /* 0000_0000_0000_0000
                        * UNIMPLEMENTED<2:0>: 000
                        * DISSCK: 0 - Internal SPI clk enabled N/A
                        * DISSDO: 0 - SDO in use by module
                        * MODE16: 1 = word-wide communication
                        * SMP: 0 - Data sampled at middle of data N/A
                        * CKE: 0 - Output data changes on idle to active clk state
                        * SSEN: 0 - SS1 controlled by port
                        * CKP: 1 - Idle clk state is low
                        * MSTEN: 0 - Slave mode
                        * SPRE<2:0>: 000 - N/A
                        * PPRE<1:0>: 00 - N/A
                        */
    
    SPI1CON2bits.FRMEN = 1;          // FRAMEN = 0, SPIFSD = 0,
    SPI1CON2bits.SPIFSD = 1;
    SPI1CON2bits.FRMPOL = 1;         //Polarity of Frame sync pulse
    
    SPI1STAT = 0x8000; /* 1000_0000_0000_0000
                        * SPIEN: 1 - Enable SPI1
                        * UNIMPLEMENTED: 0
                        * SPISIDL: 0 - Idle mode disregarded
                        * UNIMPLEMENTED<1:0>: 00
                        * SPIBEC<2:0>: 000 - Pending SPI transfers
                        * SRMPT: 0 - SPI1 empty and ready-to-send
                        * SPITBF: 0 - TX buffer empty
                        * SPIRBF: 0 - RX buffer empty
                        */
    
    return;
    
}

void _spi2_slave_init( void ) 
{
    
    SPI2BUF = 0;
    
    SPI2STAT = 0x0;    // disable the SPI module (just in case)
    SPI2CON1 = 0x0440; /* 0000_0000_0000_0000
                        * UNIMPLEMENTED<2:0>: 000
                        * DISSCK: 0 - Internal SPI clk enabled N/A
                        * DISSDO: 0 - SDO in use by module
                        * MODE16: 1 = word-wide communication
                        * SMP: 0 - Data sampled at middle of data N/A
                        * CKE: 0 - Output data changes on idle to active clk state
                        * SSEN: 0 - SS1 controlled by port
                        * CKP: 1 - Idle clk state is low
                        * MSTEN: 0 - Slave mode
                        * SPRE<2:0>: 000 - N/A
                        * PPRE<1:0>: 00 - N/A
                        */
    
    SPI2CON2bits.FRMEN = 1;
    SPI2CON2bits.SPIFSD = 1;
    SPI2CON2bits.FRMPOL = 1; 
    SPI2CON2bits.FRMDLY = 0; 
    
    SPI2STAT = 0x8000; /* 1000_0000_0000_0000
                        * SPIEN: 1 - Enable SPI1
                        * UNIMPLEMENTED: 0
                        * SPISIDL: 0 - Idle mode disregarded
                        * UNIMPLEMENTED<1:0>: 00
                        * SPIBEC<2:0>: 000 - Pending SPI transfers
                        * SRMPT: 0 - SPI1 empty and ready-to-send
                        * SPITBF: 0 - TX buffer empty
                        * SPIRBF: 0 - RX buffer empty
                        */
    
    return;
    
}

int16_t spi1_transmit( int16_t data ) 
{
    
    SPI1BUF = data;                          // write the data out to SPI1TX
    
    while( !SPI1STATbits.SPIRBF )            // wait for the data to be sent out
        ;
    
    return SPI1BUF;
    
}

inline int16_t spi2_transmit( int16_t data ) 
{
    
    SPI2BUF = data;                          // write the data out to SPI2TX
    
    while( !SPI2STATbits.SPIRBF )            // wait for the data to be sent out
        ;
    
    return SPI2BUF;
    
}

void spi1_select( bool sel )
{
    // ss active low
    if (sel) {
        PORTCbits.RC4 = 0;
    } else {
        PORTCbits.RC4 = 1;
    }
    
    return;
    
}