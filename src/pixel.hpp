#ifndef SRC_PIXEL_HPP_
#define SRC_PIXEL_HPP_

#include "gaussian.hpp"

class Pixel
{
    private:
        int k;
        Gaussian *gaussian_ptr;
    public:
        void init(int k, double alpha, uchar **gaussian_means);
        void frame_init(double *weight, double **gaussian_means, double *standard_devation);
        bool is_foreground(int red, int green, int blue);
};

#endif /* SRC_PIXEL_HPP_ */
