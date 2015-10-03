#include "gaussian.hpp"
#include <iostream>
using namespace std;

void Gaussian::init(double alpha, uchar *gaussian_mean)
{
    this->alpha = alpha;
    this->r_mean = gaussian_mean[0];
    this->g_mean = gaussian_mean[1];
    this->b_mean = gaussian_mean[2];
    this->weight = 0.0;
    this->standard_deviation = 0.0
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

void Gaussian::get_rgb_mean(double * gaussian_means)
{
    gaussian_means[0] = r_mean;
    gaussian_means[1] = g_mean;
    gaussian_means[2] = b_mean;
}

void Gaussian::print()
{
    string delimiter = ", ";
    cout<<"(w, r_mean, g_mean, b_mean, std_dev) = ";
    cout<<"("<<weight<<delimiter<<r_mean<<delimiter;
    cout<<g_mean<<delimiter<<b_mean<<delimiter;
    cout<<standard_deviation<<")"<<endl;
}

bool Gaussian::update()
{
    return true;
}
