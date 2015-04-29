#ifndef __3danaglyph_h_
#define __3danaglyph_h_
#include "3DAnaglyph.h"
#include "cv.h"
#include <windows.h>
#include <Vfw.h>
#include "avi_utils.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "cs.h"
#ifdef __cplusplus
}
#endif
#include "cxcore.h"
#include "highgui.h"


struct  awpColor
{
	unsigned char bRed;
	unsigned char bGreen;
	unsigned char bBlue;
};
/*
//	вычисление значений градиента
*/
void MakeMaskGradient(IplImage* src1, IplImage* mask, IplImage* gr);
/*
//	оценка относительного расстояния
*/
void GetSigmaFromGradients(IplImage* n1, IplImage* n2, IplImage* s);
/*
	нормировка
*/
void NormSigma(IplImage* s, IplImage* d);
/*
//	Совмещенный билатеральный фильтр
*/
void JointBilateralFilter(IplImage* depth1, IplImage* src1, IplImage* result);
/*
//	Гауссовский фильтр
*/
void GaussFilter(IplImage* src1, IplImage* dst1);
/*
//функция преобразования исходного изображения в разреженную карту глубины 
*/
void MakeSparceDepthMap(IplImage* src, IplImage* depth_map);
/*
// выполняет интерполяцию изображения 
*/
int Interpolate1(IplImage* pSrc, IplImage* pConstMap, IplImage* pConstVals, IplImage* pResult);
/*
// получение полной карты глубины	
*/
int MakeFullDepthMap(IplImage* src, IplImage* sparce_depth, IplImage* full_depth);
/*
// выполняет смещение изображения в соответствии с картой глубины.
// максимальное горизонтальное смещение - 5 пикс (соответствует глубине 0)
// максимальное вертикальное смещение - 0 пикс.
// неизвестные пиксели заполняются граничными значениями
*/
void Displace(IplImage* src, IplImage* map, IplImage* dst, int max_shift);

void Displace1(IplImage* src, IplImage* map, IplImage* dst, int max_shift);
/*
// получение 3D анаглифа из двух изображений
*/
void MakeAnaglyph(IplImage* src1, IplImage* src2, IplImage* dst, int options);
/*
// получение мозаичного изображения 
*/
void MakeTile(IplImage* src1, IplImage* src2, IplImage* dst);
/*
// проверка расширения для имени файла
*/
bool CheckFileNameExt(const char* lpExt, const char* lpFileName);
/*
// проверка допустимости имени файла 
*/
bool AcceptFileName(const char* lpFileName, int options, bool isSrc);

/*
// получение jps изображения
*/
int _Make3DTile(IplImage* pSrc, IplImage* pDst);

#endif//__3danaglyph_h_