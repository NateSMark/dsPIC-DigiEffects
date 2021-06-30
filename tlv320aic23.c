
#include "tlv320aic23.h"
#include "spi.h"

// control register addresses (upper 7 bits)
#define LLI_VOL         0x0000
#define RLI_VOL         0x0100 << 1
#define LHI_VOL         0x0200 << 1
#define RHI_VOL         0x0300 << 1
#define AN_PATH         0x0400 << 1
#define DIG_PATH        0x0500 << 1
#define POW_DOWN        0x0600 << 1
#define DIG_FORMAT      0x0700 << 1
#define FS_CTRL         0x0800 << 1
#define DIG_ACTIVATE    0x0900 << 1
#define REG_RESET       0x0F00 << 1

void _init_codec( void )
{
    
    //
    // dsPIC runs in master mode, word wide comms
    //
    // reset
    spi1_select( true );
    spi1_transmit( ( REG_RESET ) | 0x00 ); 
    spi1_select( false );
    
    // unmute LLI
    spi1_select( true );
    spi1_transmit( ( LLI_VOL ) | 0x17 );
    spi1_select( false );
    
    // mute RLI
    spi1_select( true );
    spi1_transmit( ( RLI_VOL ) | 0x97 );
    spi1_select( false );
    
    // mute LHI
    spi1_select( true );
    spi1_transmit( ( LHI_VOL ) | 0x30 );
    spi1_select( false );
    
    // mute RHI
    spi1_select( true );
    spi1_transmit( ( RHI_VOL ) | 0x30 );
    spi1_select( false );
    
    // analog path ctrl
    spi1_select( true );
    spi1_transmit( ( AN_PATH ) | 0x12 ); 
    spi1_select( false );

    // digital path ctrl
    spi1_select( true );
    spi1_transmit( ( DIG_PATH ) | 0x02 ); 
    spi1_select( false );
    
    // power down ctrl
    spi1_select( true );
    spi1_transmit( ( POW_DOWN ) | 0x02 ); 
    spi1_select( false );
    
    // digital audio interface format
    spi1_select( true );
    spi1_transmit( ( DIG_FORMAT ) | 0x53 ); // tlv is master dsp mode
    spi1_select( false );
    
    // sample rate ctrl
    // 32kHz with 12MHz crystal
    spi1_select( true );
    spi1_transmit( ( FS_CTRL ) | 0x18 ); 
    spi1_select( false );

    // digital interface activation
    spi1_select( true );
    spi1_transmit( ( DIG_ACTIVATE ) | 0x01 ); 
    spi1_select( false );
    
}