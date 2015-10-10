#ifndef SRC_MIXTURE_OF_GAUSSIANS_HPP_
#define SRC_MIXTURE_OF_GAUSSIANS_HPP_

#include <opencv2/core/core.hpp>
using namespace cv;

#include "pixel.hpp"
#include "tools.hpp"

const uchar BLACK = (uchar) 0;
const uchar WHITE = (uchar) 255;

class MixtureOfGaussians
{
    private:
        Pixel **pixels;
        int height, width;
        int k;
        bool is_initialized;

        void initialise(const Mat & input_frame);
    public:
        MixtureOfGaussians(int k, double alpha, double bg_classifier, int learning_frames_num);
        ~MixtureOfGaussians();

        void update(const Mat & input_frame, Mat & result_frame);

        void print_parameters(int row = -1, int col = -1, int gaussian_num = -1);
};

#endif /* SRC_MIXTURE_OF_GAUSSIANS_HPP_ */
