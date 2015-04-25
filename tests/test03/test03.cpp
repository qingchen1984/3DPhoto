// test03.cpp: тестирование функций Displace и MakeAnaglyph
// построение анаглифного изображения из исходного изображения и карты глубины. 

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	IplImage* src = NULL;
	IplImage* dmap = NULL;
	IplImage* rep = NULL;
	IplImage* lep = NULL;
	IplImage* tmp = NULL;
	IplImage* dst = NULL;
	src  = cvLoadImage(argv[1]);
	if (src == NULL)
		return -1;
	dmap = cvLoadImage(argv[2]);
	if (dmap == NULL)
		return -1;

	CvSize s; s.height = src->height; s.width = src->width;

	if(dmap->nChannels > 1)
	{
		tmp = cvCreateImage(s, IPL_DEPTH_8U, 1);
		if (tmp == NULL)
			return -1;
		cvCvtColor(dmap,tmp,CV_BGR2GRAY);
		cvReleaseImage(&dmap);
		dmap = tmp;
	}
	lep = cvCreateImage(s, IPL_DEPTH_8U, 3);
	if (lep == NULL)
		return -1;
	rep = cvCreateImage(s, IPL_DEPTH_8U, 3);
	if (rep == NULL)
		return -1;
	dst = cvCreateImage(s, IPL_DEPTH_8U, 3);
	if (dst == NULL)
		return -1;	
	Displace(src, dmap, lep, -10);
	Displace(src, dmap, rep, 10);
	MakeAnaglyph(lep, rep, dst, ColorAnaglyph);
	cvSaveImage("anaglyph.png", dst);

	cvReleaseImage(&lep);
	cvReleaseImage(&rep);
	cvReleaseImage(&dmap);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	return 0;
}
