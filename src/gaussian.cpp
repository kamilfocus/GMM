#include "gaussian.hpp"
#include <iostream>

void Gaussian::init(double alpha, uchar *gaussian_mean)
{
    this->alpha = alpha;
    this->r_mean = gaussian_mean[0];
    this->g_mean = gaussian_mean[1];
    this->b_mean = gaussian_mean[2];
    //std::cout<<alpha<<std::endl;
}

void Gaussian::frame_init(double weight, double *gaussian_mean, double standard_deviation)
{
    this->weight = weight;
    this->r_mean = gaussian_mean[0];
    this->g_mean = gaussian_mean[1];
    this->b_mean = gaussian_mean[2];
    this->standard_deviation = standard_deviation;
}

bool Gaussian::update()
{
    return true;
}
