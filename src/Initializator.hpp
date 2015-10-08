/*
 * Initializator.h
 *
 *  Created on: 7 paź 2015
 *      Author: piotrek
 */

#ifndef SRC_INITIALIZATOR_HPP_
#define SRC_INITIALIZATOR_HPP_

#include "tools.hpp"
#include "gaussian.hpp"

class Initializator {
public:
	Initializator();
	~Initializator();
	void init(double * rgb, int current_frame);
	void clear();
	void get_gaussian_param(double ** ptr_weight, double ** ptr_deviation, double *** ptr_gaussian_means);

	static void set_init_gaussian_means(double ** init_rgb_means) { Initializator::init_gaussian_means = init_rgb_means; }
    static void set_learning_frames_num(int learning_frames_num) { Initializator::learning_frames_num = learning_frames_num; }
    static void set_gaussian_num (int gaussian_num) { Initializator::gaussians_num = gaussian_num; }

private:
	static double ** init_gaussian_means;
	static int learning_frames_num;
	static int gaussians_num;

	uchar** pixel_history;
    double **gaussian_means;
    double *weight, *standard_deviation;
    int* cluster_elements;
    int* pixel_cluster_group;





};

#endif /* SRC_INITIALIZATOR_HPP_ */
