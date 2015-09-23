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

using namespace cv;
using namespace std;

const int frame_num = 1700;
const int windows_num = 3;

int main(int argc, char** argv)
{


  FileNameGenerator input_file_name_generator("highway/input/in", JPG);
  FileNameGenerator ground_truth_file_name_generator("highway/groundtruth/gt", PNG);

  initialize_windows();

  Mat input_frame, gt_frame;
  string frame_name, gt_name;
  for(int frame_id = 1; frame_id < frame_num; frame_id++)
  {
	  frame_name = input_file_name_generator.get_frame_name(frame_id);
	  gt_name = ground_truth_file_name_generator.get_frame_name(frame_id);
	  input_frame = imread(frame_name, 1);
	  gt_frame = imread(gt_name, 1);
	  update_windows(windows_num, &input_frame, &input_frame, &gt_frame);
	  if(waitKey(10) != -1)//experimental value ~~~63fps
		  break;
  }

  return 0;
}
