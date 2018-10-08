#include "mask.h"

using namespace std;
using namespace cv;

mask::mask()
{
	// initial mask - nothing ignored
	this->setsize(0, 0);
	// default mask color for ignored areas
	this->__mask_color = Vec3b(255, 0, 255);
}
mask::mask(int rows, int cols)
{
	// initial mask - nothing ignored
	this->setsize(rows, cols);
	// default mask color for ignored areas
	this->__mask_color = Vec3b(255, 0, 255);
}
mask::~mask()
{
	this->__mask.release();
}

void mask::setsize(int rows, int cols)
{
	this->__size_rows = rows;
	this->__size_cols = cols;

	this->__mask.rows = this->__size_rows;
	this->__mask.cols = this->__size_cols;
		
	this->reset();
}

void mask::addmask(Mat newmask)
{
	for (int row = 0; row < newmask.rows; ++row)
	{
		for (int col = 0; col < newmask.cols; ++col)
		{
			// pixel from mask equals ignored area color
			if (newmask.at<Vec3b>(row, col) == this->__mask_color)
			{
				this->__mask.at<bool>(row, col) = true;
			}
		}
	}
}

void mask::setmaskcolor(Vec3b color)
{
	this->__mask_color = color;
}


void mask::addarea(int row, int col, unsigned int height, unsigned int width, bool value)
{
	// are we trying to set value of the pixel outside of the size of mask?
	height = ((row + height) >= this->__mask.rows) ? (this->__mask.rows - row) : (height);
	width = ((col + width) >= this->__mask.cols) ? (this->__mask.cols - col) : (width);

	for (int it_row = row; it_row < (row + height); it_row++)
	{
		for (int it_col = col; it_col < (col + width); it_col++)
		{
			// set pixel in mask to requested value
			this->setpixel(it_row, it_col, value);
		}
	}
}

void mask::setpixel(int row, int col, bool value)
{
	this->__mask.at<bool>(row, col) = value;
}

void mask::reset()
{
	this->__mask.release();
	this->__mask = Mat_<bool>(this->__size_rows, this->__size_cols, false);
}

bool mask::get(int row, int col) const
{
	return (bool)(this->__mask.at<bool>(row, col));
}

int mask::getsize_rows() const
{
	return (int)(this->__size_rows);
}
int mask::getsize_cols() const
{
	return (int)(this->__size_cols);
}