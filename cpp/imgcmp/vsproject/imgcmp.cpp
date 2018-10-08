#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "mask.h"
#include "compare_images.h"

using namespace std;
using namespace cv;

#define ERR_IMAGES_NAMES	-1
#define ERR_IMAGES_SIZE		-2
#define ERR_MASK_SIZE		-3

int main(int argc, char ** argv)
{
	// Commandline parameters
	string 	arg_imagename1 = "";
	string 	arg_imagename2 = "";
	string 	arg_comparisonmask1 = "";
	string 	arg_comparisonmask2 = "";
	string	arg_diffname = "";

	// Get names of compared images
	if (argc >= 3)
	{
		arg_imagename1 = argv[1];
		arg_imagename2 = argv[2];
	}

	// Get mask parameter
	if (argc >= 4)
	{
		arg_diffname = argv[3];
	}

	// Get mask 1 parameters
	if (argc >= 5)
	{
		arg_comparisonmask1 = argv[4];
	}

	// Get mask 2 parameters
	if (argc >= 6)
	{
		arg_comparisonmask2 = argv[5];
	}


	// Load images
	Mat image1 = imread(arg_imagename1, CV_64F);
	Mat image2 = imread(arg_imagename2, CV_64F);
	Mat diff = image1.clone();

	// Check images size
	if (image1.rows != image2.rows || image1.cols != image2.cols)
	{
		cerr << "Size of images doesn't correspong" << endl;
	}

	// Initialize mask object
	mask comparisonmask(image1.rows, image1.cols);
	comparisonmask.setmaskcolor(Vec3b(255, 255, 0));	// cyan

	// regular expression definitions
	regex re_file("^(.+\\.(png|jpg|bmp))$");

	if (arg_comparisonmask1 != "")
	{
		// is mask defined as file?
		if (regex_match(arg_comparisonmask1, re_file))
		{
			// load mask definition from image file
			comparisonmask.addmask(imread(arg_comparisonmask1, CV_64F));
		}
	}

	if (arg_comparisonmask2 != "")
	{
		// is mask defined as file?
		if (regex_match(arg_comparisonmask2, re_file))
		{
			// load mask definition from image file
			comparisonmask.addmask(imread(arg_comparisonmask2, CV_64F));
		}
	}

	// Compare images and generate diff image
	int comparisonresult = compare_images(image1, image2, 3, &diff, &comparisonmask);
	
	// Export diff image
	if (arg_diffname != "")
	{
		//imwrite(arg_diffname, diff);
		cvSaveImage(arg_diffname.c_str(), &(IplImage(diff)));
	}

	return comparisonresult;
}
