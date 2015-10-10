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

const int frame_num = 1700;
const int windows_num = 4;

#define DEBUG

void print_image(Mat & image)
{
    int rows = image.rows;
    int cols = image.cols;
    int bgr[3];
    cout<<"Image size: "<<rows<<" x "<<cols<<endl;
    for(int row = 0; row < rows; ++row)
    {
        uchar* p = image.ptr(row);
        for(int col = 0; col < cols; ++col)
        {
            //points to each pixel B,G,R value in turn assuming a CV_8UC3 color image
            for(int i=0; i < 3; ++i)
            {
                bgr[i] = (int) *p++;
            }
            cout<<"["<<bgr[2]<<", "<<bgr[1]<<", "<<bgr[0]<<"]\t";
        }
        cout<<endl;
    }
}

int main(int argc, char** argv)
{
    Mat test_frame = imread("highway/input/in000001.jpg", 1);
    Mat output_frame = test_frame;

    MixtureOfGaussians MoG(3, 0.1, 0.8, 100);

    FileNameGenerator input_file_name_generator("highway/input/in", JPG);
    FileNameGenerator ground_truth_file_name_generator("highway/groundtruth/gt", PNG);

    initialize_windows();

    Mat input_frame, gt_frame, cv_mixture_of_gaussians_frame;
    string frame_name, gt_name;

    Ptr< BackgroundSubtractor> cv_mixture_of_gaussians;
    cv_mixture_of_gaussians = createBackgroundSubtractorMOG2();

#ifdef DEBUG
    const int observed_x = 120;
    const int observed_y = 180;
    const uchar COLOR[3] = {255,0,0}; //Red

#endif

    for(int frame_id = 1; frame_id < frame_num; frame_id++)
    {
        frame_name = input_file_name_generator.get_frame_name(frame_id);
        gt_name = ground_truth_file_name_generator.get_frame_name(frame_id);
        input_frame = imread(frame_name, 1);
        gt_frame = imread(gt_name, 1);
        cv_mixture_of_gaussians->apply(input_frame, cv_mixture_of_gaussians_frame);
        MoG.update(input_frame, output_frame);

#ifdef DEBUG
        MoG.print_parameters(observed_x , observed_y );
        uchar* p = output_frame.ptr(observed_x);
        for(int i=0; i < 3; ++i)
        {
            p[3*observed_y + i] = COLOR[2-i];
        }
#endif

        update_windows(windows_num, &input_frame, &cv_mixture_of_gaussians_frame, &gt_frame, &output_frame);
        if(waitKey(10) != -1)//experimental value ~~~63fps
            break;
    }

    return 0;
}
