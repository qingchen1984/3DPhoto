#include "_3DAnaglyph.h"
/*
	���������� ������ ����� �������. 
	�������� ����������� ��������: �������� ����������� src, 
	����������� ����� ������� sparce_depth
	�������� �������� - full_depth, ������� ����� �������.
	������� �����������:
	src - 3 ������ IPL_DEPTH_8U
	sparce_depth - 1 ����� IPL_DEPTH_8U
	full_depth - 1 ����� IPL_DEPTH_8U
	� �������� ������ ������� ����������� ����������� � ������ IPL_DEPTH_64F

	������ �������
	1. ������������ ������ ��� ������� Interpolate1
	3. ������������� ��������� ������  ������� Interpolate1 � full_depth
*/
int MakeFullDepthMap(IplImage* src, IplImage* sparce_depth, IplImage* full_depth)
{
	int res = 0;
	CvSize s; s.width = src->width; s.height = src->height;
	// ��������� �� �����������
	IplImage* G = NULL;
	IplImage* G1 = NULL;
	IplImage* I = NULL;
	IplImage* consts_map = NULL;
	IplImage* consts_vals = NULL;
	IplImage* result = NULL;
	//��������� �� ������ 
	double* d1 = NULL;
	double* d2 = NULL;
	double* d3 = NULL;
	unsigned char* b1 = NULL;
	unsigned char* b2 = NULL;
	// ��������
	int i, j;
	// ������������� ������ ��� �����������
	G = cvCreateImage(s, IPL_DEPTH_8U, 1);
	I = cvCreateImage(s,IPL_DEPTH_64F, 1);
	consts_map = cvCreateImage(s, IPL_DEPTH_64F,1);
	consts_vals = cvCreateImage(s, IPL_DEPTH_64F, 1);
	result = cvCreateImage(s, IPL_DEPTH_64F,1);
	// ��������� ����������� G
	cvCvtColor(src, G, CV_BGR2GRAY);
	if (sparce_depth->nChannels > 1)
	{
		G1 = cvCreateImage(s, IPL_DEPTH_8U, 1);
		cvCvtColor(sparce_depth, G1, CV_BGR2GRAY);
	}
	else
		G1 = sparce_depth;
	// �������������� G->I. I = G/255.
	// �������������� sparce_depth -> �onst_vals, �onst_map
	// const_map = sparce_depth != 0?1:0;
	// const_vals = sparce_depth / 255
	for (j = 0; j < src->height; j++)
	{
		// ��������� �� ������ �����������
		d1 = (double*)(I->imageData) + j*I->width;
		b1 = (unsigned char*)(G->imageData + j*G->widthStep);
		b2 = (unsigned char*)(G1->imageData + j*G1->widthStep);
		d2 = (double*)(consts_map->imageData) + j*consts_map->width;
		d3 = (double*)(consts_vals->imageData) + j*consts_vals->width;
		for(i = 0; i < src->width; i++)
		{
			d1[i] = b1[i] / 255.;
			d2[i] = b2[i] > 10? 1:0;
			d3[i] = b2[i] / 255.;
		}
	}
	cvReleaseImage(&G);

	// ���������� ������������
	res = Interpolate1(I, consts_map, consts_vals, result);
	if (res == 0)
	{
		// �������������� result -> full_depth
		//full_depth = 255*(1-result);
		double max_val, min_val;
		cvMinMaxLoc(result, &min_val, &max_val);
		for (j = 0; j < src->height; j++)
		{
			// ��������� �� ������ �����������
			d1 = (double*)(result->imageData) + j*result->width;
			b1 = (unsigned char*)(full_depth->imageData + j*full_depth->widthStep);
			for(i = 0; i < src->width; i++)
			{
				b1[i] = (unsigned char)(255*(max_val - d1[i])/(max_val - min_val));		
			}
		}
	}
	// ������������ ������
	cvReleaseImage(&I);
	cvReleaseImage(&consts_map);
	cvReleaseImage(&consts_vals);
	cvReleaseImage(&result);
	if (sparce_depth->nChannels > 1)
		cvReleaseImage(&G1);
	return res;
}
