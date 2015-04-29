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
//	���������� �������� ���������
*/
void MakeMaskGradient(IplImage* src1, IplImage* mask, IplImage* gr);
/*
//	������ �������������� ����������
*/
void GetSigmaFromGradients(IplImage* n1, IplImage* n2, IplImage* s);
/*
	����������
*/
void NormSigma(IplImage* s, IplImage* d);
/*
//	����������� ������������� ������
*/
void JointBilateralFilter(IplImage* depth1, IplImage* src1, IplImage* result);
/*
//	����������� ������
*/
void GaussFilter(IplImage* src1, IplImage* dst1);
/*
//������� �������������� ��������� ����������� � ����������� ����� ������� 
*/
void MakeSparceDepthMap(IplImage* src, IplImage* depth_map);
/*
// ��������� ������������ ����������� 
*/
int Interpolate1(IplImage* pSrc, IplImage* pConstMap, IplImage* pConstVals, IplImage* pResult);
/*
// ��������� ������ ����� �������	
*/
int MakeFullDepthMap(IplImage* src, IplImage* sparce_depth, IplImage* full_depth);
/*
// ��������� �������� ����������� � ������������ � ������ �������.
// ������������ �������������� �������� - 5 ���� (������������� ������� 0)
// ������������ ������������ �������� - 0 ����.
// ����������� ������� ����������� ���������� ����������
*/
void Displace(IplImage* src, IplImage* map, IplImage* dst, int max_shift);

void Displace1(IplImage* src, IplImage* map, IplImage* dst, int max_shift);
/*
// ��������� 3D �������� �� ���� �����������
*/
void MakeAnaglyph(IplImage* src1, IplImage* src2, IplImage* dst, int options);
/*
// ��������� ���������� ����������� 
*/
void MakeTile(IplImage* src1, IplImage* src2, IplImage* dst);
/*
// �������� ���������� ��� ����� �����
*/
bool CheckFileNameExt(const char* lpExt, const char* lpFileName);
/*
// �������� ������������ ����� ����� 
*/
bool AcceptFileName(const char* lpFileName, int options, bool isSrc);

/*
// ��������� jps �����������
*/
int _Make3DTile(IplImage* pSrc, IplImage* pDst);

#endif//__3danaglyph_h_