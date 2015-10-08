/*
 * Initializator.cpp
 *
 *  Created on: 7 paź 2015
 *      Author: piotrek
 */

#include "Initializator.hpp"
#include <iostream>

double** Initializator::init_gaussian_means = NULL;
int Initializator :: learning_frames_num = 0;
int Initializator :: gaussians_num = 0;

using namespace std;

Initializator::Initializator() {

    this->gaussian_means = new double*[gaussians_num];
    this->weight = new double[gaussians_num];
    this->standard_deviation = new double[gaussians_num];
    this->cluster_elements = new int[gaussians_num];
    this->pixel_cluster_group = new int [learning_frames_num];

    for(int i = 0; i < gaussians_num; ++i)
    {
        cluster_elements[i] = 0;
        weight[i] = 0.0;
        standard_deviation[i] = 0.0;
        gaussian_means[i] = new double[RGB_COMPONENTS_NUM];
        for(int j=0; j < RGB_COMPONENTS_NUM; ++j)
            gaussian_means[i][j] = 0.0;
    }

    this->pixel_history = new uchar*[learning_frames_num];
    for(int i=0; i<learning_frames_num; i++)
    	this->pixel_history[i] = new uchar[RGB_COMPONENTS_NUM];

}

Initializator::~Initializator() {
	clear();
	for(int i=0; i<gaussians_num; i++)
		delete [] gaussian_means[i];
	delete [] gaussian_means;

    delete [] weight;
    delete [] standard_deviation;
    delete [] cluster_elements;
    delete [] pixel_cluster_group;
}

void Initializator::init(double * rgb, int current_frame)
{
	for(int i=0; i<RGB_COMPONENTS_NUM; i++)
		pixel_history[current_frame][i] = rgb[i];

    double cluster_value;
    int best_cluster_num = 0;
    double best_cluster_value = malahidan_distance(rgb, init_gaussian_means[0], RGB_COMPONENTS_NUM);

    for(int i = 1; i < gaussians_num; ++i)
    {
        cluster_value = malahidan_distance(rgb, init_gaussian_means[i], RGB_COMPONENTS_NUM);
        if(cluster_value < best_cluster_value)
        {
            best_cluster_value = cluster_value;
            best_cluster_num = i;
        }
    }

    pixel_cluster_group[current_frame] = best_cluster_num;
    cluster_elements[best_cluster_num]++;
    for(int rgb_num = 0; rgb_num < RGB_COMPONENTS_NUM; ++rgb_num)
    {
        gaussian_means[best_cluster_num][rgb_num] += rgb[rgb_num];
    }

    if(current_frame < learning_frames_num-1)
    	return;

    //Count final weights and means
//    cout<<"[";
    for(int i = 0; i < gaussians_num; ++i)
    {
        weight[i] = cluster_elements[i]/((double) learning_frames_num);
        if(weight[i]==0)
        	weight[i] = 0.01;
  //      cout << cluster_elements[i] <<" ";

        for(int rgb_num = 0; rgb_num < RGB_COMPONENTS_NUM; ++rgb_num)
        {
        	if(cluster_elements[i] != 0)
        		gaussian_means[i][rgb_num] = gaussian_means[i][rgb_num]/cluster_elements[i];
        }
    }
//    cout<<" ]"<<endl;

    //Precount standard deviation
	double temp_rgb[RGB_COMPONENTS_NUM];
    for(int i=0; i<gaussians_num; i++)
    {
    	for(int j = 0; j <learning_frames_num; ++j)
    	{
    		for(int k=0; k<RGB_COMPONENTS_NUM; k++)
    			temp_rgb[k] = (double)pixel_history[j][k];
    		double rgb_mean_diff = malahidan_distance(temp_rgb, gaussian_means[i], RGB_COMPONENTS_NUM);
    		standard_deviation[i] += rgb_mean_diff;
    	}
    }

    double initial_variance = 0;
    //Count final standard deviation
    for(int i = 0; i < gaussians_num; ++i)
    {
    	if(cluster_elements[i] != 0)
    		standard_deviation[i] /= (double) cluster_elements[i];
        standard_deviation[i] /= (double) RGB_COMPONENTS_NUM;

        if(i == 0 || initial_variance < standard_deviation[i])
        	initial_variance = standard_deviation[i];

        standard_deviation[i] = sqrt(standard_deviation[i]);
    }


    clear();

}

void Initializator::clear()
{
	if(pixel_history == NULL)
		return;

	for(int i=0; i<learning_frames_num; i++)
		delete [] pixel_history[i];
	delete [] pixel_history;
	pixel_history = NULL;
}

void Initializator::get_gaussian_param(double ** ptr_weight, double ** ptr_deviation, double *** ptr_gaussian_means)
{
	*ptr_weight = this->weight;
	*ptr_deviation = this->standard_deviation;
	*ptr_gaussian_means = this->gaussian_means;
}
