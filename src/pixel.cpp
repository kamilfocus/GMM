#include "pixel.hpp"
#include "gaussian.hpp"
#include <iostream>

void Pixel::init(int k, double alpha)
{
    this->k = k;
    //std::cout<<k<<" "<<alpha<<std::endl;
    gaussian_ptr = new Gaussian[k];
    for(int i = 0; i < k; ++i)
        gaussian_ptr[i].init(alpha);
}

bool Pixel::is_foreground(int red, int green, int blue)
{
    return true;
}
