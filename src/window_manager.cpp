#include <opencv2/highgui/highgui.hpp>
#include "window_manager.hpp"

void initialize_windows()
{
	namedWindow(INPUT, CV_WINDOW_AUTOSIZE);
	namedWindow(LIB_MOG_1, CV_WINDOW_AUTOSIZE);
	namedWindow(GROUND_TRUTH, CV_WINDOW_AUTOSIZE);
	moveWindow(INPUT, WINDOW_PADDING, WINDOW_PADDING);
	moveWindow(LIB_MOG_1, WINDOW_PADDING + WINDOW_X_POS_OFFSET, WINDOW_PADDING);
	moveWindow(GROUND_TRUTH, WINDOW_PADDING, WINDOW_PADDING + WINDOW_Y_POS_OFFSET);
}

void update_windows(int windows_num, ...)
{
    va_list args;
    va_start(args, windows_num);
    Mat *img;
    img = va_arg(args, Mat *);
	imshow(INPUT, *img);
	img = va_arg(args, Mat *);
	imshow(LIB_MOG_1, *img);
	img = va_arg(args, Mat *);
	imshow(GROUND_TRUTH, *img);
}
