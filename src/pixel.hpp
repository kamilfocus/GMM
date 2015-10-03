#ifndef SRC_PIXEL_HPP_
#define SRC_PIXEL_HPP_

#include "gaussian.hpp"

using namespace std;

class Pixel
{
    private:
        int k;
        Gaussian *gaussian_ptr;
        void print_error(int gaussian_num);
    public:
        void init(int k, double alpha, uchar **gaussian_means);
        void frame_init(double *weight, double **gaussian_means, double *standard_devation);
        void get_rgb_mean(int gaussian_num, double * gaussian_means);
        void print(int gaussian_num = -1);
        bool is_foreground(int red, int green, int blue);
};

#endif /* SRC_PIXEL_HPP_ */
