#ifndef SRC_PIXEL_HPP_
#define SRC_PIXEL_HPP_

#include "gaussian.hpp"
#include "tools.hpp"

using namespace std;

class Pixel
{
    private:
        static double T;
        static int k;

        Gaussian *gaussian_ptr;
        int matchsum_total;

        void print_error(int gaussian_num);
        void sort(); //Parameter T from article, background classifier
        double get_max_deviation();

    public:
        static void set_T(double T) { Pixel::T = T; }
        static void set_k(double k) { Pixel::k = k; }

        void initialise(double *weight, double **gaussian_means, double *standard_devation);
        void get_rgb_mean(int gaussian_num, double * gaussian_means);
        void print(int gaussian_num = -1);
        bool is_foreground(double * rgb);
};

#endif /* SRC_PIXEL_HPP_ */
