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
    float val = 1.0f/(float)(w*w);
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
    for(i = 0; i < filter.w; i++, ii++)
    {
        jj = y - (filter.h/2);
        for(j = 0; j < filter.h; j++, jj++)
        {
            new_val += get_pixel(filter, i, j, f_c)*get_pixel(im, ii, jj, im_c);
        }   
    }

    return new_val;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(im.c == filter.c || filter.c == 1);

    int i, j, k;
    image filtered;
    image filtered_temp = make_image(im.w, im.h, im.c);


    if(im.c == filter.c)
    {
        for(i = 0; i < im.w; i++)
        {
            for(j = 0; j < im.h; j++)
            {
                for(k = 0; k < im.c; k++)
                {
                    set_pixel(filtered_temp, i, j, k, do_convolution(im, filter, i, j, k, k));
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
                for(k = 0; k < im.c; k++)
                {
                    set_pixel(filtered_temp, i, j, k, do_convolution(im, filter, i, j, k, 0));
                }
            }
        }
    }

    if(preserve == 0)
    {
        filtered = make_image(im.w, im.h, 1);
        for(i = 0; i < im.w; i++)
        {
            for(j = 0; j < im.h; j++)
            {
                float val = 0.0f;
                for(k = 0; k < im.c; k++)
                {
                    val += get_pixel(filtered_temp, i, j, k);
                }
                set_pixel(filtered, i, j, 0, val);
            }
        }
    }
    else
    {
        filtered = filtered_temp;
    }
    
    return filtered;
}

image make_highpass_filter()
{
    image filter = make_image(3, 3, 1);
    float data[] = { 0.0f, -1.0f, 0.0f, -1.0f, 4.0f, -1.0f, 0.0f, -1.0f, 0.0f };
    memcpy(filter.data, data , sizeof(float)*3*3);

    return filter;
}

image make_sharpen_filter()
{
    image filter = make_image(3, 3, 1);
    float data[] = { 0.0f, -1.0f, 0.0f, -1.0f, 5.0f, -1.0f, 0.0f, -1.0f, 0.0f };
    memcpy(filter.data, data , sizeof(float)*3*3);

    return filter;
}


image make_emboss_filter()
{
    image filter = make_image(3, 3, 1);
    float data[] = { -2.0f, -1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f };
    memcpy(filter.data, data , sizeof(float)*3*3);

    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: highpass - no preserve, finds edges, no color needed. Sharpen+emboss - do stylistical modifications to image, thus preserve shall be used to keep 3 channels.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: yes for sharpen and emboss we need to clamp image since the sum of the values is not 0

image make_gaussian_filter(float sigma)
{
    int x, y, xx, yy;
    int w = (int)ceil(sigma*6);
    float sigma2 = sigma*sigma;
    
    w = (w % 2 == 0) ? (w+1) : w;

    image filter = make_image(w, w, 1);
    
    xx = -(w/2);
    for(x = 0; x < w; x++, xx++)
    {
        yy = -(w/2);
        for(y = 0; y < w; y++, yy++)
        {
            float val = 1.0f/(TWOPI*sigma2)*exp(-((float)(xx*xx+yy*yy))/(2.0f*sigma2));
            set_pixel(filter, x, y, 0, val);
        }
    }

    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image summed = make_image(a.w, a.h, a.c);

    int i, j, k;
    for(i = 0; i < a.w; i++)
    {
        for(j = 0; j < a.h; j++)
        {
            for(k = 0; k < a.c; k++)
            {
                float summed_val = get_pixel(a, i, j, k)+get_pixel(b, i, j, k);
                set_pixel(summed, i, j, k, summed_val);
            }
        }
    }

    return summed;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image subtracted = make_image(a.w, a.h, a.c);

    int i, j, k;
    for(i = 0; i < a.w; i++)
    {
        for(j = 0; j < a.h; j++)
        {
            for(k = 0; k < a.c; k++)
            {
                float subtracted_val = get_pixel(a, i, j, k)-get_pixel(b, i, j, k);
                set_pixel(subtracted, i, j, k, subtracted_val);
            }
        }
    }

    return subtracted;
}

image make_gx_filter()
{
    image filter = make_image(3, 3, 1);
    float data[] = { -1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 1.0f };
    memcpy(filter.data, data , sizeof(float)*3*3);

    return filter;
}

image make_gy_filter()
{
    image filter = make_image(3, 3, 1);
    float data[] = { -1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f };
    memcpy(filter.data, data , sizeof(float)*3*3);

    return filter;
}

void feature_normalize(image im)
{
    int i, j, k;
    float minv = 999999999;
    float maxv = -999999999;
    for(i = 0; i < im.w; i++)
    {
        for(j = 0; j < im.h; j++)
        {
            for(k = 0; k < im.c; k++)
            {
                float pixval = get_pixel(im, i, j, k);
                minv = fmin(minv, pixval);
                maxv = fmax(maxv, pixval);  
            }
        }
    }
    float range = maxv-minv;
    if(abs(range) < 1.0E-9f)
    {
        memset(im.data, 0, sizeof(float)*im.w*im.h*im.c);
    }
    else
    {
        for(i = 0; i < im.w; i++)
        {
            for(j = 0; j < im.h; j++)
            {
                for(k = 0; k < im.c; k++)
                {
                    float pixval = get_pixel(im, i, j, k);
                    set_pixel(im, i, j, k, (pixval-minv)/(range));
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    image gxf = make_gx_filter();
    image gyf = make_gy_filter();
    image gx = convolve_image(im, gxf, 0);
    image gy = convolve_image(im, gyf, 0);

    image mag = make_image(gx.w, gx.h, gx.c);
    image grd = make_image(gx.w, gx.h, gx.c);

    int i,j;
    for(i = 0; i < im.w; i++)
    {
        for(j = 0; j < im.h; j++)
        {
            float gxv = get_pixel(gx, i, j, 0);
            float gyv = get_pixel(gy, i, j, 0);
            set_pixel(mag, i, j, 0, sqrt((gxv*gxv) + (gyv*gyv)));
            set_pixel(grd, i, j, 0, atan2(gyv,gxv));
        }
    }

    image* sobel_result = calloc(2, sizeof(image));
    sobel_result[0] = mag;
    sobel_result[1] = grd;

    return sobel_result;
}

image colorize_sobel(image im)
{
    image gauss5 = make_gaussian_filter(5);
    im = convolve_image(im, gauss5, 1);

    image *sobel_result = sobel_image(im);
    image mag = sobel_result[0];
    //image mag = convolve_image(sobel_result[0], gauss5, 0);
    image grd = sobel_result[1];
    //image grd = convolve_image(sobel_result[1], gauss5, 0);
    image result_image = make_image(im.w, im.h, im.c);

    feature_normalize(mag);
    feature_normalize(grd);

    int i,j;
    for(i = 0; i < im.w; i++)
    {
        for(j = 0; j < im.h; j++)
        {
            float magv = get_pixel(mag, i, j, 0);
            float grdv = get_pixel(grd, i, j, 0);
            set_pixel(result_image, i, j, 0, grdv);
            set_pixel(result_image, i, j, 1, magv);
            set_pixel(result_image, i, j, 2, magv);
        }
    }

    hsv_to_rgb(result_image);

    return result_image;
}
