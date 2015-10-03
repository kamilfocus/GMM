#include "mixture_of_gaussians.hpp"
#include "tools.hpp"
#include <iostream>

MixtureOfGaussians::MixtureOfGaussians(int k, double alpha)
{
    this->width = NONE;
    this->height = NONE;
    this->clusters_num = k;
    is_initialized = false;

    uchar ** gaussian_means;
    gaussian_means = generate_inital_means(true);

    pixels = new Pixel*[height];
    for(int i=0; i < height; ++i)
    {
        pixels[i] = new Pixel[width];
    }

    for(int i=0; i < height; ++i)
    {
        for(int j=0; j < width; ++j)
            pixels[i][j].init(k, alpha, gaussian_means);
    }
}

uchar ** MixtureOfGaussians::generate_inital_means(bool deterministic)
{
    return NULL;
}

Mat MixtureOfGaussians::update(const Mat & input_frame)
{
    Mat result = input_frame;
    if(!is_initialized)
    {
        height = input_frame.rows;
        width = input_frame.cols;
        initialize_gaussians(input_frame, result);
        is_initialized = true;
    }
    for(int row = 0; row < height; ++row)
    {
        const uchar *input_pixel_ptr = input_frame.ptr(row);
        uchar *result_pixel_ptr = result.ptr(row);
        for(int col = 0; col < width; ++col)
        {
            //points to each pixel B,G,R value in turn assuming a CV_8UC3 color image
            paint_foreground(&input_pixel_ptr, &result_pixel_ptr, row, col);
        }
    }
    return result;
}

void MixtureOfGaussians::paint_foreground(const uchar ** const input_pixel_ptr,
                                          uchar ** const result_pixel_ptr,
                                          int row, int col)
{
    int blue, green, red; //RGB
    blue = (int) *(*input_pixel_ptr)++;
    green = (int) *(*input_pixel_ptr)++;
    red = (int) *(*input_pixel_ptr)++;

    if( pixels[row][col].is_foreground(red, green, blue) )    //Foreground
    {
        for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
            *(*result_pixel_ptr)++ = WHITE;
    }
    else
    {
        for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
            *(*result_pixel_ptr)++ = BLACK;
    }
}

void MixtureOfGaussians::initialize_gaussians(const Mat & input_frame, Mat & result_frame)
{
    //Allocate memory
    double **gaussian_means;
    double *weight, *standard_deviation;
    int * cluster_elements;
    int ** pixel_cluster_group;

    gaussian_means = new double*[clusters_num];
    weight = new double[clusters_num];
    standard_deviation = new double[clusters_num];
    cluster_elements = new int[clusters_num];
    pixel_cluster_group = new int*[height];
    for(int i = 0; i < clusters_num; ++i)
    {
        cluster_elements[i] = 0;
        weight[i] = 0.0;
        standard_deviation[i] = 0.0;
        gaussian_means[i] = new double[RGB_COMPONENTS_NUM];
        for(int j=0; j < RGB_COMPONENTS_NUM; ++j)
            gaussian_means[i][j] = 0.0;
    }

    //Precount means and weights
    int total_pixels_num = 0;
    double rgb[RGB_COMPONENTS_NUM];
    double temp_gaussian_means[RGB_COMPONENTS_NUM];
    for(int row = 0; row < height; ++row)
    {
        pixel_cluster_group[row] = new int[width];
        const uchar *input_pixel_ptr = input_frame.ptr(row);
        uchar *result_pixel_ptr = result_frame.ptr(row);

        for(int col = 0; col < width; ++col)
        {
            total_pixels_num++;

            //RGB reverted order
            rgb[2] = (double) *input_pixel_ptr++;
            rgb[1] = (double) *input_pixel_ptr++;
            rgb[0] = (double) *input_pixel_ptr++;

            pixels[row][col].get_rgb_mean(0, temp_gaussian_means);
            double cluster_value;
            int best_cluster_num = 0;
            double best_cluster_value = malahidan_distance(rgb, temp_gaussian_means, RGB_COMPONENTS_NUM);
            for(int gaussian_num = 1; gaussian_num < clusters_num; ++gaussian_num)
            {
                pixels[row][col].get_rgb_mean(gaussian_num, temp_gaussian_means);
                cluster_value = malahidan_distance(rgb, temp_gaussian_means, RGB_COMPONENTS_NUM);
                if(cluster_value < best_cluster_value)
                {
                    best_cluster_value = cluster_value;
                    best_cluster_num = gaussian_num;
                }
            }

            pixel_cluster_group[row][col] = best_cluster_num;
            cluster_elements[best_cluster_num]++;
            for(int rgb_num = 0; rgb_num < RGB_COMPONENTS_NUM; ++rgb_num)
            {
                gaussian_means[best_cluster_num][rgb_num] += rgb[rgb_num];
            }

            //Paint Black first result frame
            for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
                *result_pixel_ptr++ = BLACK;
        }
    }

    //Count final weights and means
    for(int i = 0; i < clusters_num; ++i)
    {
        weight[i] = cluster_elements[i]/((double) total_pixels_num);
        for(int rgb_num = 0; rgb_num < RGB_COMPONENTS_NUM; ++rgb_num)
        {
            gaussian_means[i][rgb_num] /= cluster_elements[i];
        }
    }

    //Precount standard deviation
    for(int row = 0; row < height; ++row)
    {
        const uchar *input_pixel_ptr = input_frame.ptr(row);
        for(int col = 0; col < width; ++col)
        {
            rgb[2] = (double) *input_pixel_ptr++;
            rgb[1] = (double) *input_pixel_ptr++;
            rgb[0] = (double) *input_pixel_ptr++;

            int pixel_cluster = pixel_cluster_group[row][col];
            double rgb_mean_diff = malahidan_distance(rgb, gaussian_means[pixel_cluster], RGB_COMPONENTS_NUM);
            standard_deviation[pixel_cluster] += rgb_mean_diff;
        }
    }

    //Count final standard deviation
    for(int i = 0; i < clusters_num; ++i)
    {
        standard_deviation[i] *= ((double) cluster_elements[i])/RGB_COMPONENTS_NUM;
    }

    //Initialize gaussians with generated value and free memory
    for(int row = 0; row < height; ++row)
    {
        delete [] pixel_cluster_group[row];
        for(int col = 0; col < width; ++col)
        {
            pixels[row][col].frame_init(weight, gaussian_means, standard_deviation);
        }
    }

    delete [] pixel_cluster_group;
    delete [] cluster_elements;
    delete [] weight;
    delete [] standard_deviation;
    for(int i = 0; i < clusters_num; ++i)
    {
        delete [] gaussian_means[i];
    }
    delete [] gaussian_means;
}
