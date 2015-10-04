#include "gaussian.hpp"
#include "tools.hpp"
#include <iostream>
using namespace std;

double Gaussian::initial_variance = 0;

void Gaussian::init(double alpha, uchar *gaussian_mean)
{
    this->alpha = alpha;
    //this->r_mean = gaussian_mean[0];
    //this->g_mean = gaussian_mean[1];
    //this->b_mean = gaussian_mean[2];
    rgb_mean = new double[RGB_COMPONENTS_NUM];
    for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
    {
        rgb_mean[i] = gaussian_mean[i];
    }
    this->weight = 0.0;
    this->standard_deviation = 0.0;
    //std::cout<<alpha<<std::endl;
}

void Gaussian::frame_init(double weight, double *gaussian_mean, double standard_deviation)
{
    this->weight = weight;
    for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
    {
        rgb_mean[i] = gaussian_mean[i];
    }
    this->standard_deviation = standard_deviation;
}

void Gaussian::get_rgb_mean(double * gaussian_means)
{
    for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
    {
        gaussian_means[i]= rgb_mean[i];
    }
    //gaussian_means[0] = r_mean;
    //gaussian_means[1] = g_mean;
    //gaussian_means[2] = b_mean;
}

void Gaussian::print()
{
    string delimiter = ", ";
    //cout<<"(w, r_mean, g_mean, b_mean, std_dev) = ";
    cout<<"("<<weight<<delimiter<<rgb_mean[0]<<delimiter;
    cout<<rgb_mean[1]<<delimiter<<rgb_mean[2]<<delimiter;
    cout<<standard_deviation<<")";
}

bool Gaussian :: operator<(const Gaussian& gaussian) const
{
    return (get_sort_parameter() > gaussian.get_sort_parameter());
}

double Gaussian::get_sort_parameter() const
{
    return (weight/standard_deviation);
}

void Gaussian::update_unmatched()
{
    weight *= (1 - alpha);
}

void Gaussian::update_matched(double *rgb)
{
    //Weight
    weight *=  (1 - alpha);
    weight += alpha;

    //RGB means
    double ro = alpha*count_probability_density(rgb, rgb_mean, standard_deviation);
    for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
    {
        rgb_mean[i] *= (1 - ro);
        rgb_mean[i] += ro*rgb[i];
    }

    //Standard deviation
    double dist = malahidan_distance(rgb, rgb_mean, RGB_COMPONENTS_NUM);
    standard_deviation *= standard_deviation;
    standard_deviation *= (1 - ro);
    standard_deviation += ro*dist;
    standard_deviation = sqrt(standard_deviation);
}

bool Gaussian::check_pixel_match(double *rgb) //sqrt((r-r_mean)^2 + (b-b_mean)^2 + (c-c_mean)^2)  / dev
{
    double dist = malahidan_distance(rgb, rgb_mean, RGB_COMPONENTS_NUM);
    dist = sqrt(dist);
    dist /= standard_deviation;
    double threshold = THRESHOLD * standard_deviation;
    if(dist < threshold)
        return true;
    else
        return false;
}

Gaussian & Gaussian::operator=(const Gaussian & gaussian)
{
    weight = gaussian.weight;

    for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
    {
        rgb_mean[i] = gaussian.rgb_mean[i];
    }
    //r_mean = gaussian.r_mean;
    //g_mean = gaussian.g_mean;
    //b_mean = gaussian.b_mean;
    standard_deviation = gaussian.standard_deviation; //same for all RGB values, sigma without square
    foreground = gaussian.foreground;

	return (*this);
}
