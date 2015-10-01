#ifndef SRC_GAUSSIAN_HPP_
#define SRC_GAUSSIAN_HPP_

class Gaussian
{
    private:
        double alpha;
        double weigth;
        double r_mean;
        double g_mean;
        double b_mean;
        double standard_deviation; //same for all RGB values
        bool foreground;
    public:
        void init(double alpha);
        bool update();
};

#endif /* SRC_GAUSSIAN_HPP_ */
