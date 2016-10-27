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