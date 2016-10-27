仪器光电综合实验OPENCV模糊数字识别
----
<br />
*by*	陈霁阳、戴鹏程

# 主要程序代码


### header.h

``` cpp
#pragma once
#include "opencv2/opencv.hpp"
#include "iostream"
#include "math.h"

using namespace std;
using namespace cv;

/**********************宏定义声明**********************/

#define findNumberAreaWay 1		//findNumberAreaWay == 1 局部自适应阈值化
#define NUM_HEIGHT 120
#define NUM_WIDTH 60
```

### function.h

``` cpp
#pragma once
#include "header.h"

using namespace std;
using namespace cv;

/********************全局变量声明*********************/


/**********************函数声明**********************/

//图像预处理函数（测试）
vector<Mat> preProcess(Mat const src, vector<Mat> result);

//寻找数字串区域的函数，返回矩形轮廓
vector<Rect> findStringArea(Mat const src);

//分离数字区域，返回区域的图片
Mat separateStringArea(Mat const src, Rect rectArea);

//对数字串区域排序，按照从上到下
vector<Rect> sortStringArea(vector<Rect> srcRect);

//寻找数字串中单个数字的区域范围，返回矩形轮廓
vector<Rect> findNumberArea(Mat const src, int thresholdVal, Size kernelSize1, Size kernelSize2);

void enhanceStringArea(vector<Mat> parts);

//分离数字串中的数字，返回Mat的vector
vector<Mat> separateNumberArea(Mat part, vector<Rect> number);

//识别数字
vector<char> recognizeNubmers(const vector<Mat>, const vector<Mat>, const int);

//对比度拉伸
Mat ContrastStretch(Mat const SourceImage, double min);

//通道分离并提取r通道
Mat separateChannel(Mat const src);

//计算外接矩形
vector<Rect> calcExternalRect(Mat const mask_contours);

//计算中心位置并将图像大小标准化
vector<Rect> calcCenterAndStd(vector<Rect> dstRect);
```

### main.cpp

``` cpp
#include "header.h"
#include "function.h"

//extern Mat src;

int main()
{
	/**************************************************************************/
	/*                          读入图片文件以及初始化                            */
	const Mat src = imread("./img/src.jpg");
	Mat src_changeable;
	src.copyTo(src_changeable);	//副本
	/**************************************************************************/
	//预处理尝试 for test
	//vector<Mat> temptest;
	//preProcess(src, temptest);
	/**************************************************************************/
	/*                               图片分割                                  */
	//查找区域
	vector<Rect> stringArea = findStringArea(src);

	//从上到下序号排序
	stringArea = sortStringArea(stringArea);

	//查看：展示找到的区域，用红色框框出
	for (int i = 0; i < stringArea.size(); i++)
		rectangle(src_changeable, stringArea[i], Scalar(CV_RGB(255, 0, 0)), 2, 8, 0);
	imshow("show number sequence area", src_changeable);

	//分离区域，将每串数字区域存入parts矢量，为三通道未处理图片
	vector<Mat> parts;
	vector<Mat> parts_changeable(stringArea.size());
	parts.clear();
	for (int i = 0; i < stringArea.size(); i++)
	{
		parts.push_back(separateStringArea(src, stringArea[i]));
		parts[i].copyTo(parts_changeable[i]);
	}
	//enhanceStringArea(parts_changeable);

	//寻找单个区域中的单个数字
	vector<vector<Rect>> numbers(parts.size());

	//findNumberAreaWay == 0手动阈值化，需配合enhanceStringArea，后舍弃（此处保留代码以便可能的使用）
	//numbers[0] = findNumberArea(parts_changeable[0], 190, Size(10, 2), Size(2, 300));
	//numbers[1] = findNumberArea(parts_changeable[1], 155, Size(10, 2), Size(2, 300));
	//numbers[2] = findNumberArea(parts_changeable[2], 150, Size(2, 11), Size(2, 300));

	//findNumberAreaWay == 1 局部自适应阈值化
	for(int i = 0; i < numbers.size();i++)
		numbers[i] = findNumberArea(parts_changeable[i], -20, Size(2, 15), Size(2, 300));

	//查看，将展示选择出单个数字区域的情况，用绿色框框出
	char str[6] = "test0";
	for (int i = 0; i < numbers.size(); i++)
	{
		for(int j = 0; j < numbers[i].size(); j++)
		{
			rectangle(parts_changeable[i], numbers[i][j], Scalar(CV_RGB(0, 255, 0)), 1, 8, 0);
		}
		str[4] = (char)(i + '0');
		imshow(str, parts_changeable[i]);
	}

	//分离单个数字
	vector<Mat> numbersArea0 = separateNumberArea(parts[0], numbers[0]);
	vector<Mat> numbersArea1 = separateNumberArea(parts[1], numbers[1]);
	vector<Mat> numbersArea2 = separateNumberArea(parts[2], numbers[2]);

	//查看：分别展示单个数字区域，这将打开很多窗口，注意
	char strNum[4] = "2-0";
	for (int i = 0; i < numbersArea1.size(); i++)
	{
		strNum[2] = (char)(i+'0');
		namedWindow(strNum, WINDOW_NORMAL);
		imshow(strNum, numbersArea1[i]);
	}
	/*                           图片分割部分结束                                */
	/**************************************************************************/
	/**************************************************************************/
	/*                 制作匹配模板，已经制作完成了，故下方全注释了                   */
	char modulename[40] = "./img/module0.jpg";
	//generage template
	/*char modulename2[40] = "./img/img/module0.jpg";
	for (int ii = 0;ii < 9;ii++) {
		modulename2[16] = '0' + ii;
		modulename[12] = '0' + ii;
		Mat temp = imread(modulename2, IMREAD_GRAYSCALE);
		temp = ContrastStretch(temp, 0.05);
		threshold(temp, temp, 180, 255, CV_THRESH_BINARY);
		erode(temp, temp, getStructuringElement(CV_SHAPE_ELLIPSE, Size(7, 7)));
		imwrite(modulename, temp);
	}*/
	/**************************************************************************/
	/**************************************************************************/
	/*                       匹配模板并输出结果到控制台                            */
	vector<Mat> modules;
	for (int ii = 0;ii < 9;ii++) {
		modulename[12] = '0' + ii;
		Mat temp = imread(modulename, IMREAD_GRAYSCALE);
		modules.push_back(temp);
	}
	auto nums0 = recognizeNubmers(numbersArea0, modules, 120);
	auto nums1 = recognizeNubmers(numbersArea1, modules, 120);
	auto nums2 = recognizeNubmers(numbersArea2, modules, 120);
	for (auto x : nums0) cout << x;
	cout << endl;
	for (auto x : nums1) cout << x;
	cout << endl;
	for (auto x : nums2) cout << x;
	cout << endl;

	system("pause");
	waitKey(0);

	return 0;
}
```

### preProcess.cpp

``` cpp
#include"header.h"

//图像预处理函数（测试）
vector<Mat> preProcess(Mat const src, vector<Mat> result)
{
	result.clear();
	Mat src_changeable;
	src.copyTo(src_changeable);
	Mat dst,temp,temp_mask;
	//通道分离
	vector<Mat> Vchannels;
	Mat channel_b, channel_g, channel_r;
	split(src,Vchannels);
	channel_b = Vchannels.at(0);
	channel_g = Vchannels.at(1);
	channel_r = Vchannels.at(2);

	channel_r.copyTo(dst);				//红色通道对比度最高

	//addWeighted(channel_g, 0.5, channel_r, 0.5, 0, temp);

	//拉普拉斯算子锐化，效果不好，放弃
	//Mat sharpenKernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	//filter2D(dst, dst, dst.depth(), sharpenKernel);

	//高提升锐化，效果过得去，拟采用
	//Mat mask;
	//GaussianBlur(dst, mask, Size(17, 17), 0, 0);
	//mask = dst - mask;
	//dst = dst + mask * 4;

	//全局阈值化，效果不好，放弃
	//adaptiveThreshold(dst, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 51, -20);

	//手动阈值化
	threshold(dst, dst, 155, 255, CV_THRESH_BINARY);

	//来一波开闭
	//Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));	//方形核
	//Mat kernel = getStructuringElement(MORPH_CROSS, Size(7, 7), Point(-1, -1));	//十字核
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7), Point(-1, -1));	//圆核，效果较好
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);
	dilate(dst, dst, kernel, Point(-1, -1), 1, 0);
	dilate(dst, dst, kernel, Point(-1, -1), 1, 0);
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);


	/******************************************************************************/
	/*******************整体阈值处理效果不好,接下来考虑分开处理***************************/
	/******************************************************************************/

	//分离数字串用的模板
	boxFilter(dst, temp_mask, -1, Size(81,81));
	threshold(temp_mask, temp_mask, 20, 255, CV_THRESH_BINARY);
	Mat kernel_mask = getStructuringElement(MORPH_RECT, Size(50, 1), Point(-1, -1));
	erode(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);
	dilate(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);
	dilate(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);
	erode(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);

	kernel_mask = getStructuringElement(MORPH_RECT, Size(20, 20), Point(-1, -1));
	erode(temp_mask, temp_mask, kernel_mask, Point(-1, -1), 1, 0);

	Mat mask_contours;
	temp_mask.copyTo(mask_contours);
	vector<vector<Point>>contours;
	vector<Vec4i> hierarchy;
	findContours(mask_contours, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		rectangle(src_changeable, boundRect[i], Scalar(CV_RGB(255,0,0)), 2, 8, 0);
		result.push_back(channel_r(Rect(boundRect[i])));
	}
	imshow("show number area", src_changeable);

	//数字部分分别处理
	threshold(result[0], result[0], 148, 255, CV_THRESH_BINARY);
	kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(-1, -1));	//圆核，效果较好
	erode(result[0], result[0], kernel, Point(-1, -1), 1, 0);
	dilate(result[0], result[0], kernel, Point(-1, -1), 1, 0);
	dilate(result[0], result[0], kernel, Point(-1, -1), 1, 0);
	erode(result[0], result[0], kernel, Point(-1, -1), 1, 0);

	return result;
}
```

### findStringArea.cpp

``` cpp
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
```

### separateStringArea.cpp

``` cpp
#include"header.h"

//分离数字区域，返回区域的图片
Mat separateStringArea(Mat const src, Rect rectArea)
{
	Mat area;
	src(Rect(rectArea)).copyTo(area);
	return area;
}
```

### sortStringArea.cpp

``` cpp
#include"header.h"

//对数字串区域排序，按照从上到下
vector<Rect> sortStringArea(vector<Rect> srcRect)
{
	Rect temp;
	//冒泡
	int i = 0, j = 0;
	for (i = 0; i < srcRect.size() - 1; i++)
	{
		for (j = 1; j < srcRect.size(); j++)
		{
			if (srcRect[i].y > srcRect[j].y)
			{
				temp = srcRect[i];
				srcRect[i] = srcRect[j];
				srcRect[j] = temp;
			}
		}
	}
	return srcRect;
}

/*C++中的匿名函数方法*/
/*	
	//按标号排序
	std::sort(dstRect.begin(), dstRect.end(),
		[](Rect rec1, Rect rec2) {return rec1.x < rec2.x; }
	);
*/
```

### separateChannel.cpp

``` cpp
#include"header.h"

//通道分离并提取r通道
Mat separateChannel(Mat const src)
{
	Mat dst;
	vector<Mat> Vchannels;
	Mat channel_b, channel_g, channel_r;
	split(src, Vchannels);
	channel_b = Vchannels.at(0);
	channel_g = Vchannels.at(1);
	channel_r = Vchannels.at(2);
	channel_r.copyTo(dst);				//红色通道对比度最高
	return dst;
}
```

### findNumberArea.cpp

``` cpp
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

```

### separateNumberArea.cpp

``` cpp
#include"header.h"

//分离数字串中的数字，返回Mat的vector
vector<Mat> separateNumberArea(Mat part, vector<Rect> number)
{
	vector<Mat> numberArea(number.size());
	for (int i = 0; i < number.size(); i++)
		part(Rect(number[i])).copyTo(numberArea[i]);
	return numberArea;
}
```

### calcExternalRect.cpp

``` cpp
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
```

### calcCenterAndStd.cpp

``` cpp
#include"header.h"

//计算中心位置并将图像大小标准化
vector<Rect> calcCenterAndStd(vector<Rect> dstRect)
{
	for (int i = 0; i < dstRect.size(); i++)
	{
		Point center(dstRect[i].x + 0.5 * dstRect[i].width, dstRect[i].y + 0.5 * dstRect[i].height);
		dstRect[i].x = center.x - NUM_WIDTH / 2;
		dstRect[i].width = NUM_WIDTH;
		dstRect[i].y = center.y - NUM_HEIGHT / 2;
		dstRect[i].height = NUM_HEIGHT;
	}

	return dstRect;
}
```

### recognizeNumber.cpp

``` cpp
#include"header.h"

//对比度拉伸
Mat ContrastStretch(Mat const SourceImage, double min)
{
	Mat hist;
	int histSize = 256;
	calcHist(&SourceImage, 1, 0, Mat(), hist, 1, &histSize, 0);
	//cout << hist << endl;

	int data_min = 0, data_max = 255;
	int accumulate = 0, minValue = static_cast<int>(SourceImage.rows * SourceImage.cols * min);
	for (accumulate = 0; data_min < histSize; data_min++) {
		accumulate += (hist.at<float>(data_min));
		if (accumulate > minValue)
			break;
	}

	for (accumulate = 0; data_max >= 0; data_max--)
	{
		accumulate += hist.at<float>(data_max);
		if (accumulate > minValue)
			break;
	}

	//cout << data_min << " " << data_max << endl;

	Mat lookUp(1, 256, CV_8U);
	int len = data_max - data_min;

	if (len < 1) return SourceImage;
	//剔除直方图两边并将中间拉伸
	for (int i = 0; i < 256; i++)
	{
		if (i < data_min) lookUp.at<uchar>(i) = 0;
		else if (i > data_max) lookUp.at<uchar>(i) = 255;
		else lookUp.at<uchar>(i) = static_cast<uchar>(255.0*(i - data_min) / len);
	}

	Mat resultImage;
	//查找表，灰度值映射
	LUT(SourceImage, lookUp, resultImage);
	return resultImage;
}

//计算图像和模板之间的相关度
double relativity(const Mat img, const Mat module)
{
	//cout << img << endl;
	//cout << module << endl;
	/*Mat temp1 = (img / 240),temp2 = (module / 240);
	Mat temp3 = temp1.mul(temp2), temp4 = temp1 + temp2 - 2*temp3;
	Mat temp5 = temp3 * 255, temp6 = temp4 * 255;
	double te = sum(temp3+temp4)[0];*/
	Mat result, srcsca(Size(img.cols * 2, img.rows * 2), CV_8U);
	img.copyTo(srcsca(Rect(img.cols /2, img.rows / 2, img.cols, img.rows)));
	matchTemplate(srcsca, module, result, CV_TM_CCOEFF_NORMED);
	//cout << result;
	double min, max;
	//检索最值
	minMaxLoc(result, &min, &max);
	return max;
}

//识别
char recognize(const Mat src, const vector<Mat> modules, const int thre)
{
	CV_DbgAssert(src.rows == NUM_HEIGHT && src.cols == NUM_WIDTH);	//检查图片大小
	Mat dst;
	cvtColor(src, dst, COLOR_BGR2GRAY);
	dst = ContrastStretch(dst, 0.05);
	threshold(dst, dst, 180, 255, CV_THRESH_BINARY);
	erode(dst, dst, getStructuringElement(CV_SHAPE_ELLIPSE, Size(11, 11)));
	//imshow("recog", dst);
	//waitKey();
	vector<double> rel;
	for (auto x : modules)
	{
		rel.push_back(relativity(dst, x));
	}
	
	double max = 0;
	char res=0;
	
	for (int ii = 0;ii < rel.size();++ii)
	{
		if (rel[ii] > max) {
			res = '0' + ii;
			max = rel[ii];
		}
	}
	if (res == '8')
		res = '9';
	return res;
}

vector<char> recognizeNubmers(const vector<Mat> srcList, const vector<Mat> modules, const int thre)
{
	vector<char> res;
	for (auto x : srcList)
		res.push_back(recognize(x, modules, thre));
	return res;
}
```
