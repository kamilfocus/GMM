#include "pixel.hpp"
#include "gaussian.hpp"
#include "tools.hpp"
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

Pixel & Pixel::operator=(const Pixel & pixel)
{
	for(int i=0; i<k; i++)
		gaussian_ptr[i] = pixel.gaussian_ptr[i];

	return (*this);
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

void Pixel::sort(double bg_classifier)
{
    std::sort(gaussian_ptr, (gaussian_ptr + k));

    double sum = 0;
    bool isForeground = false;
    for(int i=0; i<k; i++)
    {
    	sum += gaussian_ptr[i].getWeight();
    	gaussian_ptr[i].setIsForeground(isForeground);
    	if(sum > bg_classifier)
    		isForeground = true;
    }
}

bool Pixel::is_foreground(double * rgb)
{
    // Check if any gaussian matches current pixel
    bool match_found = false;
    int match_index;
    for(match_index=0; match_index < k; ++match_index)
    {
        match_found = gaussian_ptr[match_index].check_pixel_match(rgb);
        if(match_found == true)
            break;
    }

    if(match_found)// Case 1: Match is found
    {
        for(int i=0; i < k; i++)
        {
            if(i == match_index)
            {
                gaussian_ptr[i].update_matched(rgb);
                continue;
            }
            gaussian_ptr[i].update_unmatched();
        }
        return true;
    }
    else//@TODO Case 2: No match is found
    {

        return false;
    }
}
