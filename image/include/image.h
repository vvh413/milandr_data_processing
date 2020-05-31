#ifndef IMG_H
#define IMG_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#define FILL 150

uint8_t bilinear_interpolate(float x, float y, int32_t rows, int32_t cols, uint8_t (*the_image)[cols]);

void arotate(float angle, int32_t bilinear, int32_t cols, int32_t rows, uint8_t (*the_image)[cols],
             uint8_t (*out_image)[cols], int32_t m, int32_t n);

void full_warp_loop(int32_t rows, int32_t cols, uint8_t (*the_image)[cols], uint8_t (*out_image)[cols],
                    int32_t x1, int32_t x2, int32_t x3, int32_t x4,
                    int32_t y1, int32_t y2, int32_t y3, int32_t y4);

void erosion(int32_t rows, int32_t cols,
             uint8_t (*the_image)[cols], uint8_t (*out_image)[cols],
             uint8_t t1, uint8_t t2, int32_t nbrs);

#endif