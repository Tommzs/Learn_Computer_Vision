#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    int i, j, k;
    float val_sum = 0;
    for(i = 0; i < im.w; i++)
    {
        for(j = 0; j < im.h; j++)
        {
            for(k = 0; k < im.c; k++)
            {
                val_sum += get_pixel(im, i, j, k); 
            }
        }
    }
    for(i = 0; i < im.w; i++)
    {
        for(j = 0; j < im.h; j++)
        {
            for(k = 0; k < im.c; k++)
            {
                set_pixel(im, i, j, k, get_pixel(im, i, j, k)/val_sum); 
            }
        }
    }
}

image make_box_filter(int w)
{
    assert((w % 2) == 1);

    int i, j;
    image box_filter = make_image(w, w, 1);
    float val = 1/(w*w);
    for(i = 0; i < w; i++)
    {
        for(j = 0; j < w; j++)
        {
            set_pixel(box_filter, i, j, 0, val);
        }
    }

    return box_filter;
}

float do_convolution(image im, image filter, int x, int y, int im_c, int f_c)
{
    assert(filter.c == 1);

    float new_val = 0.f;
    int i, j, ii, jj;
    ii = x - (filter.w/2);
    jj = y - (filter.h/2);
    for(i = 0; i < filter.w; i++, ii++)
    {
        for(j = 0; j < filter.h; j++, jj++)
        {
            new_val += get_pixel(filter, i, j, f_c)*get_pixel(im, ii, jj, im_c);
        }   
    }

    return new_val/(float)(filter.w*filter.h);
}

image convolve_image(image im, image filter, int preserve)
{
    assert(im.c == filter.c || filter.c == 1);

    int i, j, k; 
    image filtered = copy_image(im);


    if(im.c == filter.c && filter.c != 1)
    {
        for(i = 0; i < im.w; i++)
        {
            for(j = 0; j < im.h; j++)
            {
                for(k = 0; k < im.c; k++)
                {
                    set_pixel(filtered, i, j, k, do_convolution(im, filter, i, j, k, k));
                }
            }
        }
    }
    else if(preserve == 1)
    {
        for(i = 0; i < im.w; i++)
        {
            for(j = 0; j < im.h; j++)
            {
                for(k = 0; k < im.c; k++)
                {
                    set_pixel(filtered, i, j, k, do_convolution(im, filter, i, j, k, 0));
                }
            }
        }
    }
    else
    {
        for(i = 0; i < im.w; i++)
        {
            for(j = 0; j < im.h; j++)
            {
                set_pixel(filtered, i, j, k, do_convolution(im, filter, i, j, 0, 0));
            }
        }
    }
    
    return filtered;
}

image make_highpass_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_sharpen_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_emboss_filter()
{
    // TODO
    return make_image(1,1,1);
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
