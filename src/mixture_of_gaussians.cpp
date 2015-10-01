#include "mixture_of_gaussians.hpp"
#include <iostream>

MixtureOfGaussians::MixtureOfGaussians(int clusters_num, int k, double alpha)
{
    this->width = NONE;
    this->height = NONE;
    this->clusters_num = clusters_num;
    is_initialized = false;
    pixels = new Pixel*[height];
    for(int i=0; i < height; ++i)
    {
        pixels[i] = new Pixel[width];
    }

    for(int i=0; i < height; ++i)
    {
        for(int j=0; j < width; ++j)
            pixels[i][j].init(k, alpha);
    }
}

Mat MixtureOfGaussians::update(const Mat & input_frame)
{
    Mat result = input_frame;
    if(!is_initialized)
    {
        height = input_frame.rows;
        width = input_frame.cols;
        is_initialized = true;
    }
    for(int row = 0; row < height; ++row)
    {
        const uchar *input_pixel_ptr = input_frame.ptr(row);
        uchar *result_pixel_ptr = result.ptr(row);
        for(int col = 0; col < width; ++col)
        {
            //points to each pixel B,G,R value in turn assuming a CV_8UC3 color image
            paint_foreground(&input_pixel_ptr, &result_pixel_ptr, row, col);
        }
    }
    return result;
}

void MixtureOfGaussians::paint_foreground(const uchar ** const input_pixel_ptr,
                                          uchar ** const result_pixel_ptr,
                                          int row, int col)
{
    int blue, green, red; //RGB
    blue = (int) *(*input_pixel_ptr)++;
    green = (int) *(*input_pixel_ptr)++;
    red = (int) *(*input_pixel_ptr)++;

    if( pixels[row][col].is_foreground(red, green, blue) )    //Foreground
    {
        for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
            *(*result_pixel_ptr)++ = WHITE;
    }
    else
    {
        for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
            *(*result_pixel_ptr)++ = BLACK;
    }
}
