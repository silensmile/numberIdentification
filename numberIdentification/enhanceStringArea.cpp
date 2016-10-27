#include"header.h"

//适度的图像“增强”，你懂的
void enhanceStringArea(vector<Mat> parts)
{
	Mat shadow1 = Mat(Size(parts[1].cols - 450, parts[1].rows), parts[1].type(), Scalar(CV_RGB(0, 0, 0)));
	addWeighted(shadow1, 0.17, parts[1](Rect(450, 0, parts[1].cols - 450, parts[1].rows)), 0.83,
		0, parts[1](Rect(450, 0, parts[1].cols - 450, parts[1].rows)));
	Mat shadow2 = Mat(Size(parts[2].cols - 465, parts[2].rows), parts[2].type(), Scalar(CV_RGB(0, 0, 0)));
	addWeighted(shadow2, 0.23, parts[2](Rect(465, 0, parts[2].cols - 465, parts[2].rows)), 0.77,
		0, parts[2](Rect(465, 0, parts[2].cols - 465, parts[2].rows)));
	Mat light2 = Mat(Size(36, 16), parts[2].type(), Scalar(255, 255, 255));
	addWeighted(light2, 0.1, parts[2](Rect(480, 55, 36, 16)), 0.9, 0,
		parts[2](Rect(480, 55, 36, 16)));
}