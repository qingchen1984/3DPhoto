// test01.cpp: тестироване функции MakeFullDepthMap
// программа test01 выполн€ет интерпол€цию разреженной 
// карты глубины и строит плотную карту глубины. 

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	IplImage* src = NULL;
	IplImage* dm = NULL;
	IplImage* dst = NULL;
	
	src = cvLoadImage(argv[1]);
	if (src == NULL)
		return -1;
	dm = cvLoadImage(argv[2]);
	if (dm == NULL)
		return -1;
	CvSize s; s.height =src->height; s.width = src->width;
	dst = cvCreateImage(s, IPL_DEPTH_8U, 1);
	if (dst == NULL)
		return -1;
	printf("processing...");
	DWORD ts  = GetTickCount();
	if (MakeFullDepthMap(src, dm, dst) != 0)
		printf("error.");
	else
		printf("done.");
	DWORD t = GetTickCount() - ts;
	printf("  %ld ms\n", t);
	cvSaveImage("out.png", dst);
	cvReleaseImage(&src);
	cvReleaseImage(&dm);
	cvReleaseImage(&dst);
	return 0;
}

