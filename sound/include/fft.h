#ifndef FFT_H
#define FFT_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define  DIRECT        -1
#define  INVERSE        1

static const float r_coef[14] =
{  
    -1.0000000000000000,  0.0000000000000000,  0.7071067811865475,
    0.9238795325112867,  0.9807852804032304,  0.9951847266721969,
    0.9987954562051724,  0.9996988186962042,  0.9999247018391445,
    0.9999811752826011,  0.9999952938095761,  0.9999988234517018,
    0.9999997058628822,  0.9999999264657178
};
static const float i_coef[14] =
{   
    0.0000000000000000, -1.0000000000000000, -0.7071067811865474,
    -0.3826834323650897, -0.1950903220161282, -0.0980171403295606,
    -0.0490676743274180, -0.0245412285229122, -0.0122715382857199,
    -0.0061358846491544, -0.0030679567629659, -0.0015339801862847,
    -0.0007669903187427, -0.0003834951875714
};

void fft(float *re, float *im, uint16_t N, uint16_t logn, int8_t flag);

void shift(int16_t shift, float *re, float *im, uint16_t n);

#endif