
#include "adc.h"
#include "devboard_utils.h"

void _init_adc1( void )
{
    
    ANSELA = 0x0003;
    ANSELB = 0x0004;
    ANSELC = 0x0005;
    
    AD1CON1 = 0x0008; // simultaneous sampling
    AD1CON2 = 0x0305; // sample CH0-3
    AD1CON3 = 0x000F;
    AD1CON4 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000; // No scan

    /* Assign MUXA inputs */ 
    AD1CHS0bits.CH0SA = 4; // AN4/RB2 for CH0 +ve in **
    AD1CHS0bits.CH0NA = 0; // Vref- for CH0 -ve in
    AD1CHS123bits.CH123SA = 0; // CH1 = AN0/RA0 **; CH2 = AN1/RA1 **; CH3 = AN2/RB0 (N/A debugger) 
    AD1CHS123bits.CH123NA = 0; // Vref- for CH1-3
    
    /* Assign MUXB inputs */ 
    AD1CHS0bits.CH0SB = 8; // AN8/RC2 for CH4 +ve in **
    AD1CHS0bits.CH0NB = 0; // Vref- for CH0 -ve in
    AD1CHS123bits.CH123SB = 1; // CH5 = AN3/RB1 (N/A debugger); CH6 = AN0/RA0; CH7 = AN6/RC0 ** 
    AD1CHS123bits.CH123NB = 0; // Vref- for CH1-3

    AD1CON1bits.ADON = 1;
    _delay_us(20);

}
