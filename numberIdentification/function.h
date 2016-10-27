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