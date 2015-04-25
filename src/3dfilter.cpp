#include "_3DAnaglyph.h"

void JointBilateralFilter(IplImage* depth1, IplImage* src1, IplImage* result)
{
	const float sigma1 = 1.f;
	const float sigma2 = 0.1f;
	const int half_aperture =1;
	float* d = (float*)depth1->imageData;
	unsigned char* s = (unsigned char*)src1->imageData;
	float* r = (float*)result->imageData;
	int x,y,xx,yy;

	cvZero(result);
	for (y = half_aperture; y < depth1->height - half_aperture; y++)
	{
		for (x = half_aperture; x < depth1->width - half_aperture; x++)
		{
			if (d[y*depth1->width + x] > 0)
			{
				float edst = 0; //евклидово рассто€ние между пиксел€ми
				float cdst = 0; //рассто€ние в цветовом пространстве между пиксел€ми 

				float c0 = 0;	//сумма евклидовых рассто€ний
				float s0 = 0;   //сумма рассто€ний в пространстве интенсивностей
				float t0 = 0;	//сумма с учетом пикселей изображени€ 
				float n0 = 0;   //нормировочна€ сумма
				for (yy = y - half_aperture; yy <= y+half_aperture; yy++)
				{
					for (xx = x - half_aperture; xx <= x + half_aperture; xx++)
					{
						edst = sqrt((float)(x-xx)*(x-xx) + (float)(y-yy)*(y-yy));
						c0 += exp(-0.5f*(edst/sigma1)*(edst/sigma1)); 
						cdst = fabs((float)s[y*depth1->widthStep + x] - (float)s[yy*depth1->widthStep + xx]);
						s0 += exp(-0.5f*(cdst/sigma2)*(cdst/sigma2));
						n0+= c0*s0;
						t0 += c0*s0*d[yy*depth1->width + xx];
					}
				}
				r[y*depth1->width + x] = t0/n0;
			}
		}
	}
}
float gf[25] = {
				 1,4,7,4,1,
				 4,16,26,16,4,
				 7,26,41,26,7,
				 4,16,26,16,4,
				 1,4,7,4,1
};
const float gnorm = 273;

void GaussFilter(IplImage* src1, IplImage* dst1)
{
	unsigned char* s = (unsigned char*)src1->imageData;
	unsigned char* d = (unsigned char*)dst1->imageData;
	int x,y,xx,yy;
	cvZero(dst1);
	for (y = 2; y < src1->height - 2; y++)
	{
		for (x = 2; x < src1->width - 2; x++)
		{
			float sum = 0;
			int k = 0;
			for (yy = y - 2; yy <= y+2; yy++)
			{
				for (xx = x - 2; xx <= x+2; xx++, k++)
				{
					sum += gf[k]*s[yy*src1->widthStep + xx];
				}
			}
			sum /= gnorm;
			d[y*src1->widthStep + x] = (unsigned char)(sum);
		}
	}

}