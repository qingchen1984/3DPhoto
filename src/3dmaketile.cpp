#include "_3DAnaglyph.h"
/*
// получение мозаичного изображения 
*/
void MakeTile(IplImage* src1, IplImage* src2, IplImage* dst)
{
	int i,j;
	unsigned char* s1 = (unsigned char*) src1->imageData;
	unsigned char* s2 = (unsigned char*) src2->imageData;
	unsigned char* d = (unsigned char*) dst->imageData;
	for (i = 0; i < dst->height; i++)
		for (j = 0; j < dst->widthStep; j++)
		{
			if (j < src1->widthStep)
			{
				d[i*dst->widthStep + j] =  s1[i*src1->widthStep + j];
			}
			else
			{
				d[i*dst->widthStep + j] =  s2[i*src2->widthStep + j-src2->widthStep];
			}
		}
}
