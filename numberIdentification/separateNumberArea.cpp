#include"header.h"

//分离数字串中的数字，返回Mat的vector
vector<Mat> separateNumberArea(Mat part, vector<Rect> number)
{
	vector<Mat> numberArea(number.size());
	for (int i = 0; i < number.size(); i++)
		part(Rect(number[i])).copyTo(numberArea[i]);
	return numberArea;
}