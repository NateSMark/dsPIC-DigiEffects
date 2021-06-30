/*
 * File:   main.c
 * Author: Nathan
 *
 * Created on February 2, 2020, 1:52 AM
 */

//p33EP256GP504.h
#include <xc.h> // #include <p33EP256GP504.h>
#include <libq.h>
#include <stdint.h>
#include "tlv320aic23.h"
#include "interrupts.h"
#include "devboard_utils.h"
#include "adc.h"
#include "spi.h"
#include "audio_processing.h"

#define POSC_CLK

#if __XC16_VERSION < 1011
#warning "Please upgrade to XC16 v1.11 or newer."
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Macros for Configuration Fuse Registers:
// *****************************************************************************
// *****************************************************************************
/* Invoke macros to set up device configuration fuse registers.The fuses will
   select the oscillator source, power-up timers, watch-dog timers etc. The
   macros are defined within the device header files. The configuration fuse
   registers reside in Flash memory.
 */
// DSPIC33EP256GP506 Configuration Bit Settings
// 'C' source line config statements
// FICD
#pragma config ICS = PGD3           // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF          // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF        // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF        // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25       // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768    // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128       // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON          // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF         // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#ifdef FRC_CLK
#pragma config POSCMD = NONE        // No primary oscillator
#pragma config OSCIOFNC = ON        // OSC2 Pin Function bit (OSC2 is IO)
#pragma config IOL1WAY = OFF        // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD       // Clock Switching Mode bits (Clock switching enabled, Fail-safe Clock Monitor disabled)
#endif

#ifdef POSC_CLK
#pragma config POSCMD = HS          // external high speed oscillator
#pragma config OSCIOFNC = OFF       // OSC2 Pin Function bit (OSC2 is OSC2)
#pragma config IOL1WAY = OFF        // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD       // Clock Switching Mode bits (Clock switching enabled, Fail-safe Clock Monitor disabled)
#endif

// FOSCSEL
#pragma config FNOSC = FRC          // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config IESO = OFF           // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)


// FGS
#pragma config GWRP = OFF           // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF            // General Segment Code-Protect bit (General Segment Code protect is Disabled)


// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************



int main(void) 
{
    
    // Core control register to ensure accumulator, 
    // w registers, and data space are saturating
    CORCONbits.ACCSAT = 1;
    CORCONbits.SATA = 1;
    CORCONbits.SATB = 1;
    CORCONbits.SATDW = 1;
    
    
    // initialize primary oscillator
#ifdef FRC_CLK
    _init_FRC();
#endif
    
#ifdef POSC_CLK
    _init_POSC();
#endif
    
    _enable_interrupts();
    _init_SPI2Err_interrupt();
    
    _init_timers();
    _init_io();
    _init_processing();
    
    _spi1_master_init();
    _init_codec();
    _spi1_disable();
    _init_adc1();
    _init_CN_interrupt();
    _spi2_slave_init();
    
    
    while( 1 )
    {
        ;
    }
    
}





