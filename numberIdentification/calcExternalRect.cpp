#include"header.h"

//计算外接矩形
vector<Rect> calcExternalRect(Mat const mask_contours)
{
	vector<vector<Point>>contours;
	vector<Vec4i> hierarchy;
	findContours(mask_contours, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	return boundRect;
}