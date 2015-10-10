#include "mixture_of_gaussians.hpp"
#include "tools.hpp"
#include "gaussian.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

MixtureOfGaussians::MixtureOfGaussians(int k, double alpha, double bg_classifier, int init_std_dev)
{
    this->width = 0;
    this->height = 0;
    this->k = k;
    this->init_std_dev = init_std_dev;
    Pixel::set_T(bg_classifier);
    Pixel::set_k(k);
    Gaussian::set_alpha(alpha);
    is_initialized = false;
    pixels = NULL;
}

MixtureOfGaussians::~MixtureOfGaussians()
{
	for(int i=0; i < height; i++)
		delete [] pixels[i];
	delete [] pixels;

}

void MixtureOfGaussians::update(const Mat & input_frame, Mat & result_frame)
{

    if(!is_initialized)
    {
        initialise(input_frame);
        is_initialized = true;
    }

    double rgb[RGB_COMPONENTS_NUM];
    const uchar * input_pixel_ptr;
    uchar * result_pixel_ptr;
    uchar mask;
    for(int row = 0; row < height; ++row)
    {
        input_pixel_ptr = input_frame.ptr(row);
        result_pixel_ptr = result_frame.ptr(row);
        for(int col = 0; col < width; ++col)
        {
            //RGB reverted order
            rgb[2] = (double) *input_pixel_ptr++;
            rgb[1] = (double) *input_pixel_ptr++;
            rgb[0] = (double) *input_pixel_ptr++;
            mask = (pixels[row][col].is_foreground(rgb)) ? WHITE : BLACK;

            for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
                *result_pixel_ptr++ = mask;
        }
    }
}

void MixtureOfGaussians::print_parameters(int row, int col, int gaussian_num)
{
    if(row == -1 || col == -1)
    {
        for(int row = 0; row < height; ++row)
        {
            for(int col = 0; col < width; ++col)
            {
                cout<<"["<<row<<"]"<<"["<<col<<"]: ";
                pixels[row][col].print(gaussian_num);
                cout<<endl;
            }
        }
    }
    else
    {
        if(row < 0 && row >= height)
        {
            cout<<"Row out of bounds: "<<row<<endl;
            return;
        }
        if(col < 0 && col >= width)
        {
            cout<<"Col out of bounds: "<<col<<endl;
            return;
        }
        cout<<"["<<row<<"]"<<"["<<col<<"]: ";
        pixels[row][col].print(gaussian_num);
        cout<<endl;
    }
}

void MixtureOfGaussians::initialise(const Mat & input_frame)
{
    height = input_frame.rows;
    width = input_frame.cols;

	double** new_gaussian_means = new double* [k];
	double * new_weight = new double [k];
	double * new_deviation = new double[k];

	for(int i=0; i<k; i++)
		new_gaussian_means[i] = new double [RGB_COMPONENTS_NUM];

	double mean_value = 30;
	double step = (255 - 30)/k;
	for(int i=0; i<k; i++)
	{
		for(int j=0; j<RGB_COMPONENTS_NUM; j++)
			new_gaussian_means[i][j] = mean_value + i*step;
		new_weight[i] = 1.0/k;
		new_deviation[i] = init_std_dev;
	}

    pixels = new Pixel*[height];
    for(int i=0; i < height; ++i)
    {
        pixels[i] = new Pixel[width];
    }

    for(int i=0; i < height; ++i)
    {
        for(int j=0; j < width; ++j)
        {
            pixels[i][j].initialise(new_weight, new_gaussian_means, new_deviation);
        }
    }

    for(int i=0; i<k; i++)
        delete [] new_gaussian_means[i];
    delete [] new_weight;
    delete [] new_deviation;
    delete [] new_gaussian_means;
}

