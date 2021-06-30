/* 
 * File:   23LC1024_drv.h
 * Author: Nathan
 *
 * Created on November 6, 2019, 7:11 PM
 */

#ifndef SRAM23LC1024_DRV_H
#define	SRAM23LC1024_DRV_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "spi.h"

// instruction macros
#define WRITE 0x02
#define READ 0x03
#define WRMR 0x01 // write mode register
#define INPUT_BUF 0
#define OUTPUT_BUF 1

// for 32k sized buffers
#define ST_ADDR 0
#define END_ADDR 44300 - 1

void sram_init( void );
void sel_input_buf( bool );
void sel_output_buf( bool );
void write_addr( uint8_t, uint16_t, uint16_t );
void write_buf( uint8_t, uint16_t, uint16_t );
int16_t read_addr( uint8_t, uint16_t );
int16_t read_buf( uint8_t, uint16_t );
void test_sram( void );
uint16_t idx_to_addr( uint16_t );
uint16_t addr_to_idx( uint16_t );

#endif	/* SRAM23LC1024_DRV_H */

