#ifndef SRC_MIXTURE_OF_GAUSSIANS_HPP_
#define SRC_MIXTURE_OF_GAUSSIANS_HPP_

#include <opencv2/core/core.hpp>
using namespace cv;

#include "pixel.hpp"
#include "tools.hpp"

const uchar BLACK = (uchar) 0;
const uchar WHITE = (uchar) 255;

const int NONE = 0;

class MixtureOfGaussians
{
    private:
        Pixel **pixels;
        int height, width;
        int clusters_num;
        bool is_initialized;
        double alpha;
        double bg_classifier;

        void paint_foreground(const uchar * input_pixel_ptr,
                              uchar * result_pixel_ptr,
                              int row, int col);
    public:
        /* @brief Constructor for Mixture of Gaussians algorithm
         *
         * @param[in]   k               number of Gaussians for each pixel
         * @param[in]   alpha           learning rate for each Gaussian
        */
        MixtureOfGaussians(int k, double alpha, double bg_classifier);
        ~MixtureOfGaussians();

        Mat update(const Mat & input_frame);

        //Initalization

        /* @brief Generates clusters_num of RGB means (3-D vectors)
         *
         * @param[in]   deterministic   initialization mode
        */
        uchar ** generate_inital_means(bool deterministic);

        /* @brief Executes k-mean algorithm for Gaussian parameters initialization
         *
         * @param[in]   input_frame     the first frame of the movie
         * @param[out]  result_frame    Returned frame is always black
        */
        void initialize_gaussians(const Mat & input_frame, Mat & result_frame);

        void print_parameters(int row = -1, int col = -1, int gaussian_num = -1);
        void sort();
        void foreground_detection(const Mat & input_frame, Mat & result_frame);
};

#endif /* SRC_MIXTURE_OF_GAUSSIANS_HPP_ */
