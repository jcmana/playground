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
	// Commandline parameters
	string arg_imagename1 = "";
	string arg_imagename2 = "";
	string arg_imageoutput = "";

	// Get names of images to join
	if (argc >= 3)
	{
		arg_imagename1 = argv[1];
		arg_imagename2 = argv[2];
	}

	// Get name of output image
	if (argc >= 4)
	{
		arg_imageoutput = argv[3];
	}


	// Load images from disk
	Mat image1 = imread(arg_imagename1, CV_64F);
	Mat image2 = imread(arg_imagename2, CV_64F);
	Mat image_result = image1.clone();

	// Color for ignored pixels
	Vec3b mask_color = Vec3b(255, 255, 0);

	// Flag, indicating if something was changed in original image
	bool flag_changed = false;

	// Join ignored areas from image2 with image1
	for (int row = 0; row < image1.rows; ++row)
	{
		for (int col = 0; col < image1.cols; ++col)
		{
			// Check, whether this pixel is mask colored and also if this is not already ignored in base image(image1)
			if (image2.at<Vec3b>(row, col) == mask_color && image1.at<Vec3b>(row, col) != mask_color)
			{
				// Set new ignored pixel in result image(image_result)
				image_result.at<Vec3b>(row, col) = mask_color;

				// Set that result image differs from the base one
				flag_changed = true;
			}
		}
	}

	// Save the result image
	cvSaveImage(arg_imageoutput.c_str(), &(IplImage(image_result)));

	// Check if the result image differs from the base one
	if (flag_changed)
	{
		return 1;
	}

	// Nothing has changed
	return 0;
}
