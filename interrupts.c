
#include "interrupts.h"
#include "spi.h"
#include "devboard_utils.h"
#include "audio_processing.h"

volatile int16_t audio_in;
volatile int16_t audio_out;
volatile int i;
int32_t _int_temp;

void _enable_interrupts( void )
{
    
    audio_in = 0;
    audio_out = 0;
    
    INTCON2bits.GIE = 1;
    
    // interrupt nesting
    INTCON1bits.NSTDIS = 0;

    return;
    
}

void _disable_interrupts( void )
{
    
    INTCON2bits.GIE = 0;

    return;
    
}

void _init_T1_interrupt( void )
{
    
    // TMR1
    IPC0bits.T1IP = 2;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TON = 1;
    
    return;
    
}

void _init_SPI1Err_interrupt( void )
{
    // SPI1 Error
    IPC2bits.SPI1EIP = 2;
    IFS0bits.SPI1EIF = 0; 
    IEC0bits.SPI1EIE = 1;
    
    return;
    
}

void _init_SPI2Err_interrupt( void )
{
    // SPI1 Error
    IPC8bits.SPI2EIP = 2;
    IFS2bits.SPI2EIF = 0; 
    IEC2bits.SPI2EIE = 1;
    
    return;
    
}

void _init_CN_interrupt( void )
{
    
    // Change Notification
    IPC4bits.CNIP = 3;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    CNENBbits.CNIEB13 = 1;
    
    return;
    
}


void _ISRFAST __attribute__( (interrupt, no_auto_psv) ) _CNInterrupt( void )
{
    
    PORTBbits.RB9 = 1;
    
    // set filter parameters from adc readings
    set_filter_params();
    
    // start sampling
    AD1CON1bits.SAMP = 1;

    // get filter modulation from envelope
    get_envelope();
    
    /*
     * BIQUAD LOWPASS
     * 
     * slightly too slow for stability 
     * 
     */
//    calc_bq_coeffs();
//    apply_bq_lpf();
    
    // start conversions
    AD1CON1bits.SAMP = 0;
    
    /*
     * STATE VARIABLE FILTER
     * 
     * pass 'l' for lowpass, 'b' for bandpass, or 'h' for highpass tap
     * 
     */
    apply_svf( 'b' );
    
    // deliver processed sample
//    audio_out = limit(outbuf[0], 8000);
    inbuf[0] = spi2_transmit( outbuf[0] );
    
    PORTBbits.RB9 = 0;
    
    IFS1bits.CNIF = 0;

}

void _ISR __attribute__( (interrupt, no_auto_psv) ) _MI2C1Interrupt( void )
{

    IFS1bits.MI2C1IF = 0;   //Clear the DMA0 Interrupt Flag;
    
}

void _ISRFAST __attribute__( (interrupt, no_auto_psv) ) _SPI1ErrInterrupt(void) 
{
    
    SPI1STATbits.SPIROV = 0;
    IFS0bits.SPI1EIF = 0;

}

void _ISRFAST __attribute__( (interrupt, no_auto_psv) ) _SPI2ErrInterrupt(void) 
{
    
    SPI2STATbits.SPIROV = 0;
    IFS2bits.SPI2EIF = 0;

}

void _ISR __attribute__( (interrupt, no_auto_psv) ) _T1Interrupt( void )
{
            
    IFS0bits.T1IF = 0;
    
}