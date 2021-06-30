#include "sram23lc1024.h"

extern void delay( uint16_t );

// probably not necessary... default mode is sequential
void sram_init( void )
{
    
    // init input_mem to sequential mode
    sel_input_buf(true);
    spi2_transmit(WRMR);
    spi2_transmit(0x40);
    sel_input_buf(false);
    
    // init output_mem to sequential mode
    sel_output_buf(true);
    spi2_transmit(WRMR);
    spi2_transmit(0x40);
    sel_output_buf(false);
    
    // dummy reads to init
    read_buf( INPUT_BUF, 0 );
    read_buf( OUTPUT_BUF, 0 );
    
}

void write_buf( uint8_t io, uint16_t idx, uint16_t data ) {
    
    uint16_t addr = idx_to_addr(idx);
    write_addr(io, addr, data);
    
}

int16_t read_buf( uint8_t io, uint16_t idx ) {
    
    uint16_t addr = idx_to_addr(idx);
    return read_addr(io, addr);
    
}

void write_addr( uint8_t io, uint16_t addr, uint16_t data )
{
    
    int16_t temp;
    
    temp = SPI2BUF;
    
    
    if (io == OUTPUT_BUF) {
        
        sel_output_buf(true);
        spi2_wonly_transmit(((uint16_t)(WRITE) << 8) & 0xFF00);
        spi2_wonly_transmit(addr);
        spi2_wonly_transmit(data);
        delay(1);
        sel_output_buf(false);
         
    } else if (io == INPUT_BUF) {
        sel_input_buf(true);
        spi2_wonly_transmit(((uint16_t)(WRITE) << 8) & 0xFF00);
        spi2_wonly_transmit(addr);
        spi2_wonly_transmit(data);
        delay(1);
        sel_input_buf(false);

    }
    
}

int16_t read_addr( uint8_t io, uint16_t addr )
{
    
    int16_t data;
    
    if (io == OUTPUT_BUF) {

        sel_output_buf(true);
        spi1_transmit(((uint16_t)(READ) << 8) & 0xFF00);
        spi1_transmit(addr);
        data = spi1_transmit(0);
        delay(1);
        sel_output_buf(false);
    
    } else if (io == INPUT_BUF) {
        
        sel_input_buf(true);
        spi1_transmit(((uint16_t)(READ) << 8) & 0xFF00);
        spi1_transmit(addr);
        data = spi1_transmit(0);
        delay(1);
        sel_input_buf(false);
        
    } else {
        return 0;
    }
    
    return data;
    
}

void sel_input_buf( bool sel )
{
    spi1_select(INPUT_BUF, sel);
}

void sel_output_buf( bool sel )
{
    spi1_select(OUTPUT_BUF, sel);
}

void test_sram( void )
{

    sel_output_buf(true);
    spi1_transmit(WRITE);
    spi1_transmit(0);
    spi1_transmit(0);
    spi1_transmit(0);
    spi1_transmit(0xFF);

    spi1_transmit(READ);
    spi1_transmit(0);
    spi1_transmit(0);
    spi1_transmit(0);
    spi1_transmit(0);
    sel_output_buf(false);
    
    sel_input_buf(true);
    spi1_transmit(WRITE);
    spi1_transmit(0);
    spi1_transmit(0);
    spi1_transmit(0);
    spi1_transmit(0xFF);

    spi1_transmit(READ);
    spi1_transmit(0);
    spi1_transmit(0);
    spi1_transmit(0);
    spi1_transmit(0);
    sel_input_buf(false);
    
}

// Want to make accessing mem locations like indexes of array, but mem
// locations are byte addressed and samples are 2 bytes. So, if we want to
// grab the nth sample, we want address n*2

uint16_t addr_to_idx( uint16_t addr )
{
    
    return addr >> 1;
    
}

uint16_t idx_to_addr( uint16_t idx )
{
    
    return idx << 1;
    
}
    
