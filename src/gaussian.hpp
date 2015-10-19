#ifndef SRC_GAUSSIAN_HPP_
#define SRC_GAUSSIAN_HPP_

#include <opencv2/core/core.hpp>
using namespace cv;

class Gaussian
{
    private:
		static double initial_variance;
		static double alpha;

		double weight;
        double *rgb_mean;
        double standard_deviation; //same for all RGB values, sigma without square
        int matchsum;
        bool foreground;
        double get_sort_parameter() const;

    public:
        void initialise(double weight, double *gaussian_mean, double standard_deviation);
        void get_rgb_mean(double * gaussian_means);
        double get_weight() { return weight; }
        int get_matchsum() {return matchsum; }
        void print();
        void update_unmatched();
        void update_matched(double *rgb);
        bool check_pixel_match(double *rgb);

        bool operator<(const Gaussian& gaussian) const;
        void set_isForeground(bool foreground) { this->foreground = foreground; }
        bool isForeground() { return foreground; }
        double get_deviation() { return standard_deviation; }

        static double& get_initial_variance() { return Gaussian::initial_variance; }
        static void set_alpha(double alpha) {Gaussian::alpha = alpha; }
};


#endif /* SRC_GAUSSIAN_HPP_ */
