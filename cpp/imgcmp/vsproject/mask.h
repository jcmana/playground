#ifndef MASK_H
#define MASK_H

#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
using namespace std;
using namespace cv;

class mask
{
	private:
		int __size_rows;
		int __size_cols;

		Mat_<bool>	__mask;
		Vec3b		__mask_color;

	public:
		mask();
		mask(int rows, int cols);
		~mask();

		void setsize(int rows, int cols);

		void setpixel(int row, int col, bool value);
		void setmaskcolor(Vec3b color);
		void addmask(Mat newmask);
		void addarea(int row, int col, unsigned int height, unsigned int width, bool value);	

		void reset();

		bool get(int row, int col) const;

		int getsize_rows() const;
		int getsize_cols() const;
};

#endif