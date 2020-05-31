#include "image.h"

uint8_t bilinear_interpolate(float x, float y, int32_t rows, int32_t cols, uint8_t (*the_image)[cols])
{
    float fraction_x, fraction_y,
    one_minus_x, one_minus_y,
    tmp_float;
    int32_t ceil_x, ceil_y, floor_x, floor_y;
    uint8_t LP, UP, Target, result = FILL;
    //если точка находится за границами
    if(x < 0.0 || x >= (float)(cols-1) || y < 0.0 || y >= (float)(rows-1))
        return(result);
    floor_x = floor(x);
    floor_y = floor(y);
    ceil_x = ceil(x);
    ceil_y = ceil(y);

    fraction_x = x - floor_x;
    fraction_y = y - floor_y;
    one_minus_x = 1.0 - fraction_x;
    one_minus_y = 1.0 - fraction_y;

    //считаем значения мнимых точек по заданным формулам
    LP = one_minus_x * (float)(the_image[floor_y][floor_x]) +
                fraction_x * (float)(the_image[floor_y][ceil_x]);

    UP = one_minus_x * (float)(the_image[ceil_y][floor_x]) +
                fraction_x * (float)(the_image[ceil_y][ceil_x]);

    Target = one_minus_y * (float)(LP) + fraction_y * (float)(UP);

    return(Target);
}

void arotate(float angle, int32_t bilinear, int32_t cols, int32_t rows, uint8_t (*the_image)[cols],
             uint8_t (*out_image)[cols], int32_t m, int32_t n){
    float cosa, sina, radian_angle, new_x, new_y;
    int32_t i, j, new_i, new_j;

    radian_angle = angle/57.29577951;
    cosa = cos(radian_angle);
    sina = sin(radian_angle);

    for(i=0; i<rows; i++){
        for(j=0; j<cols; j++){
            new_x = (float)(j)*cosa - (float)(i)*sina - (float)(m)*cosa + (float)(m) + (float)(n)*sina;
            new_y = (float)(i)*cosa + (float)(j)*sina - (float)(m)*sina - (float)(n)*cosa + (float)(n);
            //округляем полученный дробный результат
            new_j = new_x;
            new_i = new_y;
            if(bilinear == 0){
                if(new_j < 0 || new_j >= cols || new_i < 0 || new_i >= rows)
                    out_image[i][j] = FILL;
                else
                    out_image[i][j] = the_image[new_i][new_j];
            }
            else{
                out_image[i][j] =
                bilinear_interpolate(new_x, new_y, rows, cols, the_image);
            }
        }
    }
}

void full_warp_loop(int32_t rows, int32_t cols, uint8_t (*the_image)[cols], uint8_t (*out_image)[cols],
                    int32_t x1, int32_t x2, int32_t x3, int32_t x4,
                    int32_t y1, int32_t y2, int32_t y3, int32_t y4)
{
    int32_t a, b, d, i, j, x_out, y_out;
    //это делитель для коэффициентов a и b
    d = (float)cols * rows;
    for(i=0; i<rows; i++){
        a = i / d;
        for(j=0; j<cols; j++){
            b = j / d;
            x_out = x1 + (x2-x1)*a + (x4-x1)*b + (x1-x2+x3-x4)*a*b;
            y_out = y1 + (y2-y1)*a + (y4-y1)*b + (y1-y2+y3-y4)*a*b;
            if(x_out < 0 || x_out >= cols || y_out < 0 || y_out >= rows)
                out_image[i][j] = FILL;
            else
                out_image[i][j] = the_image[y_out][x_out];
        }
    }
}

void erosion(int32_t rows, int32_t cols,
             uint8_t (*the_image)[cols], uint8_t (*out_image)[cols],
             uint8_t t1, uint8_t t2, int32_t nbrs)
{
    int32_t a, b, count, i, j, k;
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++)
            out_image[i][j] = the_image[i][j];
    for(i=1; i<rows-1; i++){
        for(j=1; j<cols-1; j++){
            //если мы не хотим выделять точки данного цвета
            if(the_image[i][j] < t1){
                //смотрим, стоит ли удалять саму точку
                if (the_image[i][j] <=t2){
                    out_image[i][j] = 0;
                } else{
                    count = 0;
                    //считаем количество соседних точек, удовлетворяющих условию удаления
                    for(a=-1; a<=1; a++){
                        for(b=-1; b<=1; b++){
                            if( (i+a) >= 0){
                                if(the_image[i+a][j+b] <= t2)
                                    count++;
                            }
                        }
                    }
                    //если их количество больше порогового значения, значит и сама точка должна быть удалена
                    if(count > nbrs) out_image[i][j] = 0;
                }
            }
        }
    }
}