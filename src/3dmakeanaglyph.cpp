#include "_3DAnaglyph.h"
const double cnMaxSize = 4096;
const double cnMaxSparse = 800*600;
const int cnMaxShift = 50;
const int cShift = 20;

#define _SAFE_RELEASE_(v) if (v) cvReleaseImage(&v);
/*
	��������� ����� ������� [pDst] �� ��������� ����������� [pSrc]. 
	������� ����������� ������ ���������. 
	���������� � ������������:
	�������� ����������� ������ ����� ������� IPL_DEPTH_8U, ���� 
	��� ��� �������� ������. 
	��������������� ����������� ������ ����� ������� IPL_DEPTH_8U
	���� ����� � ����� ���� �������, ��� � �������� �����������. 

	������������ ��������
	NOERROR - ���� ����� ������� ��������
	ERR_INVALID_PARAMS - ����������� �� ������������� �����������

*/
static int _MakeDepthMap(IplImage* pSrc, IplImage* pDst)
{
	int res = NOERROR;
	// �������� ������� ����������
	if (pSrc == NULL || pDst == NULL)
		return ERR_INVALID_PARAMS;
	if (pSrc->depth != IPL_DEPTH_8U || pSrc->depth != pDst->depth)
		return ERR_INVALID_PARAMS;
	if (pSrc->nChannels != 3 && pSrc->nChannels != 1)
		return ERR_INVALID_PARAMS;
	if (pDst->nChannels != 1)
		return ERR_INVALID_PARAMS;
	if (pSrc->width != pDst->width || pSrc->height != pDst->height)
		return ERR_INVALID_PARAMS;
	// ����. ��������������� � ������� ��������������� �����������
	double image_size = pSrc->width*pSrc->height;
	int n1 = (int) sqrt(image_size / cnMaxSparse);
	n1 = n1 < 1 ? 1:n1;
	int n2 = (int) sqrt(image_size / cnMaxSize);
	n2 = n2 < 1 ? 1: n2;
	CvSize s,s1,s2;
	s.height = pSrc->height;s.width = pSrc->width;
	s1.height = s.height / n1; s1.width = s.width / n1;
	s2.height = s.height / n2; s2.width = s.width / n2;
	// ��������������� � ��������� �����������
	IplImage* src1 = NULL; //����������� ����� ��������� ����������� ��� ��������� ����������� ����� �������
	IplImage* src2 = NULL; //����������� ����� ��������� ����������� ��� ��������� ������ ����� �������
	IplImage* sdepth_map = NULL;//����������� ����� �������
	IplImage* sdm = NULL;//����������� ����� ����������� ����� �������
	IplImage* fdepth_map = NULL;//������ ����� �������
	
	try
	{
		//������������� ������
		src1 = cvCreateImage(s1,pSrc->depth, pSrc->nChannels);
		if (src1 == NULL) throw ERR_MEMORY;
		src2 = cvCreateImage(s2,pSrc->depth, pSrc->nChannels); 
		if (src2 == NULL) throw ERR_MEMORY;
		sdepth_map = cvCreateImage(s1, IPL_DEPTH_8U, 1);
		if (sdepth_map == NULL) throw ERR_MEMORY;
		sdm = cvCreateImage(s2, IPL_DEPTH_8U, 1);
		if (sdm == NULL) throw ERR_MEMORY;
		fdepth_map = cvCreateImage(s2, IPL_DEPTH_8U, 1);
		if (fdepth_map == NULL) throw ERR_MEMORY;
		cvResize(pSrc, src1);
		cvResize(pSrc, src2);
		MakeSparceDepthMap(src1,sdepth_map);
		cvResize(sdepth_map, sdm);
		if (MakeFullDepthMap(src2, sdm, fdepth_map) != NOERROR)
			throw ERR_MAKE_DEPTH;
		cvResize(fdepth_map, pDst);
#ifdef _DEBUG
	 cvSaveImage("debug_full.png",pDst);
#endif 
	}
	catch(int& a)
	{
		res = a;
		_SAFE_RELEASE_(src1)
		_SAFE_RELEASE_(src2)
		_SAFE_RELEASE_(sdepth_map)
		_SAFE_RELEASE_(sdm)
		_SAFE_RELEASE_(fdepth_map)
		return res;
	}

	//������������ ������
	_SAFE_RELEASE_(src1)
	_SAFE_RELEASE_(src2)
	_SAFE_RELEASE_(sdepth_map)
	_SAFE_RELEASE_(sdm)
	_SAFE_RELEASE_(fdepth_map)
	return res;
}
static int  _RemoveBorder(IplImage* src, IplImage* dst, int lx, int rx)
{
	if (src == NULL || dst == NULL)
		return ERR_INVALID_PARAMS;
	if (lx < 0 || lx > cnMaxShift || rx < 0 || rx > cnMaxShift)
		return ERR_INVALID_PARAMS; 
	IplImage* tmp = NULL;
	CvSize s; 
	s.height = src->height;
	s.width = src->width - lx - rx;
	tmp = cvCreateImage(s, src->depth, src->nChannels);
	if (tmp == NULL)
		return ERR_MEMORY;
	//�������� ������ � awpColor
	awpColor* psrc = NULL;//������� ��������� �����������
	awpColor* ptmp = NULL;//������� ���������� �����������
	for (int y = 0; y < src->height; y++)
	{
		int c = 0;
		psrc = (awpColor*)((unsigned char*)src->imageData + y*src->widthStep);
		ptmp = (awpColor*)((unsigned char*)tmp->imageData + y*tmp->widthStep);
		for (int x = lx; x < src->width - rx; x++)
		{
			ptmp[c++] = psrc[x];
		}
	}

	cvResize(tmp, dst);
	cvReleaseImage(&tmp);
	return NOERROR;
}

// �������� �������� ������ �������� ��������� 
// ����������� ����������������� �� ������ ����������� 
//1. �������� ����������� �� �����
//2. ��������� ����������� ����� �������
//3. ��������� ������� ����� �������
//4. ��������� ����������� ��� ������ �����
//5. ��������� ����������� ��� ������� �����
//6. ��������� ����������� �����������
//7. ���������� ����������� �����������


// ����������� ���� ���������� ���� ���������� �����������
static int _Make3DAnaglyph(IplImage* pSrc, IplImage* pDst,int options)
{
	if (options > 4)
		return ERR_INVALID_PARAMS;
	int res = NOERROR;
	CvSize s; s.width = pSrc->width;s.height = pSrc->height;
	IplImage* dm = cvCreateImage(s, IPL_DEPTH_8U, 1);

	res = _MakeDepthMap(pSrc, dm);
	if (res < 0)
	{
		cvReleaseImage(&dm);
		return res;
	}

	IplImage* lep = NULL;		//����������� ��� ������ �����
	IplImage* rep = NULL;		//����������� ��� ������� �����		
	// ���������� ����������� ��� ������ � ������� �����
	lep = cvCreateImage(s, pSrc->depth, pSrc->nChannels);
	rep = cvCreateImage(s, pSrc->depth, pSrc->nChannels);
	if (lep == NULL || rep == NULL)
	{
		if (lep != NULL) cvReleaseImage(&lep);
		if (rep != NULL) cvReleaseImage(&rep);
		return ERR_CREATE_STEREOPAIR;//������� �� ����� ������� ����������� ��� ������(�������) �����
	}
	Displace(pSrc, dm, lep, -cShift);
	Displace(pSrc, dm, rep, cShift);
#ifdef _DEBUG
	cvSaveImage("debug_lep.png", lep);
	cvSaveImage("debug_rep.png", rep);
#endif

	IplImage* tmp = NULL;
	tmp = cvCreateImage(s, pSrc->depth, pSrc->nChannels);
	MakeAnaglyph(lep,rep, tmp, options);
	_RemoveBorder(tmp, pDst, cShift, cShift); 
	cvReleaseImage(&tmp);
	cvReleaseImage(&lep);
	cvReleaseImage(&rep);
	cvReleaseImage(&dm);

	return res;
}
// �������� ������� �� ����������� ��� ������ � ������� �����
int _Make3DTile(IplImage* pSrc, IplImage* pDst)
{
	int res = NOERROR;
	CvSize s; s.width = pSrc->width;s.height = pSrc->height;
	IplImage* dm = cvCreateImage(s, IPL_DEPTH_8U, 1);

	res = _MakeDepthMap(pSrc, dm);
	if (res < 0)
	{
		cvReleaseImage(&dm);
		return res;
	}

	IplImage* lep = NULL;		//����������� ��� ������ �����
	IplImage* rep = NULL;		//����������� ��� ������� �����		
	// ���������� ����������� ��� ������ � ������� �����
	lep = cvCreateImage(s, pSrc->depth, pSrc->nChannels);
	rep = cvCreateImage(s, pSrc->depth, pSrc->nChannels);
	if (lep == NULL || rep == NULL)
	{
		if (lep != NULL) cvReleaseImage(&lep);
		if (rep != NULL) cvReleaseImage(&rep);
		return ERR_MEMORY;//������� �� ����� ������� ����������� ��� ������(�������) �����
	}
	IplImage* tmp = cvCreateImage(s, pSrc->depth, pSrc->nChannels);
	if (tmp == NULL)
	{
		cvReleaseImage(&lep);
		cvReleaseImage(&rep);
		cvReleaseImage(&dm);
		return ERR_MEMORY;
	}


	Displace(pSrc, dm, tmp, -cShift);
	_RemoveBorder(tmp,lep,0,cShift);
	Displace(pSrc, dm, tmp,cShift);
	_RemoveBorder(tmp,rep,cShift,0);


#ifdef _DEBUG
	cvSaveImage("debug_lep.png", lep);
	cvSaveImage("debug_rep.png", rep);
#endif

	MakeTile(lep,rep, pDst);

	cvReleaseImage(&lep);
	cvReleaseImage(&rep);
	cvReleaseImage(&dm);
	return res;
}
//��������� ���� �������� � ������ 
static int _InsertContFrame(IplImage* fr, IplImage* cnt, int x, int y)
{
	int i,j;
	unsigned char* b = (unsigned char*)cnt->imageData;
	unsigned char* f = (unsigned char*)fr->imageData;
	for (i = 0; i < cnt->height; i++)
	{
		for (j = 0; j < cnt->widthStep; j++)
		{
			f[(i + y)*fr->widthStep + j + 3*x] = b[i*cnt->widthStep + j];
		}
	}
	return NOERROR;
}
// �� ����� ������� ��������� ���������� 
static int _Make3DVideo(IplImage* src, const char* lpDstName, int options)
{
	if (options <6 || options > 8)
		return ERR_INVALID_PARAMS;
	int res = NOERROR;
	CvSize s; s.width = src->width;s.height = src->height;
	IplImage* dm = cvCreateImage(s, IPL_DEPTH_8U, 1);
	res = _MakeDepthMap(src, dm);
	if (res < 0)
	{
		cvReleaseImage(&dm);
		return res;
	}
	int w,h,w1,h1,x,y;
	float alfa1 = 1;
	float alfa2 = 1;
	switch(options)
	{
		case VGAAvi:
			w1 = 640;h1 = 480;
			break;
		case HDAvi720:
			w1 = 1280; h1 = 720;
			break;
		case HDAvi1080:
			w1 = 1920; h1 = 1080;
			break;
	}
	alfa1 = (float)w1 / (float)h1;
	alfa2 = (float)src->width / (float)src->height;
	// ���������� ����������� � ����������
	float alfa = 1.f;
	if (alfa1 > alfa2)
	{
		alfa = (float)src->height / (float)h1;
		w = (int)(src->width / alfa);
		h = h1;
		y = 0;
		x = abs(w1 - w) / 2;
	}
	else
	{
		alfa = (float)w1 / (float)src->width;
		h = (int)(src->height * alfa);
		w = w1;
		x = 0;
		y = abs(h1 - h) / 2;
	}
    s.height = h; s.width = w;
	CvSize sf; sf.height = h1; sf.width = w1;
	IplImage* frame = cvCreateImage(sf, IPL_DEPTH_8U, 3);
	IplImage* cont = cvCreateImage(s, IPL_DEPTH_8U, 3);
	IplImage* tmp = cvCreateImage(s, src->depth, src->nChannels);

	IplImage* src1 = cvCreateImage(s, IPL_DEPTH_8U, 3);
	IplImage* dm1 = cvCreateImage(s, IPL_DEPTH_8U, 1);
	cvResize(src, src1);
	cvResize(dm, dm1);
	
	CvVideoWriter* writer = NULL;
	double fps = 24;
	writer = cvCreateVideoWriter(lpDstName, CV_FOURCC('I','Y','U','V'), fps,sf);
	int delta = -20;
	for (int i = 0; i < 40; i++)
	{
		cvZero(frame);
		delta += 1;
		Displace1(src1, dm1, tmp, delta);
		_RemoveBorder(tmp,cont, 20,20);
		_InsertContFrame(frame,cont,x,y);
		cvWriteFrame(writer, frame);
	}
	cvReleaseVideoWriter(&writer);
	cvReleaseImage(&dm);
	cvReleaseImage(&frame);
	cvReleaseImage(&cont);
	cvReleaseImage(&src1);
	cvReleaseImage(&dm1);
	cvReleaseImage(&tmp);
	return res;
}
#define WAIT_TIME 50
static int _ConvertVideo23D(const char* lpSrcName, const char* lpDstName, int Options)
{
	int res = NOERROR;
	if (lpSrcName == NULL || lpDstName == NULL || Options < 10)
		return ERR_INVALID_PARAMS;
	CvCapture* capture = NULL;
	CvVideoWriter* writer = NULL;
	capture = cvCaptureFromFile(lpSrcName);
	if (capture == NULL)
		return ERR_LOAD_SOURCE;

	// ��������� ���������� � �����
	double val = 0;
	CvSize s;
	val = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	s.width = (int)val;
	val = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	s.height = (int)val;
	val = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	double fps = val;
	val = cvGetCaptureProperty(capture, CV_CAP_PROP_FOURCC);
	int fourcc = (int)val;
	val = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
#ifdef _DEBUG	
	printf("%i\n",(int)val);
#endif
	writer = cvCreateVideoWriter(lpDstName, 0, fps, s);
	if (writer == NULL)
	{
		cvReleaseCapture(&capture);	
		return ERR_CREATE_DESTINATION;
	}

	IplImage* pimg = NULL;
	IplImage* pdst = NULL;
	int c = 0;
#ifdef _DEBUG
	int KeyCode = 0;
	cvNamedWindow("src");
#endif 
	do
	{
#ifdef _DEBUG		
		KeyCode = cvWaitKey(50);
		if (KeyCode == VK_ESCAPE)
			break;
#endif
		
		pimg = cvQueryFrame(capture);
		if (pimg == NULL)
			break;
		CvSize s1;
		s1.height = pimg->height;
		s1.width = pimg->width;
		pdst = cvCreateImage(s1, pimg->depth, pimg->nChannels);
		if (pdst == NULL)
			break;
		//cvCopy(pimg, pdst);
		_Make3DAnaglyph(pimg, pdst, ColorAnaglyph);
//		cvSaveImage("out.jpg", pdst);
#ifdef _DEBUG
		cvShowImage("src", pdst);
#endif 
		cvWriteFrame(writer, pdst);
		cvReleaseImage(&pdst);
		c++;
#ifdef _DEBUG
		if (c % 100 == 0) 
			printf(".");
#endif 
	}while(true);
	cvReleaseCapture(&capture);
	cvReleaseVideoWriter(&writer);
	return res;
}
int Make3DAnaglyph(const char* lpSrcName, const char* lpDstName,int options)
{
	if (lpSrcName == NULL || lpDstName == NULL || options < 0 || options > 10)
		return ERR_INVALID_PARAMS;
	if (!AcceptFileName(lpSrcName, options, true))
		return ERR_INVALID_PARAMS;
	if (!AcceptFileName(lpDstName, options, false))
		return ERR_INVALID_PARAMS;
	IplImage* pSrc = NULL;
	IplImage* pDst = NULL;
	//�������� ��������� �����������
	if (options < 10)
	{
		pSrc = cvLoadImage(lpSrcName);
		if (pSrc == NULL)
			return ERR_LOAD_SOURCE; // ������� �� ����� ��������� �������� �����������
	}
	CvSize s;
	int result = 0;
	if (options < 5)
	{
		s.width = pSrc->width; s.height = pSrc->height;
		pDst = cvCreateImage(s, pSrc->depth, pSrc->nChannels);
		if (pDst == NULL)
		{
			cvReleaseImage(&pSrc);
			return ERR_MEMORY; // ������� �� ����� ������� ����������� ��� ���������� ����������
		}
		result  = _Make3DAnaglyph(pSrc,pDst, options);
		if (result == NOERROR)
		{
			//� ������ ��������� �������������� ����������� � �������
			//���������� ������� � ����
			result = cvSaveImage(lpDstName, pDst) == 0? ERR_SAVE_ANAGLYPH:0;
		}
		cvReleaseImage(&pDst);
	}
	else if (options == 5)
	{	
		//todo: �o�������� �������
		s.width = 2*pSrc->width; s.height = pSrc->height;
		pDst = cvCreateImage(s, pSrc->depth, pSrc->nChannels);
		if (pDst == NULL)
		{
			cvReleaseImage(&pSrc);
			return ERR_MEMORY; // ������� �� ����� ������� ����������� ��� ���������� ����������
		}
		result = _Make3DTile(pSrc,pDst);
		if (result == NOERROR)
		{
			//� ������ ��������� �������������� ����������� � �������
			//���������� ������� � ����
			result = cvSaveImage("tmp_file.jpg", pDst) == 0? ERR_SAVE_ANAGLYPH:0;

			//todo: �������������� �����?
			if (result == 0)
			{
				rename("tmp_file.jpg", lpDstName);
				remove("tmp_file.jpg");
			}
		}
		cvReleaseImage(&pDst);
	}
	else if (options < 9)
	{
		//todo: ���������� �����������
		result = _Make3DVideo(pSrc, lpDstName, options); 
	}
	else if (options == 9)
	{
		//todo: ���������� GIF ��������
	
	}
	else 
		//����������� �����
		result = _ConvertVideo23D(lpSrcName, lpDstName, options);
	cvReleaseImage(&pSrc);
	return result;
}









