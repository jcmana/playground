#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

#define ERR_IMAGES_NAMES	-1
#define ERR_IMAGES_SIZE		-2
#define ERR_MASK_SIZE		-3

int main(int argc, char ** argv)
{
	// Commandline parameters
	string 	imagename1;
	string 	imagename2;
	string 	maskname;
	string	diffname;
	
	// Get names of compared images
	if( argc >= 3 )
	{
		imagename1 = argv[1];
		imagename2 = argv[2];
	}
			
	// Get name of mask definition for image comparison
	if( argc >= 4 )
	{
		maskname = argv[3];
	}
	
	// Get name of difference image file
	if( argc >= 5 )
	{
		diffname = argv[4];
	}
	
	// input images
	Mat image1;
	Mat image2;
	Mat mask;
	// output images
	Mat diff;
	// flags
	bool f_comp = false;
	bool f_mask = false;
	bool f_diff = false;
	
	
	// validate commandline parameters and set flags
	if( imagename1 != "" && imagename2 != "" )
	{
		image1 = imread(imagename1,-1);
		image2 = imread(imagename2,-1);
		f_comp = true;
	}
	if( maskname != "" )
	{
		mask = imread(maskname,-1);
		f_mask = true;
	}
	if( diffname != "" )
	{
		diff = image1.clone();
		f_diff = true;
	}
	
	// check whether parameters were valid
	if( ! f_comp )
	{
		cerr << "Images for comparison must be set." << endl;
		return ERR_IMAGES_NAMES;
	}
	
	// compare sizes of images
	if (	(image1.size().width != image2.size().width) ||
		(image1.size().height != image2.size().height) )
	{
		cerr << "Size of input images doesn't match." << endl;
		return ERR_IMAGES_SIZE;
	}
	
	// compare size of mask and images
	if (	(maskname != "") &&
		(	(mask.size().width != image1.size().width) ||
			(mask.size().height != image1.size().height) ) )
	{
		cerr << "Size of images doesn't match the size of mask." << endl;
		return ERR_MASK_SIZE;
	}
	
	// result of comparison
	bool comparison_result = true;
	
	// pixel comparison flags
	bool f_alter_rslt = false;
	bool f_alter_ignored = false;
	
	// run comparison pixel by pixel
	for(int x = 0; x < image1.rows; ++x)
	{
		for(int y = 0; y < image1.cols; ++y)
		{
			// compare pixels from both images
			if( image1.at<Vec3b>(x,y) != image2.at<Vec3b>(x,y) )
			{
				// check if they are masked
				if( false )
				{
					if( mask.at<Vec3b>(x,y) != Vec3b(255,0,255) )
					{
						// change result to false
						f_alter_rslt = true;
					}
					else
					{
						// mark ignored area onto diff image
						f_alter_ignored = true;
					}
				}
				else
				{
					// change result to false
					f_alter_rslt = true;
				}
			}
			
			// process the result of pixel comparison
			if( f_alter_rslt )
			{
				// change result of comparison to false
				comparison_result = false;
				
				// write changes to diff file if flag is set
				if( f_diff )
				{	
					// mark difference(#FF0000)
					//diff.at<Vec3b>(x,y) = Vec3b(0,0,255);
					diff.at<Vec3b>(x,y) = diff.at<Vec3b>(x,y);
				}
				
			}
			else
			{
				// mark ignored areas(#FF00FF)
				if( f_diff && f_alter_ignored )
				{
					//diff.at<Vec3b>(x,y) = Vec3b(255,0,255);
				}
			}
			
			// reset comparison flags
			f_alter_rslt = false;
			f_alter_ignored = false;
		}
	}

	
	// write result on stdout
	if( comparison_result )
	{
		cout << "Images are identical" << endl;
	}
	else
	{
		cout << "Images are different." << endl;
		
		// write diff image
		if( f_diff )
		{
			imshow("Difference",diff);
			//imwrite(diffname,diff);
		}
	}
	
	waitKey();
	
	return 0;
}
