#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    int xi = round(x);
    int yi = round(y);
    xi = xi >= 0 ? xi : 0;
    yi = yi >= 0 ? yi : 0;
    return get_pixel(im, xi, yi, c);
}

image nn_resize(image im, int w, int h)
{
    int i, j, k;

    image resized = make_image(w, h, im.c);
    float wr = ((float)im.w)/((float)w);
    float hr = ((float)im.h)/((float)h);
    
    for(i = 0; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            for(k = 0; k < im.c; k++)
            {
                float iim = ((float)i+0.5f)*wr-0.5f;
                float jim = ((float)j+0.5f)*hr-0.5f;
                set_pixel(resized, i, j, k, nn_interpolate(im, iim, jim, k));
            }
        }
    }

    return resized;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    int left = floorf(x+0.5f);
    int right = ceilf(x+0.5f);
    int down = floorf(y+0.5f);
    int up = ceilf(y+0.5f);

    left = left < im.w ? left : im.w;
    right = right < im.w ? right : im.w;
    down = down < im.h ? down : im.h;
    up = up < im.h ? up : im.h;

    float d1 = fabsf(x+0.5f-(float)left);
    float d2 = fabsf(x+0.5f-(float)right);
    float d3 = fabsf(y+0.5f-(float)down);
    float d4 = fabsf(y+0.5f-(float)up);

    float A1 = d2*d4;
    float A2 = d1*d4;
    float A3 = d2*d3;
    float A4 = d1*d3;

    float V1 = get_pixel(im, left, down, c);
    float V2 = get_pixel(im, right, down, c);
    float V3 = get_pixel(im, left, up, c);
    float V4 = get_pixel(im, right, up, c);

    float rval = V1*A1 + V2*A2 + V3*A3 + V4*A4;
    return rval;
}

image bilinear_resize(image im, int w, int h)
{
    int i, j, k;

    image resized = make_image(w, h, im.c); 
    float wr = ((float)im.w)/((float)w);
    float hr = ((float)im.h)/((float)h);
    
    for(i = 0; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            for(k = 0; k < im.c; k++)
            {
                float iim = ((float)i+0.5f)*wr;
                float jim = ((float)j+0.5f)*hr;
                float v = bilinear_interpolate(im, iim, jim, k);
                set_pixel(resized, i, j, k, v);
            }
        }
    }

    return resized;
}

