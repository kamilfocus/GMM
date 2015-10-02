#include "pixel.hpp"
#include "gaussian.hpp"
#include <iostream>

void Pixel::init(int k, double alpha, uchar **gaussians_means)
{
    this->k = k;
    //std::cout<<k<<" "<<alpha<<std::endl;
    gaussian_ptr = new Gaussian[k];
    for(int i = 0; i < k; ++i)
        gaussian_ptr[i].init(alpha, gaussians_means[i]);
}

void Pixel::frame_init(double *weight, double **gaussian_means, double *standard_devation){
    for(int i=0; i < k; ++i)
    {
        gaussian_ptr[i].frame_init(weight[i], gaussian_means[i], standard_devation[i]);
    }
}

bool Pixel::is_foreground(int red, int green, int blue)
{
    return true;
}

