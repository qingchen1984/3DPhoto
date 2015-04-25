// test04.cpp: тестирование функции MakeSparceDepthMap
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	IplImage* src = NULL;
	IplImage* dst = NULL;
	
	src = cvLoadImage(argv[1]);
	if (src == NULL)
		return -1;
	CvSize s; s.height = src->height; s.width = src->width;
	dst = cvCreateImage(s, IPL_DEPTH_8U, 1);
	if (dst == NULL)
		return -1;

	MakeSparceDepthMap(src, dst);

	cvSaveImage("sparse.png", dst);

	cvReleaseImage(&src);
	cvReleaseImage(&src);
	return 0;
}
