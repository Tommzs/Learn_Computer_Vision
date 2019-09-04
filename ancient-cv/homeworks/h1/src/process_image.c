#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float fclamp(float low, float v, float high)
{
    return fmax(low, fmin(v, high));
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int clamp(int v, int low, int high)
{
    return max(low, min(v, high));
}

float get_pixel(image im, int x, int y, int c)
{
    int W = im.w;
    int H = im.h;
    x = clamp(x, 0, (W-1));
    y = clamp(y, 0, (H-1));
    int index = x+y*W+c*W*H;
    return im.data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    int W = im.w;
    int H = im.h;
    x = clamp(x, 0, (W-1));
    y = clamp(y, 0, (H-1));
    int index = x+y*W+c*W*H;
    im.data[index] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, sizeof(float)*im.w*im.h*im.c);
    return copy;
}

image rgb_to_grayscale(image im)
{
    int i,j;
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(i = 0; i < gray.w; i++)
    {
        for(j = 0; j < gray.h; j++)
        {
            float gray_val = 0.299f*get_pixel(im, i, j, 0) + 0.587f*get_pixel(im, i, j, 1) + .114f*get_pixel(im, i, j, 2);
            set_pixel(gray, i, j, 0, gray_val);
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    int i, j;
    for(i = 0; i < im.w; i++)
    {
        for(j = 0; j < im.h; j++)
        {
            set_pixel(im, i, j, c, get_pixel(im, i, j, c)+v);
        }
    }
}

void clamp_image(image im)
{
    int i;
    int pix_num = im.w*im.h*im.c;
    for(i = 0; i < pix_num; i++)
    {
        im.data[i] = fclamp(0.f, im.data[i], 1.f);
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    int i, j;
    float R, G, B, V, C, m, S, H;
    for(i = 0; i < im.w; i++)
    {
        for(j = 0; j < im.h; j++)
        {
            R = get_pixel(im, i, j, 0);
            G = get_pixel(im, i, j, 1);
            B = get_pixel(im, i, j, 2);
            
            V = three_way_max(R, G, B);
            m = three_way_min(R, G, B);
            C = V - m;
            S = (V > 0.f) ? C / V : 0.f;

            if(C == 0.f)
            {
                H = 0.f;
            }
            else if(V == R)
            {
                H = (G - B) / C;
            }
            else if(V == G)
            {
                H = ((B - R) / C) + 2.f;
            }
            else if(V == B)
            {
                H = ((R - G) / C) + 4.f;
            }

            H = (H / 6.f);

            if(H < 0.f)
            {
                H += 1.f;
            }

            set_pixel(im, i, j, 0, H);
            set_pixel(im, i, j, 1, S);
            set_pixel(im, i, j, 2, V);
        }
    }
}

float hsv_to_rgb_func(float H, float S, float V, float n)
{
    float k = fmod(n+(H*6), 6);
    float min_part = three_way_min(k, 4.f-k, 1.f);
    float max_part = fmax(min_part, 0.f);
    return V - (V * S * max_part);
}

void hsv_to_rgb(image im)
{
    int i, j;
    float R, G, B, V, S, H;
    for(i = 0; i < im.w; i++)
    {
        for(j = 0; j < im.h; j++)
        {
            H = get_pixel(im, i, j, 0);
            S = get_pixel(im, i, j, 1);
            V = get_pixel(im, i, j, 2);
            
            R = hsv_to_rgb_func(H, S, V, 5);
            G = hsv_to_rgb_func(H, S, V, 3);
            B = hsv_to_rgb_func(H, S, V, 1);

            set_pixel(im, i, j, 0, R);
            set_pixel(im, i, j, 1, G);
            set_pixel(im, i, j, 2, B);
        }
    }
}
