#include "gaussian.hpp"
#include "tools.hpp"
#include <iostream>
using namespace std;

double Gaussian::initial_variance = 0;
double Gaussian::alpha = 0;

#define THRESHOLD (2.5)

void Gaussian::initialise(double weight, double *gaussian_mean, double standard_deviation)
{
    rgb_mean = new double[RGB_COMPONENTS_NUM];
    matchsum = 1;
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
}

void Gaussian::print()
{
    string delimiter = ", ";
    cout<<"("<<weight<<delimiter<<rgb_mean[0]<<delimiter;
    cout<<rgb_mean[1]<<delimiter<<rgb_mean[2]<<delimiter;
    cout<<standard_deviation<<delimiter<<matchsum<<")";
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
    //RGB means
    double alpha_w = alpha/weight;
    double dist_sq = rgb[0]-rgb_mean[0];
    dist_sq*= dist_sq;
    for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
    {
        rgb_mean[i] += alpha_w*(rgb[i] - rgb_mean[i]);
        if(rgb_mean[i] < 0)
            rgb_mean[i] = 0;
        if(rgb_mean[i] > 255)
            rgb_mean[i] = 255;
    }

    //Standard deviation
    standard_deviation *= standard_deviation;
    standard_deviation += alpha_w*(dist_sq - standard_deviation);
    if(standard_deviation <= 0)
    {
        standard_deviation = 1;
        //string del = " ";
        //cout<<"Error: "<<rgb[0]<<del<<rgb_mean[0]<<del<<dist_sq<<del<<standard_deviation<<endl;
        //exit(-1);
    }
    standard_deviation = sqrt(standard_deviation);

    //Weight
    weight *=  (1 - alpha);
    weight += alpha;

    //Matchsum
    matchsum++;
}

bool Gaussian::check_pixel_match(double *rgb) //sqrt((r-r_mean)^2 + (b-b_mean)^2 + (c-c_mean)^2)  / dev
{
    double diff = rgb[0]-rgb_mean[0];
    double dist = (diff > 0) ? diff : -diff;
    double threshold = THRESHOLD * standard_deviation;
    if(dist < threshold)
        return true;
    else
        return false;
}

