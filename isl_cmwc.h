/* 
 isl_cmwc.h - v1.0.0
 public domain library with CMWC4096 PRNG by George Marsaglia 
 taken from libtcod sources
 no warranty implied; use at your own risk

 author: Ilya Kolbin (iskolbin@gmail.com)
 url: github.com/iskolbin/isl_cmwc

 Do this:
		#define ISL_CMWC_IMPLEMENTATION
 before you include this file in *one* C or C++ file to create the implementation.
 
 LICENSE

 This software is dual-licensed to the public domain and under the following
 license: you are granted a perpetual, irrevocable license to copy, modify,
 publish, and distribute this file as you see fit.
*/

#ifndef ISL_CMWC_4096_H_
#define ISL_CMWC_4096_H_

#include <stdint.h>

#ifndef ISLCMWC_STATIC
#define ISLCMWC_DEF static
#else
#define ISLCMWC_DEF extern
#endif

typedef struct {
	uint32_t Q[4096];
	uint32_t c;
	uint32_t i;
} islcmwc_state;

#ifdef __cplusplus
extern "C" {
#endif

ISLCMWC_DEF void islcmwc_srand( islcmwc_state *self, uint32_t seed );
ISLCMWC_DEF uint32_t islcmwc_rand( islcmwc_state *self );

#ifdef __cplusplus
}
#endif

void islcmwc_srand( islcmwc_state *self, uint32_t seed ) {
	int i;
	uint32_t s = seed;
	for ( int i = 0; i < 4096; i++) {
		self->Q[i] = s = (s * 1103515245) + 12345; /* glibc LCG */
	}
	self->c = ((s * 1103515245) + 12345) % 809430660;
	self->i = 0;
}

uint32_t islcmwc_rand( islcmwc_state *self ) {
	self->i = ( self->i + 1 ) & 4095;
	uint64_t t = 18782ULL * self->Q[self->i] + self->c;
	self->c = t >> 32;
	uint32_t x = (uint32_t)( t + self->c );

	if ( x < self->c ) {
		x++;
		self->c++;
	}

	if( x + 1 == 0) {
		self->c++;
		x=0;
	}
	
	self->Q[self->i] = 0xfffffffe - x;
	return self->Q[self->i];
}

#endif
