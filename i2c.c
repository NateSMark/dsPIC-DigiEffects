#include "i2c.h"

void _init_i2c1( void )
{
    
    ODCCbits.ODCC4 = 0;
    ODCCbits.ODCC5 = 0;

    I2C1CONbits.A10M = 0;
    I2C1CONbits.SCLREL = 1;
    I2C1BRG = BRGVAL;

    I2C1ADD = 0;
    I2C1MSK = 0;

    I2C1CONbits.I2CEN = 1;
//    IEC1bits.MI2C1IE = 1;
//    IFS1bits.MI2C1IF = 0;
    
    return;
    
}

uint8_t i2c1_send_data( uint8_t data, bool _start, bool _stop )
{
    uint8_t retrys;

    if(_start)
        I2C1CONbits.SEN = 1;
    
    I2C1TRN = data;
    while(I2C1STATbits.TRSTAT);
    
    if(I2C1STATbits.ACKSTAT == 1) {
        for(retrys=0; retrys < 3; retrys++) {
            if(i2c1_send_data(data, _start, _stop)){
                break;
            }
        if(_stop)
        I2C1CONbits.PEN = 1;

        return 0;
            
        }
    }
    
    if(_stop)
        I2C1CONbits.PEN = 1;
        
    return 1;
 
}

uint16_t i2c1_recieve_data( void ) {
    return 0;
};
