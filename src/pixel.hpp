#ifndef SRC_PIXEL_HPP_
#define SRC_PIXEL_HPP_

#include "gaussian.hpp"
#include "tools.hpp"

using namespace std;

class Pixel
{
    private:
		static double T;

        int k;
        Gaussian *gaussian_ptr;

        void print_error(int gaussian_num);
    public:
        void init(int k, double alpha, uchar **gaussian_means = NULL);
        void frame_init(double *weight, double **gaussian_means, double *standard_devation);
        void frame_init(Gaussian* gaussian_ptr);
        void get_rgb_mean(int gaussian_num, double * gaussian_means);
        void sort(double bg_classifier); //Parameter T from article
        void print(int gaussian_num = -1);
        bool is_foreground(double * rgb);
        Pixel & operator=(const Pixel & pixel);
        double get_max_deviation();

        void pixel_init(double * rgb, int frame_num);

        static void set_T(double T) { Pixel::T = T; }

};

#endif /* SRC_PIXEL_HPP_ */
