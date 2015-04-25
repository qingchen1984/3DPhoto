#include "_3DAnaglyph.h"
/*
	вычисление значений sigma из двух изображений градиентов
*/
void GetSigmaFromGradients(IplImage* n1, IplImage* n2, IplImage* s)
{
	float* fn1 = (float*)n1->imageData;
	float* fn2 = (float*)n2->imageData;
	float* fs  = (float*)s->imageData;
	int x, y;
	cvZero(s);
	for (y = 0; y < s->height; y++)
	{
		for (x = 0; x < s->width; x++)
		{
			if (fn2[y*s->width + x] > 0)
			{
				float r = fn1[y*s->width + x]/fn2[y*s->width + x];
				r *=r; 
				if(r > 1.1 && r <= 4)
			     	fs[y*s->width + x] = 1/sqrt(r-1);
				else
					fs[y*s->width + x] = 0; //выключаем точку из рассмотрения. 
			}
			else
				fs[y*s->width + x] = 0;
		}
	}
}
/*нормировка sigma*/
void NormSigma(IplImage* s, IplImage* d)
{
	double dmin, dmax;
	dmin= 0;
	dmax =0;
	cvMinMaxLoc(s,&dmin,&dmax);
	cvZero(d);
	float* fs = (float*)s->imageData;
	unsigned char* b = (unsigned char*)d->imageData;
	int x,y;
	for (y =0; y < d->height; y++)
	{
		for(x = 0; x < d->width; x++)
		{
				b[y*d->widthStep + x] = (unsigned char)(255*(fs[y*s->width+x] - dmin)/(dmax-dmin));
		}
	}
}
