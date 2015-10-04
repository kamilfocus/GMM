#include "pixel.hpp"
#include "gaussian.hpp"
#include <iostream>
#include <algorithm>

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

void Pixel::print(int gaussian_num)
{
    if(gaussian_num == -1)
    {
        for(int gauss_num = 0; gauss_num < k; ++gauss_num)
        {
            cout<<gauss_num<<":";
            gaussian_ptr[gauss_num].print();
            cout<<" ";
        }
    }
    else
    {
        if(gaussian_num >= 0 && gaussian_num < k)
        {
            cout<<gaussian_num<<":";
            gaussian_ptr[gaussian_num].print();
        }
        else
        {
            print_error(gaussian_num);
        }
    }
}

void Pixel::get_rgb_mean(int gaussian_num, double * gaussian_means)
{
    if(gaussian_num < k)
    {
        gaussian_ptr[gaussian_num].get_rgb_mean(gaussian_means);
    }
    else
    {
        print_error(gaussian_num);
    }
}

void Pixel::print_error(int gaussian_num)
{
    cout<<"No Gaussian with id: "<<gaussian_num<<endl;
}


bool operator<(const Gaussian& x, const Gaussian& y)
{
    return (x.get_sort_parameter() > y.get_sort_parameter());
}

void Pixel::sort(double bg_classifier)
{
    std::sort(gaussian_ptr, (gaussian_ptr + k));
}

bool Pixel::is_foreground(int red, int green, int blue)
{
    return true;
}
