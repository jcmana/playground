#include <stdio.h>

#include <iostream>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;

const int color_comp_tolerance = 3;
const Vec3b pixel_color_conflict = Vec3b(255, 255, 0);

// Tolerant equality of two pixels
bool teq(const Vec3b & pixel1, const Vec3b & pixel2)
{
	return ((abs((int)(pixel1[0]) - (int)(pixel2[0])) <= color_comp_tolerance) && (abs((int)(pixel1[1]) - (int)(pixel2[1])) <= color_comp_tolerance) && (abs((int)(pixel1[2]) - (int)(pixel2[2])) <= color_comp_tolerance));
}

// Tolerant inequality of two pixels
bool tne(const Vec3b & pixel1, const Vec3b & pixel2)
{
	return !teq(pixel1, pixel2);
}


int main(int argc, char ** argv)
{
	// Get command line parameters
	string arg_image_O = argv[1];	// original-base version
	string arg_image_A = argv[2];	// local-ours-current version
	string arg_image_B = argv[3];	// remote-theirs-branch version

	// Load each images
	Mat image_O = imread(arg_image_O, CV_64F);
	Mat image_A = imread(arg_image_A, CV_64F);
	Mat image_B = imread(arg_image_B, CV_64F);

	// Prepare object for resulting images
	Mat image_X = image_O.clone();	// image with highlighted conflicts
	Mat image_M = image_O.clone();	// image from successful merge of A and B

	// Check size of both images against base
	//		- right now I don't know how to handle different sizes
	//		- if this situation comes, it is resolved as CONFLICT and left for 
	//		  user to handle manually
	//		- we will try to automatically resolve only images with
	//		  corresponding sizes
	if (image_A.size() != image_O.size() || image_B.size() != image_O.size())
	{
		cout << "Size of images does not correspond" << endl;
		cout << "    BASE:   " << image_O.size().width << "x" << image_O.size().height << endl;
		cout << "    OURS:   " << image_A.size().width << "x" << image_A.size().height << endl;
		cout << "    THEIRS: " << image_B.size().width << "x" << image_B.size().height << endl;
	}

	// Pixel conflict counter
	int conflict_counter = 0;

	// Compare the images
	for (int row = 0; row < image_O.rows; ++row)
	{
		for (int col = 0; col < image_O.cols; ++col)
		{
			Vec3b pixel_O = image_O.at<Vec3b>(row, col);
			Vec3b pixel_A = image_A.at<Vec3b>(row, col);
			Vec3b pixel_B = image_B.at<Vec3b>(row, col);

			Vec3b pixel_X = image_X.at<Vec3b>(row, col);
			Vec3b pixel_M = image_M.at<Vec3b>(row, col);

			// No change
			if (pixel_A == pixel_O && pixel_B == pixel_O)
			{
				// No action needed
			}

			// Change in OURS(A) only
			if (pixel_A != pixel_O && pixel_B == pixel_O)
			{
				// Add OURS(A) pixel to merged image
				pixel_M = pixel_A;
			}
			
			// Change in THEIRS(B) only
			if (pixel_A == pixel_O && pixel_B != pixel_O)
			{
				// Add THEIRS(B) pixel to merged image
				pixel_M = pixel_B;
			}

			// Same change in ours and theirs
			if (pixel_A != pixel_O && pixel_B != pixel_O && pixel_A == pixel_B)
			{
				// Add OURS(A) pixel to merged image
				pixel_M = pixel_A;
			}

			// CONFLICT: Change in ours and theirs
			if (pixel_A != pixel_O && pixel_B != pixel_O && pixel_A != pixel_B)
			{
				// Mark conflict in conflict image
				pixel_X = pixel_color_conflict;

				// And remember we have a merge conflict
				++conflict_counter;
			}

			// Save changed pixels to their original objects
			image_X.at<Vec3b>(row, col) = pixel_X;
			image_M.at<Vec3b>(row, col) = pixel_M;
		}
	}

	// Name of tempfile
	char * tempfile = "temp.png";

	// Announce successful merge
	if (conflict_counter == 0)
	{
		cvSaveImage(tempfile, &(IplImage(image_M)));
	}

	// Announce conflicting merge
	if (conflict_counter > 0)
	{
		cvSaveImage(tempfile, &(IplImage(image_X)));
	}

	// Copy temp file over the original
	remove(arg_image_A.c_str());
	rename(tempfile, arg_image_A.c_str());


	// Returing "failed automerge" status
	return conflict_counter;
}
