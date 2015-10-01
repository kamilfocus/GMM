#ifndef SRC_MIXTURE_OF_GAUSSIANS_HPP_
#define SRC_MIXTURE_OF_GAUSSIANS_HPP_

#include <opencv2/core/core.hpp>
using namespace cv;

#include "pixel.hpp"

const uchar BLACK = (uchar) 0;
const uchar WHITE = (uchar) 255;
const int RGB_COMPONENTS_NUM = 3;

const int NONE = 0;

class MixtureOfGaussians
{
    private:
        Pixel **pixels;
        int height, width;
        int clusters_num;
        bool is_initialized;

        void paint_foreground(const uchar ** const input_pixel_ptr,
                              uchar ** const result_pixel_ptr,
                              int row, int col);
    public:
        /* @brief Constructor for Mixture of Gaussians algorithm
         *
         * @param[in]   clusters_num    number of clusters in K-means algorithm
         *                              (used to initialize Gaussians parameters)
         * @param[in]   k               number of Gaussians for each pixel
         * @param[in]   alpha           learning rate for each Gaussian
        */
        MixtureOfGaussians(int clusters_num, int k, double alpha);
        Mat update(const Mat & input_frame);
};

#endif /* SRC_MIXTURE_OF_GAUSSIANS_HPP_ */
