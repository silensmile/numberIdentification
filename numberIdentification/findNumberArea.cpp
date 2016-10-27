#include"header.h"

Mat separateChannel(Mat const src);
vector<Rect> calcExternalRect(Mat const mask_contours);
vector<Rect> calcCenterAndStd(vector<Rect> dstRect);

#if findNumberAreaWay == 0
//寻找数字串中单个数字的区域范围，返回矩形轮廓
vector<Rect> findNumberArea(Mat const src, int thresholdVal, Size kernelSize1, Size kernelSize2)
{
	//通道分离
	Mat dst = separateChannel(src);
	//手动阈值化
	threshold(dst, dst, thresholdVal, 255, CV_THRESH_BINARY);

	//Mat temp_mask;
	//boxFilter(dst, temp_mask, -1, Size(5, 5));
	//threshold(temp_mask, temp_mask, 30, 255, CV_THRESH_BINARY);

	//腐蚀膨胀操作
	Mat kernel = getStructuringElement(MORPH_RECT, kernelSize1, Point(-1, -1));	//横向矩形核
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);
	kernel = getStructuringElement(MORPH_RECT, kernelSize2, Point(-1, -1));	//纵向矩形核
	dilate(dst, dst, kernel, Point(-1, -1), 1, 0);

	//imshow("show", dst);

	//计算外接矩形
	Mat mask_contours;
	dst.copyTo(mask_contours);
	vector<Rect> dstRect = calcExternalRect(mask_contours);

	//按标号排序
	std::sort(dstRect.begin(), dstRect.end(), 
		[](Rect rec1, Rect rec2) {return rec1.x < rec2.x; }
	);

	//计算矩形中心并宽度标准化
	dstRect = calcCenterAndStd(dstRect);

	return dstRect;
}
#endif

#if findNumberAreaWay == 1
//寻找数字串中单个数字的区域范围，返回矩形轮廓
vector<Rect> findNumberArea(Mat const src, int thresholdVal, Size kernelSize1, Size kernelSize2)
{
	//通道分离
	Mat dst = separateChannel(src);

	//局部自适应阈值
	adaptiveThreshold(dst, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 51, thresholdVal);

	//Mat temp_mask;
	//boxFilter(dst, temp_mask, -1, Size(5, 5));
	//threshold(temp_mask, temp_mask, 30, 255, CV_THRESH_BINARY);

	//腐蚀膨胀操作
	Mat kernel = getStructuringElement(MORPH_RECT, kernelSize1, Point(-1, -1));	//横向矩形核
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);
	kernel = getStructuringElement(MORPH_RECT, kernelSize2, Point(-1, -1));	//纵向矩形核
	dilate(dst, dst, kernel, Point(-1, -1), 1, 0);
	kernel = getStructuringElement(MORPH_RECT, Size(8,1), Point(-1, -1));
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);

	//imshow("show", dst);

	//计算外接矩形
	Mat mask_contours;
	dst.copyTo(mask_contours);
	vector<Rect> dstRect = calcExternalRect(mask_contours);

	//按标号排序
	std::sort(dstRect.begin(), dstRect.end(),
		[](Rect rec1, Rect rec2) {return rec1.x < rec2.x; }
	);

	//计算矩形中心并宽度标准化
	dstRect = calcCenterAndStd(dstRect);

	return dstRect;
}
#endif
