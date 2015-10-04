#ifndef SRC_GAUSSIAN_HPP_
#define SRC_GAUSSIAN_HPP_

#include <opencv2/core/core.hpp>
using namespace cv;

#define THRESHOLD (2.5) //constant threshold for pixel-Gaussian match check

class Gaussian
{
    private:
        double alpha;
        double weight;
        double *rgb_mean;
        double r_mean;
        double g_mean;
        double b_mean;
        double standard_deviation; //same for all RGB values, sigma without square
        bool isForeground;
        double get_sort_parameter() const;

    public:
        void init(double alpha, uchar *gaussian_mean);
        void frame_init(double weight, double *gaussian_mean, double standard_deviation);
        void get_rgb_mean(double * gaussian_means);
        double getWeight() { return weight; }
        void print();
        void update_unmatched();
        void update_matched(double *rgb);
        bool check_pixel_match(double *rgb);

        bool operator<(const Gaussian& gaussian) const;
        Gaussian & operator=(const Gaussian& gaussian);
        void setIsForeground(bool isForeground) { this->isForeground = isForeground; }
};

//bool gaussian_comparator(const Gaussian& x, const Gaussian& y) { return x<y; }

#endif /* SRC_GAUSSIAN_HPP_ */
