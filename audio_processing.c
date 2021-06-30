#include <stdbool.h>
#include "audio_processing.h"

register unsigned int regA asm("A");
register unsigned int accum asm("B");

_Q16 q16_pi;
_Q16 q16_fs_by_two;
_Q16 q16_fs;

// envelope temps
volatile _Q15 g;

// biquad temps
volatile _Q16 theta_c;
volatile _Q16 d_lpf;
volatile _Q16 d_by_two;
volatile _Q16 sin_theta_c;
volatile _Q16 cos_theta_c;
volatile _Q16 alpha;
volatile _Q16 beta_num;
volatile _Q16 beta_den;
volatile _Q16 beta;
volatile _Q16 gamma;

// misc
int int_temp;
_Q16 temp;
_Q15 q15_temp;

// input and output buffers
volatile int16_t inbuf[3];
volatile int16_t procbuf[3];
volatile int16_t outbuf[2];

// envelope detector buffer
volatile int16_t envbuf[2];

// SVF buffers
volatile int16_t y_lbuf[2];
volatile int16_t y_bbuf[2];
volatile int16_t y_hbuf[2];

biquad_lpf_t bq_lpf;
svf_t svf;
filter_params_t filter_params;
envelope_t envelope;

void _init_processing( void )
{
    
    q16_pi = 0x3243F;
    q16_fs_by_two = _Q16ftoi((float)(FS/2));
    q16_fs = _Q16ftoi((float)(FS));
    
    // flush buffers
    inbuf[0] = 0;
    inbuf[1] = 0;
    inbuf[2] = 0;
    
    outbuf[0] = 0;
    outbuf[1] = 0;
    
    envbuf[0] = 0;
    envbuf[1] = 0;
    
    y_lbuf[0] = 0;
    y_lbuf[1] = 0;
    y_bbuf[0] = 0;
    y_bbuf[1] = 0;
    y_hbuf[0] = 0;
    y_hbuf[1] = 0;
    
    // set filter/env params not set by knobs
    envelope.freq_mod_sat = 1000;
    
    return;
    
}

void get_envelope( void )
{
    
    envelope.mag_sig = abs(inbuf[0] << 1);
    
    // converting results to _Q15
    envelope.g_a = (_Q16exp(-_Q16reciprocalQ16(_Q16mpy(q16_fs, envelope.tau_a))) >> 1) & 0x7FFF;
    envelope.g_r = (_Q16exp(-_Q16reciprocalQ16(_Q16mpy(q16_fs, envelope.tau_r))) >> 1) & 0x7FFF;
    
    if(envbuf[1] < envelope.mag_sig) {
        g = envelope.g_a;
    } else {
        g = envelope.g_r;
    }
    
    accum = __builtin_clr();
    accum = __builtin_mac(accum, ~g & 0x7FFF, envelope.mag_sig, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL);
    accum = __builtin_mac(accum, g, envbuf[1], NULL, NULL, 0, NULL, NULL, 0, NULL, NULL);
    
    envbuf[1] = envbuf[0];
    
    envbuf[0] = __builtin_sacr(accum, 0);
  
    int_temp = envbuf[0] - envelope.threshold;
    if(int_temp < 0) {
        envelope.freq_mod_factor = 0;
    } else if(int_temp > envelope.freq_mod_sat) {
        envelope.freq_mod_factor = envelope.freq_mod_sat;
    } else {
        envelope.freq_mod_factor = int_temp;
    }
    
    return;
    
}

void set_filter_params( void )
{
    
    // top pot is buf0, middle is buf2, bottom is buf1
    // top pot mods fc, middle q, bottom env thresh
    if(AD1CON1bits.DONE) {
        filter_params.q16_fc = ((int32_t)ADC1BUF0 << 16); // 00C8
        temp = ((int32_t)ADC1BUF2 << 8);
        filter_params.q16_q = temp < 0xB500 ? 0xB500 : temp; // 03A8
        envelope.threshold =  ADC1BUF1; 
        envelope.tau_a = (int32_t)ADC1BUF7; // max
        envelope.tau_r = (int32_t)ADC1BUF4; // max
    }
    
    return;
    
}

/*
 * STATE VARIABLE FILTER
 * 
 * 
 */
void apply_svf( char type )
{
    
    // F_1 shouldn't get above 1 given my range of params, hence Q1.15
    // F_1 = 2sin(pi*fc/fs)
    svf.F_1[1] = svf.F_1[0];
    svf.Q_1[1] = svf.Q_1[0];
    temp = filter_params.q16_fc + ((_Q16)envelope.freq_mod_factor << 16);
    svf.F_1[0] = _Q16sin(_Q16div(_Q16mpy(temp, q16_pi), q16_fs)) & 0x7FFF;
    // Q_1 = 1/q
    svf.Q_1[0] = _Q16reciprocalQ16(filter_params.q16_q);
    
    // y_h(n) = x(n) - y_l(n-1) - Q_1*y_b(n-1)
    y_hbuf[1] = y_hbuf[0]; 
    y_hbuf[0] = inbuf[0] - y_lbuf[1] - (int16_t)(_Q16mpy(svf.Q_1[0], (int32_t)y_bbuf[1] << 16) >> 16);
    
    // y_b(n) = F_1*y_h(n) + y_b(n-1)
    regA = __builtin_clr();
    regA = __builtin_mpy(svf.F_1[0], y_hbuf[0], NULL, NULL, 0, NULL, NULL, 0);
    y_bbuf[1] = y_bbuf[0];
    y_bbuf[0] = __builtin_sacr(regA, 0) + y_bbuf[1];
    
    // y_l(n) = F_1*y_b(n) + y_l(n-1)
    regA = __builtin_clr();
    regA = __builtin_mpy(svf.F_1[0], y_bbuf[0], NULL, NULL, 0, NULL, NULL, 0);
    y_lbuf[1] = y_lbuf[0];
    y_lbuf[0] = __builtin_sacr(regA, 0) + y_lbuf[1];
    
    if(type == 'h') {
        outbuf[0] = y_hbuf[0];
    } else if(type == 'b') {
        outbuf[0] = y_bbuf[0];
    } else {
        outbuf[0] = y_lbuf[0];
    }
    return;
    
}

void calc_bq_coeffs( void )
{
    
    temp = filter_params.q16_fc + ((_Q16)envelope.freq_mod_factor << 16);
    theta_c = _Q16div(_Q16mpy(q16_pi, temp), q16_fs_by_two);
    d_lpf = _Q16reciprocalQ16(filter_params.q16_q);
    
    d_by_two = d_lpf >> 1;
    sin_theta_c = _Q16sin(theta_c);
   
    beta = _Q16div(0x10000 - _Q16mpy(d_by_two, sin_theta_c),
            0x10000 + _Q16mpy(d_by_two, sin_theta_c)) >> 1;
    
    gamma = _Q16mpy((0x8000 + beta), _Q16cos(theta_c));
    
    // converting to Q1.15
    bq_lpf.a1 = ((0x8000 + beta - gamma) >> 1) & 0x7FFF;
    bq_lpf.a0 = bq_lpf.a1 >> 1;
    bq_lpf.a2 = bq_lpf.a0;
    bq_lpf.b1_frac = (gamma & 0x80000000) ? ((gamma >> 1) & 0x7FFF) | 0x8000 :  ((gamma >> 1) & 0x7FFF);
    bq_lpf.b2 = beta & 0x7FFF;
    
    return;
    
}

void apply_bq_lpf( void )
{
    
    regA = __builtin_clr();
    accum = __builtin_clr();
    
    // IIR biquad lowpass
    // y(n) = a0*x(n) + a1*x(n-1) + a2*x(n-2) - b1*y(n-1) - b2*y(n-2)
    accum = __builtin_mac(accum, inbuf[0], bq_lpf.a0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL);
    accum = __builtin_mac(accum, inbuf[1], bq_lpf.a1, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL);
    accum = __builtin_mac(accum, inbuf[2], bq_lpf.a2, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL);
    regA = __builtin_mpy(outbuf[0], bq_lpf.b1_frac, NULL, NULL, 0, NULL, NULL, 0);
    //
    // **temp is Q16, but we want Q15. Perhaps fixing this will improve performance**
    //
    temp = __builtin_sacr(regA, 0);
    temp = (temp & 0x8000) ? (temp << 1) | 0x8000 : (temp << 1) & 0x7FFF;
    accum = __builtin_msc(accum, outbuf[1], bq_lpf.b2, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL);
    
    // shift buffers
    outbuf[1] = outbuf[0];
    inbuf[2] = inbuf[1];
    inbuf[1] = inbuf[0];
    
    outbuf[0] = __builtin_sacr(accum, 0) + temp;
    
    return;
    
}

int16_t limit( int16_t sig, int16_t sat )
{
    
    int16_t ret;
    
    if(sig >= sat) {
        ret = sat;
    } else if(sig <= -sat) {
        ret = -sat;
    } else {
        ret = sig;
    }
    
    return ret;
}