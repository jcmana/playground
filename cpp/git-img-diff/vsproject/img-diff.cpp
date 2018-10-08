#include <direct.h>

#include <iostream>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
	// Get current working directory
	char * cwd_buffer = _getcwd(nullptr, 0);

	// Commandline parameters
	string arg_old_name = argv[1];
	string arg_old_path = argv[2];
	string arg_old_hex = argv[3];
	string arg_old_mode = argv[4];

	string arg_new_name = argv[5];
	string arg_new_path = (string)(cwd_buffer) + "/" + arg_new_name;
	string arg_new_hex = argv[6];
	string arg_new_mode = argv[7];

	// Define image objects
	Mat image_old = imread(arg_old_path, CV_64F);
	Mat image_new = imread(arg_new_path, CV_64F);
	Mat image_diff = image_old.clone();

	cout << arg_new_path << endl;

	// Show images
	namedWindow("Image OLD", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO);
	namedWindow("Image NEW", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO);

	imshow("Image OLD", image_old);
	imshow("Image NEW", image_new);


	// Evaluate differences
	Size image_old_size = image_old.size();
	Size image_new_size = image_new.size();
	if (image_old_size != image_new_size)
	{
		cout << "Image size don't match:" << endl;
		cout << "   OLD " << image_old_size.width << "x" << image_old_size.height << endl;
		cout << "   NEW " << image_new_size.width << "x" << image_new_size.height << endl;

		return -1;
	}
	else
	{
		for (int row = 0; row < image_old.rows; ++row)
		{
			for (int col = 0; col < image_old.cols; ++col)
			{
				if (image_old.at<Vec3b>(row, col) != image_new.at<Vec3b>(row, col))
				{
					image_diff.at<Vec3b>(row, col) = Vec3b(255, 255, 0);
				}
			}
		}

		// Show image with differences
		namedWindow("Image DIFF", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO);
		imshow("Image DIFF", image_diff);
	}
	
	// Wait till user decides to continue
	waitKey(0);

	// Nothing has changed
	return 0;
}
