#include "zbar.h"      
#include "cv.h"      
#include "highgui.h"     
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>      

using namespace std;
using namespace zbar;  //���zbar���ƿռ�    
using namespace cv;
Mat src; Mat src_gray; Mat imge;
IplImage* pSrc;
IplImage* dst;

RNG rng(12345);
//Scalar colorful = CV_RGB(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));


Point Center_cal(vector<vector<Point> > contours, int i)//�ҵ�����ȡ���������ĵ�
{
	int centerx = 0, centery = 0, n = contours[i].size();
	//����ȡ��С�����εı߽���ÿ���ܳ���������ȡһ��������꣬������ȡ�ĸ����ƽ�����꣨��ΪС�����εĴ������ģ�
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	Point point1 = Point(centerx, centery);
	return point1;
}
int main(int argc, char*argv[])
{
	src = imread("C:\\Users\\Lucky-Chi\\Desktop\\��ý���ά��\\test5.jpg", 1);
	Mat src_all = src.clone();

	cvtColor(src, src_gray, CV_BGR2GRAY);
	//  src_gray = Scalar::all(255) - src_gray;
	blur(src_gray, src_gray, Size(3, 3));
	equalizeHist(src_gray, src_gray);
	imshow("�˲���", src_gray);

	Scalar color = Scalar(1, 1, 255);
	Mat threshold_output;
	vector<vector<Point> > contours, contours2;
	vector<Vec4i> hierarchy;
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	Mat drawing2 = Mat::zeros(src.size(), CV_8UC3);
	threshold(src_gray, threshold_output, 112, 255, THRESH_BINARY);
	//Canny(src_gray,threshold_output,136,196,3);
	//imshow("Ԥ�����",threshold_output);
	//Ѱ������ 
	//��һ������������ͼ�� 2ֵ����
	//�ڶ����������ڴ�洢����FindContours�ҵ��������ŵ��ڴ����档
	//�����������ǲ㼶��**[Next, Previous, First_Child, Parent]** ��vector
	//���ĸ����������ͣ��������ṹ
	//����������ǽڵ����ģʽ��������ȫ��Ѱ��
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));
	//CHAIN_APPROX_NONEȫ��,CV_CHAIN_APPROX_SIMPLE,,,RETR_TREE    RETR_EXTERNAL    RETR_LIST   RETR_CCOMP

	int c = 0, ic = 0, k = 0, area = 0;

	//����ĺ���ɸѡ
	int parentIdx = -1;
	for (int i = 0; i< contours.size(); i++)
	{
		if (hierarchy[i][2] != -1 && ic == 0)
		{
			parentIdx = i;
			ic++;
		}
		else if (hierarchy[i][2] != -1)
		{
			ic++;
		}
		else if (hierarchy[i][2] == -1)
		{
			ic = 0;
			parentIdx = -1;
		}

		if (ic >= 2)
		{
			contours2.push_back(contours[parentIdx]);
			drawContours(drawing, contours, parentIdx, CV_RGB(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1, 8);
			ic = 0;
			parentIdx = -1;
			area = contourArea(contours[i]);//�ó�һ����ά�붨λ�ǵ�������Ա������߳���area_side�������ݸ�������ν��������λ��������һ�����ݶ����ԣ�
		}
		//cout<<"i= "<<i<<" hierarchy[i][2]= "<<hierarchy[i][2]<<" parentIdx= "<<parentIdx<<" ic= "<<ic<<endl;
	}


	for (int i = 0; i<contours2.size(); i++)
		drawContours(drawing2, contours2, i, CV_RGB(rng.uniform(100, 255), rng.uniform(100, 255), rng.uniform(100, 255)), -1, 4, hierarchy[k][2], 0, Point());


	Point point[3];
	for (int i = 0; i<contours2.size(); i++)
	{
		point[i] = Center_cal(contours2, i);
	}
	area = contourArea(contours2[1]);//Ϊʲô��һ���ǰ��һ�����������һ����
	int area_side = cvRound(sqrt(double(area)));
	for (int i = 0; i<contours2.size(); i++)
	{
		line(drawing2, point[i%contours2.size()], point[(i + 1) % contours2.size()], color, area_side / 4, 8);
	}


	imshow("��ȡ��", drawing2);
	printf("%d\n", contours.size());
	//imshow( "Contours", drawing );

	//������Ҫ�����������ά��
	Mat gray_all, threshold_output_all;
	vector<vector<Point> > contours_all;
	vector<Vec4i> hierarchy_all;
	cvtColor(drawing2, gray_all, CV_BGR2GRAY);


	threshold(gray_all, threshold_output_all, 45, 255, THRESH_BINARY);

	//��ʾֻѰ�����������
	findContours(threshold_output_all, contours_all, hierarchy_all, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));//RETR_EXTERNAL��ʾֻѰ�����������

	//����С��Χ���Σ�б��Ҳ����Ŷ
	RotatedRect rectPoint = minAreaRect(contours_all[0]);
	Point2f fourPoint2f[4];

	//��rectPoint�����д洢������ֵ�ŵ� fourPoint��������
	rectPoint.points(fourPoint2f);
	//Point pt = fourPoint2f[0];
	//cout << pt.x;
	float a = fourPoint2f[0].y;
	float z = fourPoint2f[1].y;
	float z2 = fourPoint2f[1].x;
	float b = fourPoint2f[1].y;
	float a2 = fourPoint2f[0].x;
	float c2 = fourPoint2f[2].x;
	float widths = a - b;
	float lengths = c2 - a2;
	for (int i = 0; i < 4; i++)
	{
		line(src_all, fourPoint2f[i % 4], fourPoint2f[(i + 1) % 4], Scalar(20, 21, 237), 3);
		cout << fourPoint2f[i];
	}

	imshow("��ά��", src_all);
	
	Mat img = imread("C:\\Users\\Lucky-Chi\\Desktop\\��ý���ά��\\test5.jpg");
	Rect rect(z2, z, lengths,widths );
	Mat ROI = img(rect);
	imshow("��ȡ��", ROI);
	imwrite("C:\\Users\\Lucky-Chi\\Desktop\\��ý���ά��\\test00.jpg", ROI);
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	Mat image2 = imread("C:\\Users\\Lucky-Chi\\Desktop\\��ý���ά��\\test00.jpg");
	Mat imageGray;
	cvtColor(image2, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //ɨ������    
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "��ѯ����ʧ�ܣ�����ͼƬ��" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "���ͣ�" << endl << symbol->get_type_name() << endl << endl;
		cout << "���룺" << endl << symbol->get_data() << endl << endl;
	}
	waitKey();
	imageZbar.set_data(NULL, 0);
	return 0;
}
