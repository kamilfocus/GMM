#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

const int frame_num = 1700;

string get_next_frame_name(int frame_id)
{
	string frame_name_mask = "in000000.jpg";
	int name_offset = 7;
	int loop_counter = 0;
	while(frame_id > 0 && loop_counter < 6)
	{
		int new_digit = frame_id % 10;
		frame_name_mask[name_offset] = (char)('0' + new_digit);
		frame_id /= 10;
		name_offset--;
		loop_counter++;
	}
	return frame_name_mask;
}

int main( int argc, char** argv )
{
  const string frame_name_prefix = "highway/";
  string frame_name;

  Mat image;
  namedWindow("Display Image", CV_WINDOW_AUTOSIZE);

  for(int frame_id = 1; frame_id < frame_num; frame_id++)
  {
	  frame_name = frame_name_prefix + get_next_frame_name(frame_id);
	  image = imread(frame_name, 1);
	  imshow( "Display Image", image );
	  waitKey(10);	//experimental value ~~~63fps
  }

  return 0;
}
