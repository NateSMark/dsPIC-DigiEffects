/* 
 * File:   audio_processing.h
 * Author: Nathan
 *
 * Created on April 8, 2020, 2:58 AM
 */

#ifndef AUDIO_PROCESSING_H
#define	AUDIO_PROCESSING_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <xc.h>
#include <stdint.h>
#include <math.h>
#include "dsp.h"
#include "libq.h"
#include "devboard_utils.h"

#define FS 32000L
#define MAX_MOD_FREQ 800 // Hz
#define MAX_ENV_AMP 4000 // sample value
    
typedef struct biquad_lpf {
    volatile fractional a0;
    volatile fractional a1;
    volatile fractional a2;
    volatile fractional b1_frac;
    volatile fractional b2;
} biquad_lpf_t;

typedef struct svf {
    volatile fractional F_1[2];
    volatile _Q16 Q_1[2];
} svf_t;

typedef struct filter_params {
    volatile _Q16 q16_fc;
    volatile _Q16 q16_q;
} filter_params_t;

typedef struct envelope {
    volatile _Q15 g_a, g_r;
    volatile _Q16 tau_a, tau_r;
    volatile int16_t mag_sig;
    volatile int16_t freq_mod_factor;
    volatile int16_t freq_mod_sat;
    volatile uint16_t threshold;
} envelope_t;

extern biquad_lpf_t bq_lpf;
extern svf_t svf;
extern filter_params_t filter_params;
extern envelope_t envelope;

extern volatile int16_t inbuf[3];
extern volatile int16_t outbuf[2];
extern volatile int16_t envbuf[2];

void _init_processing( void );
void calc_bq_coeffs( void );
void apply_bq_lpf( void );
void apply_svf( char );
void get_envelope( void );
void set_filter_params( void );
int16_t limit( int16_t, int16_t );
    

#ifdef	__cplusplus
}
#endif

#endif	/* AUDIO_PROCESSING_H */

