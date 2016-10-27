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