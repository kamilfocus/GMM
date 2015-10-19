#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <cstdarg>

#include "file_name_generator.hpp"
#include "window_manager.hpp"
#include "mixture_of_gaussians.hpp"

using namespace cv;
using namespace std;

const int windows_num = 4;

#define DEBUG
#define TEST_GROUP      0

#if TEST_GROUP == 0

const int frame_num = 1700;
const string input_frame_prefix = "highway/input/in";
const string gt_frame_prefix = "highway/groundtruth/gt";

#elif TEST_GROUP ==  1

const int frame_num = 2050;
const string input_frame_prefix = "test/office/input/in";
const string gt_frame_prefix = "test/office/groundtruth/gt";

#elif TEST_GROUP ==  2

const int frame_num = 1099;
const string input_frame_prefix = "test/pedestrians/input/in";
const string gt_frame_prefix = "test/pedestrians/groundtruth/gt";

#elif TEST_GROUP ==  3

const int frame_num = 1200;
const string input_frame_prefix = "test/PETS2006/input/in";
const string gt_frame_prefix = "test/PETS2006/groundtruth/gt";

#elif TEST_GROUP ==  4

const int frame_num = 1189;
const string input_frame_prefix = "test/canoe/input/in";
const string gt_frame_prefix = "test/canoe/groundtruth/gt";

#elif TEST_GROUP ==  5

const int frame_num = 4000;
const string input_frame_prefix = "test/fall/input/in";
const string gt_frame_prefix = "test/fall/groundtruth/gt";

#elif TEST_GROUP ==  6

const int frame_num = 1184;
const string input_frame_prefix = "test/fountain01/input/in";
const string gt_frame_prefix = "test/fountain01/groundtruth/gt";


#endif

void convert2grayscale(const Mat & input, Mat & output)
{
    output = input.clone();

    double rgb[3];
    const uchar * input_pixel_ptr;
    uchar * result_pixel_ptr;
    uchar gray_value;

    int height = input.rows;
    int width = input.cols;
    for(int row = 0; row < height; ++row)
    {
        input_pixel_ptr = input.ptr(row);
        result_pixel_ptr = output.ptr(row);
        for(int col = 0; col < width; ++col)
        {
            //RGB reverted order
            rgb[2] = (double) *input_pixel_ptr++;
            rgb[1] = (double) *input_pixel_ptr++;
            rgb[0] = (double) *input_pixel_ptr++;
            //gray_value = (rgb[0]+rgb[1]+rgb[2])/3;
            gray_value = 0.2126*rgb[0]+0.7152*rgb[1]+0.0722*rgb[2];

            for(int i = 0; i < 3; ++i)
                *result_pixel_ptr++ = gray_value;
        }
    }
}

void print_image(const Mat & image, int my_row = -1, int my_col = -1)
{
    int rows = image.rows;
    int cols = image.cols;
    int bgr[3];
    if(my_row == -1 && my_col == -1)
        cout<<"Image size: "<<rows<<" x "<<cols<<endl;
    for(int row = 0; row < rows; ++row)
    {
        const uchar* p = image.ptr(row);
        for(int col = 0; col < cols; ++col)
        {
            //points to each pixel B,G,R value in turn assuming a CV_8UC3 color image
            for(int i=0; i < 3; ++i)
            {
                bgr[i] = (int) *p++;
            }
            if((my_row == -1 && my_col == -1) || (my_row == row && my_col == col))
            {
                cout<<"["<<row<<"]["<<col<<"]: ";
                cout<<"("<<bgr[2]<<", "<<bgr[1]<<", "<<bgr[0]<<")\t";
            }
        }
        if(my_row == -1 && my_col == -1)
            cout<<endl;
    }
}

int main(int argc, char** argv)
{
    MixtureOfGaussians MoG(5, 0.01, 0.5 , 3);//(gaussians_num, learning_rate, T, std_deV)

    FileNameGenerator input_file_name_generator(input_frame_prefix, JPG);
    FileNameGenerator ground_truth_file_name_generator(gt_frame_prefix, PNG);

    initialize_windows();

    Mat input_frame, gray_input_frame, gt_frame, cv_mixture_of_gaussians_frame, output_frame;
    string frame_name, gt_name;

    Ptr< BackgroundSubtractor> cv_mixture_of_gaussians;
    cv_mixture_of_gaussians = createBackgroundSubtractorMOG2();

#ifdef DEBUG
    const int observed_x = 120;
    const int observed_y = 180;
    const uchar RED_COLOR[3] = {255,0,0};
#endif

    for(int frame_id = 1; frame_id < frame_num; frame_id++)
    {
        frame_name = input_file_name_generator.get_frame_name(frame_id);
        gt_name = ground_truth_file_name_generator.get_frame_name(frame_id);
        input_frame = imread(frame_name, 1);
        convert2grayscale(input_frame, gray_input_frame);
        gt_frame = imread(gt_name, 1);
        cv_mixture_of_gaussians->apply(input_frame, cv_mixture_of_gaussians_frame);
        MoG.update(gray_input_frame, output_frame);

#ifdef DEBUG
        print_image(gray_input_frame, observed_x , observed_y );
        MoG.print_parameters(observed_x , observed_y );
        uchar* p = output_frame.ptr(observed_x);
        for(int i=0; i < 3; ++i)
        {
            p[3*observed_y + i] = RED_COLOR[2-i];
        }
#endif

        update_windows(windows_num, &input_frame, &cv_mixture_of_gaussians_frame, &gray_input_frame, &output_frame);
        if(waitKey(10) != -1)//experimental value ~63fps
            break;
    }

    return 0;
}
