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