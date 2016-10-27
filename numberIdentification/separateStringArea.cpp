#include"header.h"

//分离数字区域，返回区域的图片
Mat separateStringArea(Mat const src, Rect rectArea)
{
	Mat area;
	src(Rect(rectArea)).copyTo(area);
	return area;
}