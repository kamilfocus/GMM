#ifndef SRC_GAUSSIAN_HPP_
#define SRC_GAUSSIAN_HPP_

#include <opencv2/core/core.hpp>
using namespace cv;

class Gaussian
{
    private:
        double alpha;
        double weight;
        double r_mean;
        double g_mean;
        double b_mean;
        double standard_deviation; //same for all RGB values, sigma without square
        bool foreground;
    public:
        void init(double alpha, uchar *gaussian_mean);
        void frame_init(double weight, double *gaussian_mean, double standard_deviation);
        void get_rgb_mean(double * gaussian_means);
        void print();
        bool update();
};

#endif /* SRC_GAUSSIAN_HPP_ */
