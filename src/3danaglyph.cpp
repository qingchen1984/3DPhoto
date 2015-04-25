#include "_3DAnaglyph.h"
 //true anaglyph
const float c_fA11 = 0.299f;
const float c_fA21 = 0.587f;
const float c_fA31 = 0.114f;
static void _MakeTrueAnaglyph(IplImage* src1, IplImage* src2, IplImage* dst)
{
	awpColor* s1 = NULL;
	awpColor* s2 = NULL;
	awpColor* d = NULL;
    int i,j;
	for (j = 0; j < src1->height; j++)
	{
		s1 = (awpColor*)(src1->imageData + j*src1->widthStep);
		s2 = (awpColor*)(src2->imageData + j*src2->widthStep);
		d = (awpColor*)(dst->imageData + j*dst->widthStep);
		for (i = 0; i < src1->width; i++)
		{
			d[i].bBlue = (unsigned char)(c_fA31*s1[i].bRed + c_fA21*s1[i].bGreen + c_fA11*s1[i].bBlue);
			d[i].bGreen = 0;
			d[i].bRed = (unsigned char)(c_fA31*s2[i].bRed + c_fA21*s2[i].bGreen + c_fA11*s2[i].bBlue);
		}
	}
}
static void _MakeGrayAnaglyph(IplImage* src1, IplImage* src2, IplImage* dst)
{
	awpColor* s1 = NULL;
	awpColor* s2 = NULL;
	awpColor* d = NULL;
    int i,j;
	for (j = 0; j < src1->height; j++)
	{
		s1 = (awpColor*)(src1->imageData + j*src1->widthStep);
		s2 = (awpColor*)(src2->imageData + j*src2->widthStep);
		d = (awpColor*)(dst->imageData + j*dst->widthStep);
		for (i = 0; i < src1->width; i++)
		{
			d[i].bBlue = (unsigned char)(c_fA31*s1[i].bRed + c_fA21*s1[i].bGreen + c_fA11*s1[i].bBlue);
			d[i].bGreen = (unsigned char)(c_fA31*s2[i].bRed + c_fA21*s2[i].bGreen + c_fA11*s2[i].bBlue);
			d[i].bRed = (unsigned char)(c_fA31*s2[i].bRed + c_fA21*s2[i].bGreen + c_fA11*s2[i].bBlue);
		}
	}
}
static void _MakeColorAnaglyph(IplImage* src1, IplImage* src2, IplImage* dst)
{
	awpColor* s1 = NULL;
	awpColor* s2 = NULL;
	awpColor* d = NULL;
    int i,j;
	for (j = 0; j < src1->height; j++)
	{
		s1 = (awpColor*)(src1->imageData + j*src1->widthStep);
		s2 = (awpColor*)(src2->imageData + j*src2->widthStep);
		d = (awpColor*)(dst->imageData + j*dst->widthStep);
		for (i = 0; i < src1->width; i++)
		{
			d[i].bBlue = s1[i].bBlue;
			d[i].bGreen = s2[i].bGreen;
			d[i].bRed = s2[i].bRed;
		}
	}
}
static void _MakeHalfColorAnaglyph(IplImage* src1, IplImage* src2, IplImage* dst)
{
	awpColor* s1 = NULL;
	awpColor* s2 = NULL;
	awpColor* d = NULL;
    int i,j;
	for (j = 0; j < src1->height; j++)
	{
		s1 = (awpColor*)(src1->imageData + j*src1->widthStep);
		s2 = (awpColor*)(src2->imageData + j*src2->widthStep);
		d = (awpColor*)(dst->imageData + j*dst->widthStep);
		for (i = 0; i < src1->width; i++)
		{
		d[i].bBlue = (unsigned char)(c_fA31*s1[i].bRed + c_fA21*s1[i].bGreen + c_fA11*s1[i].bBlue);
		d[i].bGreen = s2[i].bGreen;
		d[i].bRed = s2[i].bRed;
		}
	}
}
static void _MakeOptimizedAnaglyph(IplImage* src1, IplImage* src2, IplImage* dst)
{
	awpColor* s1 = NULL;
	awpColor* s2 = NULL;
	awpColor* d = NULL;
    int i,j;
	for (j = 0; j < src1->height; j++)
	{
		s1 = (awpColor*)(src1->imageData + j*src1->widthStep);
		s2 = (awpColor*)(src2->imageData + j*src2->widthStep);
		d = (awpColor*)(dst->imageData + j*dst->widthStep);
		for (i = 0; i < src1->width; i++)
		{
			d[i].bBlue = (unsigned char)(c_fA31*s1[i].bRed + c_fA21*s1[i].bGreen);
			d[i].bGreen = s2[i].bGreen;
			d[i].bRed = s2[i].bRed;
		}
	}
}
void MakeAnaglyph(IplImage* src1, IplImage* src2, IplImage* dst, int options)
{
	switch (options)
	{
	case TrueAnaglyph:
		_MakeTrueAnaglyph(src1,src2,dst);
		break;
	case GrayAnaglyph:
		_MakeGrayAnaglyph(src1,src2,dst);
		break;
	case ColorAnaglyph:
		_MakeColorAnaglyph(src1,src2,dst);
		break;
	case HalfColorAnaglyph:
		_MakeHalfColorAnaglyph(src1,src2,dst);
		break;
	case OptimizedAnaglyph:
		_MakeOptimizedAnaglyph(src1,src2,dst);
		break;
	}
}