/* 
 * 
 * 
 * I/O, oscillator setup and delays
 * 
 * 
 */  

#include <p33EP256GP504.h>
#include <xc.h>
#include "devboard_utils.h"

void _init_io( void ) 
{
    
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
 
    // SPI1 for setup of codec
    TRISAbits.TRISA4 = 0; // SPI1 SDO
    TRISCbits.TRISC3 = 0;
    TRISAbits.TRISA9 = 1; // SPI1 SDI
    TRISCbits.TRISC4 = 0; // general SPI SS
    
    // SPI2 for slave codec operation
    RPOR4bits.RP42R = 0b001000; // SDO2 on RB10
    TRISBbits.TRISB10 = 0;
    RPINR22bits.SCK2R = 0b0101011; // SCK2 (BCLK) input on RB11
    TRISBbits.TRISB11 = 1;
    RPINR22bits.SDI2R = 0b0101100; // SDI2 on RB12
    TRISBbits.TRISB12 = 1;
    RPINR23bits.SS2R = 0b0101101; // SS2 (FrameSync) on RB13
    TRISBbits.TRISB13 = 1; 

    TRISBbits.TRISB9 = 0; // debug LED
    PORTBbits.RB9 = 0;
    
    return;
    
}

void _init_timers( void )
{
    
    // TMR1, setup for interrupt
//    T1CONbits.TON = 0;
//    T1CONbits.TCS = 0;
//    T1CONbits.TGATE = 0;
//    T1CONbits.TCKPS = 0b01;
//    PR1 = 390;
    
    // TMR4&5 for 32bit delays    
    T5CONbits.TON = 0;
    T4CONbits.TON = 0;
    T4CONbits.T32 = 1;
    T4CONbits.TCS = 0;
    T4CONbits.TGATE = 0;
    T4CONbits.TCKPS = 0b00;

    return;
    
}

void _init_POSC( void )
{
    // Configure Oscillator to operate the device at 100Mhz
    // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
    // Fosc= 16M*50/(2*2)=200Mhz for 16M input clock
    PLLFBD = 38;                        // M=50
    CLKDIVbits.PLLPOST = 0;             // N1=2
    CLKDIVbits.PLLPRE = 0;              // N2=2
    OSCTUN = 0;                         // Tune FRC oscillator, if FRC is used

    // Disable Watch Dog Timer
    RCONbits.SWDTEN = 0;

    // Make all the ANx pins as digital pins
    ANSELC = 0;

    // Clock switch to incorporate PLL
    __builtin_write_OSCCONH( 0x03 );

    // Primary osc with PLL (NOSC=0b011)
    __builtin_write_OSCCONL( OSCCON | 0x01 );    // Start clock switching
    while( OSCCONbits.COSC != 0b011 );

    // Wait for Clock switch to occur
    // Wait for PLL to lock
    while( OSCCONbits.LOCK != 1 )
    { };
    
    return;
}

void _init_FRC( void ) 
{
    
    // Configure Oscillator to operate the device at 40Mhz
    // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
    // Fosc= 8M*40/(2*2)=80Mhz for 8M input clock
    PLLFBD = 38;                        // M=40
    CLKDIVbits.PLLPOST = 0;             // N1=2
    CLKDIVbits.PLLPRE = 0;              // N2=2
    OSCTUN = 0;                         // Tune FRC oscillator, if FRC is used

    // Disable Watch Dog Timer
    RCONbits.SWDTEN = 0;

    // Make all the ANx pins as digital pins
    ANSELC = 0;

    // Clock switch to incorporate PLL
    __builtin_write_OSCCONH( 0x01 );

    // FRC with PLL (NOSC=0b001)
    __builtin_write_OSCCONL( OSCCON | 0x01 );    // Start clock switching
    while( OSCCONbits.COSC != 0b001 );

    // Wait for Clock switch to occur
    // Wait for PLL to lock
    while( OSCCONbits.LOCK != 1 )
    { };
    
    return;
    
}

void _delay_us( uint32_t delay )
{
    
    TMR4 = 0x00;
    TMR5 = 0x00;
    T4CONbits.TON = 1;
    while(( ((uint32_t)TMR5 << 16) | TMR4 )  < 71 * delay ) {
        ;
    }
    T4CONbits.TON = 0;
    
    return;
    
}

void _delay_ms( uint32_t delay )
{
    
    TMR4 = 0x00;
    TMR5 = 0x00;
    T4CONbits.TON = 1;
    while(( ((uint32_t)TMR5 << 16) | (uint32_t)TMR4 ) < 71000 * delay ) {
        ;
    }
    T4CONbits.TON = 0;
    
    return;
    
}

void _delay_inst( uint32_t delay )
{
    
    TMR4 = 0x00;
    TMR5 = 0x00;
    T4CONbits.TON = 1;
    while(( ((uint32_t)TMR5 << 16) | (uint32_t)TMR4 ) < 1 * delay ) {
        ;
    }
    T4CONbits.TON = 0;
    
    return;
    
}