#include"header.h"

Mat separateChannel(Mat const src);
vector<Rect> calcExternalRect(Mat const mask_contours);
vector<Rect> calcCenterAndStd(vector<Rect> dstRect);

#if findNumberAreaWay == 0
//Ѱ�����ִ��е������ֵ�����Χ�����ؾ�������
vector<Rect> findNumberArea(Mat const src, int thresholdVal, Size kernelSize1, Size kernelSize2)
{
	//ͨ������
	Mat dst = separateChannel(src);
	//�ֶ���ֵ��
	threshold(dst, dst, thresholdVal, 255, CV_THRESH_BINARY);

	//Mat temp_mask;
	//boxFilter(dst, temp_mask, -1, Size(5, 5));
	//threshold(temp_mask, temp_mask, 30, 255, CV_THRESH_BINARY);

	//��ʴ���Ͳ���
	Mat kernel = getStructuringElement(MORPH_RECT, kernelSize1, Point(-1, -1));	//������κ�
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);
	kernel = getStructuringElement(MORPH_RECT, kernelSize2, Point(-1, -1));	//������κ�
	dilate(dst, dst, kernel, Point(-1, -1), 1, 0);

	//imshow("show", dst);

	//������Ӿ���
	Mat mask_contours;
	dst.copyTo(mask_contours);
	vector<Rect> dstRect = calcExternalRect(mask_contours);

	//���������
	std::sort(dstRect.begin(), dstRect.end(), 
		[](Rect rec1, Rect rec2) {return rec1.x < rec2.x; }
	);

	//����������Ĳ���ȱ�׼��
	dstRect = calcCenterAndStd(dstRect);

	return dstRect;
}
#endif

#if findNumberAreaWay == 1
//Ѱ�����ִ��е������ֵ�����Χ�����ؾ�������
vector<Rect> findNumberArea(Mat const src, int thresholdVal, Size kernelSize1, Size kernelSize2)
{
	//ͨ������
	Mat dst = separateChannel(src);

	//�ֲ�����Ӧ��ֵ
	adaptiveThreshold(dst, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 51, thresholdVal);

	//Mat temp_mask;
	//boxFilter(dst, temp_mask, -1, Size(5, 5));
	//threshold(temp_mask, temp_mask, 30, 255, CV_THRESH_BINARY);

	//��ʴ���Ͳ���
	Mat kernel = getStructuringElement(MORPH_RECT, kernelSize1, Point(-1, -1));	//������κ�
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);
	kernel = getStructuringElement(MORPH_RECT, kernelSize2, Point(-1, -1));	//������κ�
	dilate(dst, dst, kernel, Point(-1, -1), 1, 0);
	kernel = getStructuringElement(MORPH_RECT, Size(8,1), Point(-1, -1));
	erode(dst, dst, kernel, Point(-1, -1), 1, 0);

	//imshow("show", dst);

	//������Ӿ���
	Mat mask_contours;
	dst.copyTo(mask_contours);
	vector<Rect> dstRect = calcExternalRect(mask_contours);

	//���������
	std::sort(dstRect.begin(), dstRect.end(),
		[](Rect rec1, Rect rec2) {return rec1.x < rec2.x; }
	);

	//����������Ĳ���ȱ�׼��
	dstRect = calcCenterAndStd(dstRect);

	return dstRect;
}
#endif
