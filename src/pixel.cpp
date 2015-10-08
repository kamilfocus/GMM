#include "pixel.hpp"
#include "gaussian.hpp"
#include "tools.hpp"
#include <iostream>
#include <algorithm>

double Pixel::T = 0;

void Pixel::init(int k, double alpha, uchar **gaussians_means)
{
    this->k = k;
    //std::cout<<k<<" "<<alpha<<std::endl;
    gaussian_ptr = new Gaussian[k];
    for(int i = 0; i < k; ++i)
        gaussian_ptr[i].init(alpha, ( (gaussians_means==NULL)? NULL:gaussians_means[i] ) );
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
    	sum += gaussian_ptr[i].get_weight();
    	gaussian_ptr[i].set_isForeground(isForeground);
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
    //	cout << "case 1 " << match_index << "  ";
        for(int i=0; i < k; i++)
        {
            if(i == match_index)
            {
                gaussian_ptr[i].update_matched(rgb);
                continue;
            }
            gaussian_ptr[i].update_unmatched();
        }

        bool retval = gaussian_ptr[match_index].isForeground();
        //std::sort(gaussian_ptr, gaussian_ptr+k);
        sort(T);

        return retval;
    }
    else //Case 2: No match is found
    {
    //	cout << "case 2   ";
    	/*
    	int new_gaussian_index = 0;
    	double new_weight = 0;
    	for(int i=0; i<k; i++)
    	{
    		if(new_weight==0 || new_weight>gaussian_ptr[i].get_weight())
    		{
    			new_gaussian_index = i;
    			new_weight = gaussian_ptr[i].get_weight();
    		}
    	}
    	double * new_means = rgb;
    	double new_deviation = sqrt(Gaussian::get_initial_variance());
        gaussian_ptr[new_gaussian_index].frame_init(new_weight, new_means, new_deviation);
        std::sort(gaussian_ptr, gaussian_ptr+k);
        */

    	double * new_means = rgb;
    	//double new_deviation = max(sqrt(Gaussian::get_initial_variance()), gaussian_ptr[k-1].get_deviation());
    	double new_deviation = get_max_deviation();
    	double new_weight = gaussian_ptr[k-1].get_weight();

    	gaussian_ptr[k-1].frame_init(new_weight, new_means, new_deviation);

        return true;
    }
}

double Pixel::get_max_deviation()
{
	double max_dev = gaussian_ptr[0].get_deviation();
	for(int i=0; i<k; ++i)
		max_dev = (gaussian_ptr[i].get_deviation() > max_dev) ? gaussian_ptr[i].get_deviation() : max_dev;

	return max_dev;
}
