#include"header.h"

//ͨ�����벢��ȡrͨ��
Mat separateChannel(Mat const src)
{
	Mat dst;
	vector<Mat> Vchannels;
	Mat channel_b, channel_g, channel_r;
	split(src, Vchannels);
	channel_b = Vchannels.at(0);
	channel_g = Vchannels.at(1);
	channel_r = Vchannels.at(2);
	channel_r.copyTo(dst);				//��ɫͨ���Աȶ����
	return dst;
}