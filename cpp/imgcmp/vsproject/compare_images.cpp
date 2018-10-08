#include "compare_images.h"

bool compare_pixels(const Vec3b & pixel1, const Vec3b & pixel2, int tolerance)
{
	return ((abs((int)(pixel1[0]) - (int)(pixel2[0])) <= tolerance) && (abs((int)(pixel1[1]) - (int)(pixel2[1])) <= tolerance) && (abs((int)(pixel1[2]) - (int)(pixel2[2])) <= tolerance));
}

int compare_images(const Mat & image1, const Mat & image2, int tolerance, Mat * imagediff, const mask * compmask)
{
	// number of differing pixels
	int difference = 0;

	// pixel comparison flags
	bool flag_different;
	bool flag_ignored;

	// run comparison pixel by pixel
	for (int row = 0; row < image1.rows; ++row)
	{
		for (int col = 0; col < image1.cols; ++col)
		{
			// default flag values
			flag_different = false;
			flag_ignored = false;

			// currently processed pixels
			Vec3b pixel_image1 = image1.at<Vec3b>(row, col);
			Vec3b pixel_image2 = image2.at<Vec3b>(row, col);

			// are current pixels different?
			if (!compare_pixels(pixel_image1, pixel_image2, tolerance))
			{
				flag_different = true;
			}

			// is this pixel ignored?
			if (compmask != nullptr && compmask->get(row, col))
			{
				flag_ignored = true;
			}

			// should we generate image with differences?
			if (imagediff != nullptr)
			{
				// pixels are ignored but not different
				if (flag_ignored)
				{
					// mark ignored area into diff image
					imagediff->at<Vec3b>(row, col) = Vec3b(255 - pixel_image1[0], 255 - pixel_image1[1], 255 - pixel_image1[2]);
				}
				// pixels are not ignored and are different
				else
				if (flag_different)
				{
					// mark difference into diff image
				    imagediff->at<Vec3b>(row, col) = Vec3b(255, 255, 0);
				}
			}

			// evaluate difference
			if (flag_different && !flag_ignored)
			{
				++difference;
			}
		}
	}

	return difference;
}