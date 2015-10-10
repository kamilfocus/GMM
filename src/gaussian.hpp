#ifndef SRC_GAUSSIAN_HPP_
#define SRC_GAUSSIAN_HPP_

#include <opencv2/core/core.hpp>
using namespace cv;

//#define THRESHOLD (0.1) //constant threshold for pixel-Gaussian match check
#define THRESHOLD (2.5)
#define INITIAL_DEVIATION	11

class Gaussian
{
    private:
		static double initial_variance;

		double alpha;
        double weight;
        double *rgb_mean;
        double r_mean;
        double g_mean;
        double b_mean;
        double standard_deviation; //same for all RGB values, sigma without square
        bool foreground;
        double get_sort_parameter() const;

    public:
        void init(double alpha, uchar *gaussian_mean);
        void frame_init(double weight, double *gaussian_mean, double standard_deviation);
        void get_rgb_mean(double * gaussian_means);
        double get_weight() { return weight; }
        void print();
        void update_unmatched();
        void update_matched(double *rgb);
        bool check_pixel_match(double *rgb);

        bool operator<(const Gaussian& gaussian) const;
        Gaussian & operator=(const Gaussian& gaussian);
        void set_isForeground(bool foreground) { this->foreground = foreground; }
        bool isForeground() { return foreground; }
        double get_deviation() { return standard_deviation; }

        //static void set_initial_variance(double variance) { Gaussian::initial_variance = variance; }
        static double& get_initial_variance() { return Gaussian::initial_variance; }
};

//bool gaussian_comparator(const Gaussian& x, const Gaussian& y) { return x<y; }

#endif /* SRC_GAUSSIAN_HPP_ */
