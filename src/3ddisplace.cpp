#include "_3DAnaglyph.h"

void Displace1(IplImage* src, IplImage* map, IplImage* dst, int max_shift)
{
	// переменные
	awpColor* css = NULL; // исходное изображение
    awpColor* cdd = NULL; // полученное изображение
    unsigned char*	  d  = NULL; // карта глубины для каждого пикселя
	float 	  shift = 0; // текущее смещение пикселя
    int       ishift = 0;
    int       delta = 0;
    int x, y, pos,i; 			 // счетчики
    int xx, yy; // обновленные координаты пикселей, в соответствии с картой глубины
    if (abs(max_shift) > 100) // смещение не может превышать 5 пикс.
  	   return;
	cvZero(dst);
    delta = max_shift > 0 ? 1:-1;
    css = (awpColor*)src->imageData;
    cdd = (awpColor*)dst->imageData;
    d = (unsigned char*)map->imageData;
	for (y = 0; y < src->height; y++)
    {
        yy = y;
		cdd = (awpColor*)((unsigned char*)dst->imageData + y*dst->widthStep);
		css = (awpColor*)((unsigned char*)src->imageData + y*src->widthStep);
		if (delta == -1)
		{
			for (x = 0; x < dst->width; x++)
				{
					pos = x;
        			shift = max_shift*(d[pos])/255.f;
					ishift = (int)shift;
					xx = x + ishift;
					if (xx < 0 || xx >= dst->width)
            			continue;
					cdd[xx] = css[pos];
					if (abs(xx - x) >= 1)
					{
            			for (i = 0; i <= abs(xx-x); i++)
						{
							if (xx - delta*i <= 0 || xx  - delta*i >= dst->width)
                    			continue;
							cdd[xx -delta*i] = css[pos];
						}
					}
				}
		}
		else
		{
			for (x = src->width ; x >0; x--)
				{
					pos = x;
        			shift = max_shift*(d[pos])/255.f;
					ishift = (int)shift;
					xx = x + ishift;
					if (xx < 0 || xx >= dst->width)
            			continue;
					cdd[xx] = css[pos];
					if (abs(xx - x) >= 1)
					{
            			for (i = 0; i <= abs(xx-x); i++)
						{
							if (xx - delta*i <= 0 || xx  + delta*i >= dst->width)
                    			continue;
							cdd[xx - delta*i] = css[pos];
						}
					}
				}
		}
    }
}

const float c_e  = 200;
const float c_d  = 500;
const float c_md = 2;
const float c_base = 128;
void Displace(IplImage* src, IplImage* map, IplImage* dst, int max_shift)
{
	awpColor* css = NULL; // исходное изображение
    awpColor* cdd = NULL; // полученное изображение
    unsigned char*	  d  = NULL; // карта глубины для каждого пикселя
	//float c_e = fabs((float)max_shift);
	int dx = 0;
	int x0 = 0;
	int x1 = 0;
	int xp = 0;
    int xn = 0;
    d = (unsigned char*)map->imageData;
	if (max_shift >= 0)
	{
		dx = 1;
		x0 = 0;
		x1 = src->width;
	}
	else
	{
		dx = -1;
		x0 = src->width;
		x1 = 0;
	}
	unsigned char* c1;
	for (int i = 0; i < src->height; i++)
	{
		xp = x0;
		int x = x0;
		c1 = ((unsigned char*)src->imageData) +i*src->widthStep;
		css = (awpColor*)c1;
		c1 = ((unsigned char*)dst->imageData) + i*dst->widthStep;
		cdd =(awpColor*)c1;
		c1 = ((unsigned char*)map->imageData) + i*map->widthStep;
		d = c1;
		for (int j = 0; j < src->width; j++)
		{
			float z = (c_base - (float)d[x]/c_md);
			xn = (int)floor(x + dx*0.5*c_e*z/(z + c_d + 1));
			x += dx;
			if (xn < 0 || xn > src->width)
				continue;
			awpColor cp = css[(int)x];
			cdd[(int)xn] = cp;	
			if (abs(xn - xp) > 1)
			{
				int p = xp+dx;
				while (p >= xn - dx)
				{
					if (p < 0 || p > src->width)
						break;
					cdd[p] = cp;
    				p += dx;
				}
			}
			xp = xn;
		}
	}
}
