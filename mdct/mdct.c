#include "mdct.h"

void mdct(float* in, float** out, uint32_t length, uint32_t framelength){
    uint32_t N = framelength / 2;
    uint32_t frames = length / N - 1;
    float coef = sqrt(2.0 / N);
    uint32_t n;
    float sum;
    for(uint32_t i=0; i<frames; i++){
        for(uint32_t k=0; k<N; k++){
            sum = 0;
            for(n=0; n<framelength; n++){
                sum += in[i*N + n] * \
                       cos(PI/N * (n + 0.5 + N/2) * (k + 0.5)) * \
                       cosine[n];
            }
            out[i][k] = sum * coef;
        }
    }
}

void imdct(float** in, float* out, uint32_t length, uint32_t framelength){
    uint32_t N = framelength / 2;
    uint32_t frames = length / N - 1;
    float coef = sqrt(2.0 / N);
    uint32_t k;
    float sum;
    for(uint32_t i=0; i<frames; i++){
        for(uint32_t n=0; n<framelength; n++){
            sum = 0;
            for(k=0; k<N; k++){
                sum += in[i][k] * \
                       cos(PI/N * (n + 0.5 + N/2) * (k + 0.5));
            }
            out[i*N + n] += sum * coef * cosine[n];
        }
    }
}
