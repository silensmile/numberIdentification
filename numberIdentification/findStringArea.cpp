#include"header.h"

Mat separateChannel(Mat const src);
vector<Rect> calcExternalRect(Mat const mask_contours);

//寻找数字串区域的函数，返回矩形轮廓
vector<Rect> findStringArea(Mat const src)
{
	Mat temp_mask;
	//通道分离,红色通道对比度最高
	Mat dst = separateChannel(src);

	//手动阈值化
	threshold(dst, dst, 155, 255, CV_THRESH_BINARY);

	//来一波开闭
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7), Point(-1, -1));	//圆核，效果较好
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);
	dilate(dst, dst, kernel, Point(-1, -1), 1, 0);
	dilate(dst, dst, kernel, Point(-1, -1), 1, 0);
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);

	//分离数字串用的模板
	boxFilter(dst, temp_mask, -1, Size(81, 81));
	threshold(temp_mask, temp_mask, 20, 255, CV_THRESH_BINARY);
	//除一些干扰，长条形核来开闭
	Mat kernel_mask = getStructuringElement(MORPH_RECT, Size(50, 1), Point(-1, -1));
	erode(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);
	dilate(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);
	dilate(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);
	erode(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);

	kernel_mask = getStructuringElement(MORPH_RECT, Size(20, 20), Point(-1, -1));
	erode(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);

	//计算外接矩形
	Mat mask_contours;
	temp_mask.copyTo(mask_contours);
	vector<Rect> boundRect = calcExternalRect(mask_contours);

	return boundRect;
}