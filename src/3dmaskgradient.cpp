#include "_3DAnaglyph.h"
float gh[9] = {
			   1,0,-1,
               2,0,-2,
			   1,0,-1
			   };
float gv[9] = {
				-1,-2,-1,
				 0,0,0,
				 1,2,1
};

void MakeMaskGradient(IplImage* src1, IplImage* mask, IplImage* gr)
{
	unsigned char* b = (unsigned char*)src1->imageData;
	unsigned char* m = (unsigned char*)mask->imageData;
	float* grad = (float*)gr->imageData;
	int x,y, xx, yy;
	cvZero(gr);
	for (y = 1; y < src1->height-1; y++)
		for (x = 1; x < src1->width-1; x++)
		{
			if (m[y*src1->widthStep + x] > 0)
			{
				float nx = 0; 
				float ny = 0;
				int k  = 0;
				for (yy = y - 1; yy <= y + 1; yy++)
				{
					for (xx = x - 1; xx <= x+1; xx++,k++)
					{
						nx += b[yy*src1->widthStep + xx]*gh[k];				
						ny += b[yy*src1->widthStep + xx]*gv[k];				
					}
				}
				grad[y*src1->width + x] = sqrt(nx*nx+ny*ny);
			}
			else
				grad[y*src1->width + x] = 0;
		}
}
