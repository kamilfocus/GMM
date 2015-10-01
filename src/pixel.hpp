#ifndef SRC_PIXEL_HPP_
#define SRC_PIXEL_HPP_

#include "gaussian.hpp"

class Pixel
{
    private:
        int k;
        Gaussian *gaussian_ptr;
    public:
        void init(int k, double alpha);
        bool is_foreground(int red, int green, int blue);
};

#endif /* SRC_PIXEL_HPP_ */
