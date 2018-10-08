#ifndef COMPARE_IMAGES_H
#define COMPARE_IMAGES_H

#include <stdlib.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "mask.h"

bool compare_pixels(const Vec3b & pixel1, const Vec3b & pixel2, int tolerance = 0);
int compare_images(const Mat & image1, const Mat & image2, int tolerance = 0, Mat * imagediff = nullptr, const mask * compmask = nullptr);

#endif