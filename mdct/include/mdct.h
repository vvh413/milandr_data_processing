#ifndef MDCT_H
#define MDCT_h

#include <math.h>
#include <stdint.h>
#include "pre_compute.h"

#define PI 3.14159265

void mdct(float* in, float** out, uint32_t length, uint32_t framelength);
void imdct(float** in, float* out, uint32_t length, uint32_t framelength);


#endif
