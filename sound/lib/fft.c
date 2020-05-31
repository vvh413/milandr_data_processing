#include "fft.h"

void fft(float *re, float *im, uint16_t N, uint16_t logn, int8_t flag){
    uint16_t i, j, n, k, io, ie, in, nn;
    float ru, iu, rtp, itp, rtq, itq, rw, iw, sr;
    
    nn = N >> 1;
    ie = N;
    for(n=1; n<=logn; n++){
        rw = r_coef[logn - n];
        iw = i_coef[logn - n];
        if(flag == INVERSE) iw = -iw;
        in = ie >> 1;
        ru = 1.0;
        iu = 0.0;
        for(j=0; j<in; j++){
            for(i=j; i<N; i+=ie){
                io = i + in;
                rtp = re[i] + re[io];
                itp = im[i] + im[io];
                rtq = re[i] - re[io];
                itq = im[i] - im[io];
                re[io] = rtq * ru - itq * iu;
                im[io] = itq * ru + rtq * iu;
                re[i] = rtp;
                im[i] = itp;
            }
            sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }

        ie >>= 1;
    }

    for(j=i=1; i<N; i++){
        if(i < j){
            io = i - 1;
            in = j - 1;
            rtp = re[in];
            itp = im[in];
            re[in] = re[io];
            im[in] = im[io];
            re[io] = rtp;
            im[io] = itp;
        }
        k = nn;
        while(k < j){
            j = j - k;
            k >>= 1;
        }
        j = j + k;
    }

    if(flag == DIRECT) return;
    rw = 1.0 / N;
    for(i=0; i<N; i++){
        re[i] *= rw;
        im[i] *= rw;
    }
    return;
}

void shift(int16_t shift, float *re, float *im, uint16_t n){
    if(shift < 0 || shift >= n) return;
    for(uint16_t i=n/2-1; i>=shift; i--){
		re[i] = re[i-shift];
		re[(n-1-i)] = re[(n-1-i)+shift];
		im[i] = im[i-shift];
		im[(n-1-i)] = im[(n-1-i)+shift];
	}
	for(uint16_t i=0; i<shift; i++){
		re[i] = 0;
		re[n-1-i] = 0;
		im[i] = 0;
		im[n-1-i] = 0;
	}
}
