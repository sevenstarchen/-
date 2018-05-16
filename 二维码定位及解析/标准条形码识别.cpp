#include "zbar.h"      
#include "cv.h"      
#include "highgui.h"      
#include <iostream>      

using namespace std;
using namespace zbar;  //���zbar���ƿռ�    
using namespace cv;

int main(int argc, char*argv[])
{
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	Mat image = imread("C:\\Users\\Lucky-Chi\\Desktop\\��ý���ά��\\txm.jpg");
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
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
	imshow("Source Image", image);
	waitKey();
	imageZbar.set_data(NULL, 0);
	return 0;
}
