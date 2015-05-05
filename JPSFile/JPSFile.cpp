// JPSFile.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "JPSFile.h"

extern "C"
{
	#pragma comment(lib, "../lib/cv210.lib") 
	#pragma comment(lib, "../lib/cxcore210.lib") 
	#pragma comment(lib, "../lib/highgui210.lib") 
}

class CCvImage : public IImage
{
private:
	IplImage* m_image;
	void Clear();
public:

	CCvImage();
	CCvImage(CCvImage& image);
	~CCvImage();
	virtual void __stdcall Rlease();
	virtual bool __stdcall Init(int widht, int height, int bpp,  int cannels, unsigned char* pixels);
	virtual bool __stdcall Init(IplImage* image);
	virtual bool __stdcall Load(const char* lpFilename);
	virtual bool __stdcall Save(const char* lpFilename);
	virtual bool __stdcall IsEmpty();
	virtual int  __stdcall Width();
	virtual int  __stdcall Height();
	virtual int  __stdcall Bpp();
	virtual int  __stdcall Channels();
	virtual int  __stdcall WidthStep();
	virtual unsigned char* __stdcall Pixels();
	IplImage* GetIplImage();
};


CCvImage::CCvImage()
{
	this->m_image = NULL;
}
CCvImage::CCvImage(CCvImage& image)
{
	if (image.m_image != NULL)
	{
		this->m_image = cvCloneImage(image.m_image);
	}
	else
		this->m_image = NULL;
}

CCvImage::~CCvImage()
{
	Clear();
}

void __stdcall CCvImage::Rlease()
{
	delete this;
}

void CCvImage::Clear()
{
	if (this->m_image != NULL)
		cvReleaseImage(&this->m_image);
	this->m_image = NULL;
}

//предполагаем, что pixels содержит непрерывный набор строк, без выравнивания. 
//в случае, когда строки выровнены на 4 байта или как-то иначе их необходимо 
//предварительно скопировать в специальный буфер без выравнивания, котороый и передавать в 
//эту функцию. 
bool CCvImage::Init(int width, int height, int bpp,  int cannels, unsigned char* pixels)
{
	this->Clear();
	CvSize s;
	s.height = height;
	s.width  = width;

	m_image = cvCreateImage(s,bpp, cannels );
	if (m_image != NULL)
	{
		//todo: copy data
		unsigned char* dst = (unsigned char*)m_image->imageData;
		unsigned char* p   = pixels;
		int step = cannels*width*sizeof(unsigned char);
		for (int y = 0; y < height; y++)
		{
			memcpy(dst, p, step);
			p   += step;
			dst += m_image->widthStep;
		}
		return true;
	}
	else
		return false;
}

bool __stdcall CCvImage::Init(IplImage* image)
{
	this->Clear();
	if (image == NULL)
		return false;
	this->m_image = cvCloneImage(image);
	return this->m_image != NULL;
}

bool CCvImage::Load(const char* lpFilename)
{
	this->Clear();
	this->m_image = cvLoadImage(lpFilename);
	return !this->IsEmpty();
}

bool CCvImage::Save(const char* lpFilename)
{
	if (IsEmpty())
		return false;
	return cvSaveImage(lpFilename, this->m_image);
}

bool CCvImage::IsEmpty()
{
	return this->m_image == NULL;
}
int  CCvImage::Width()
{
	if (IsEmpty())
		return 0;
	return this->m_image->width;
}
int  CCvImage::Height()
{
	if (IsEmpty())
		return 0;
	return this->m_image->height;
}
int  CCvImage::Bpp()
{
	if (IsEmpty())
		return 0;
	return this->m_image->depth;
}
int  CCvImage::Channels()
{
	if (IsEmpty())
		return 0;
	return this->m_image->nChannels;
}
int  CCvImage::WidthStep()
{
	if (IsEmpty())
		return 0;
	return this->m_image->widthStep;
}
unsigned char* CCvImage::Pixels()
{
	if (IsEmpty())
		return 0;
	return (unsigned char*)this->m_image->imageData;
}

IplImage* CCvImage::GetIplImage()
{
	return this->m_image;
}



class CJPSFile : public IJPSFile
{
private:
	CCvImage*	m_image; 
	CCvImage*	m_lep;
	CCvImage*	m_rep;

public:
	CJPSFile();
	~CJPSFile();

	virtual bool __stdcall LoadFromFile(LPCSTR lpFileName);
	virtual bool __stdcall SaveToFile(const char* lpFileName);
	virtual bool __stdcall Init(IImage* src);
	virtual bool __stdcall InitFromFile(const char* lpFileName);
	virtual void __stdcall Rlease();
	virtual IImage*  __stdcall GetLep();
	virtual IImage*  __stdcall GetRep();
	virtual IImage*  __stdcall GetJPS();
	virtual IImage*  __stdcall GetTrueAnaglyph();
	virtual IImage*  __stdcall GetGrayAnaglyph();
	virtual IImage*  __stdcall GetColorAnaglyph();
	virtual IImage*  __stdcall GetOptimizedAnglyph();
	virtual bool     __stdcall IsEmpty();
};


CJPSFile::CJPSFile() 
{
	m_image = new CCvImage();
	m_lep = new CCvImage();
	m_rep = new CCvImage();
}

CJPSFile::~CJPSFile()
{
	if (this->m_image != NULL)
		delete this->m_image;
	if (this->m_lep != NULL)
		delete this->m_lep;
	if (this->m_rep != NULL)
		delete this->m_rep;

}

bool   CJPSFile::IsEmpty()
{
	return this->m_image->IsEmpty();
}

bool CJPSFile::LoadFromFile(LPCSTR lpFileName)
{
	
 	if (lpFileName == NULL)
		return false;

	if (!CheckFileNameExt(".jps",  lpFileName))
		return false;

	IplImage* img = NULL;
	IplImage* imgLep = NULL;
	IplImage* imgRep = NULL;

	img = cvLoadImage(lpFileName);
	if (img != NULL ) 
	{
		bool result = false;
		__try
		{
			int width  = img->width / 2;
			int height = img->height;

			CvSize size;
			size.width	= width;
			size.height = height;

			imgLep = cvCreateImage(size, img->depth,img->nChannels);
			imgRep = cvCreateImage(size, img->depth,img->nChannels);

			if (imgLep == NULL || imgRep == NULL)
				throw 0;

			// copy data
			unsigned char* src = (unsigned char*)img->imageData;
			unsigned char* lep = (unsigned char*)imgLep->imageData;
			unsigned char* rep = (unsigned char*)imgRep->imageData;

			for (int y = 0; y < height; y++)
			{
				memcpy(rep, src, width*img->nChannels);
				src += width*img->nChannels;
				memcpy(lep, src, width*img->nChannels);
				src += (img->widthStep - width*img->nChannels);
				rep += imgRep->widthStep;
				lep += imgLep->widthStep;
			}

			this->m_image->Init(img);
			this->m_lep->Init(imgLep);
			this->m_rep->Init(imgRep);

			result = true;
		}
		__finally
		{
			cvReleaseImage(&img);
			cvReleaseImage(&imgLep);
			cvReleaseImage(&imgRep);
		}
		return result;
	}
	return false;
}
bool CJPSFile::SaveToFile(const char* lpFileName)
{
	if (lpFileName== NULL)
		return false;
	if (this->m_image->IsEmpty())
		return false;
	bool result = this->m_image->Save("__tmp.jpg");
	if (result)
	{
		rename("__tmp.jpg", lpFileName);
		remove("__tmp.jpg");	
	}
	return result;
}

bool CJPSFile::Init(IImage* src)
{
	if (src == NULL)
		return false;
	
	IplImage* dst = NULL;
	IplImage* isrc = ((CCvImage*)src)->GetIplImage();

	if (isrc == NULL)
		return false;

	CvSize size;
	size.height = isrc->height;
	size.width  = isrc->width*2;

	dst = cvCreateImage(size, isrc->depth, isrc->nChannels);
	if (dst == NULL)
		return false;

	int result = _Make3DTile(isrc, dst);

	if (result != NOERROR)
	{
		cvReleaseImage(&dst);
		return false;
	}

	IplImage* imgLep = NULL;
	IplImage* imgRep = NULL;

		bool res = false;
		__try
		{
			int width  = dst->width / 2;
			int height = dst->height;

			CvSize size;
			size.width	= width;
			size.height = height;

			imgLep = cvCreateImage(size, dst->depth,dst->nChannels);
			imgRep = cvCreateImage(size, dst->depth,dst->nChannels);

			if (imgLep == NULL || imgRep == NULL)
				throw 0;

			// copy data
			unsigned char* src = (unsigned char*)dst->imageData;
			unsigned char* lep = (unsigned char*)imgLep->imageData;
			unsigned char* rep = (unsigned char*)imgRep->imageData;

			for (int y = 0; y < height; y++)
			{
				memcpy(rep, src, width*dst->nChannels);
				src += width*dst->nChannels;
				memcpy(lep, src, width*dst->nChannels);
				src += (dst->widthStep - width*dst->nChannels);
				rep += imgRep->widthStep;
				lep += imgLep->widthStep;
			}

			this->m_image->Init(dst);
			this->m_lep->Init(imgLep);
			this->m_rep->Init(imgRep);

			res = true;
		}
		__finally
		{
			cvReleaseImage(&dst);
			cvReleaseImage(&imgLep);
			cvReleaseImage(&imgRep);
		}



	//this->m_image->Init(dst);
	//cvReleaseImage(&dst);

	return res;
}

bool CJPSFile::InitFromFile(const char* lpFileName)
{
	if (lpFileName == NULL)
		return false;
	
	IplImage* img = NULL;
	img = cvLoadImage(lpFileName);
	if (img == NULL)
		return false;

	CCvImage cvImage;// = new CCvImage();
	cvImage.Init(img);

	bool res = this->Init(&cvImage);

	cvReleaseImage(&img);
	return res;


}


void CJPSFile::Rlease()
{
	delete this;
}


IImage*  CJPSFile::GetLep()
{
	return new CCvImage(*this->m_lep);
}
IImage*  CJPSFile::GetRep()
{
	return new CCvImage(* this->m_rep);
}
IImage*  CJPSFile::GetJPS()
{
	return new CCvImage(*this->m_image);
}
IImage*  CJPSFile::GetTrueAnaglyph()
{
	IplImage* rep = this->m_rep->GetIplImage();
	IplImage* lep = this->m_lep->GetIplImage();

	if (rep == NULL || lep == NULL)
		return NULL;
	CvSize size;
	size.height = lep->height;
	size.width  = lep->width;

	IplImage* result = NULL;

	result = cvCreateImage(size, lep->depth, lep->nChannels);
	MakeAnaglyph(lep,rep,result, TrueAnaglyph);

	CCvImage* anaglyph = new CCvImage();
	anaglyph->Init(result);

	return anaglyph;
}
IImage*  CJPSFile::GetGrayAnaglyph()
{
	IplImage* rep = this->m_rep->GetIplImage();
	IplImage* lep = this->m_lep->GetIplImage();

	if (rep == NULL || lep == NULL)
		return NULL;
	CvSize size;
	size.height = lep->height;
	size.width  = lep->width;

	IplImage* result = NULL;

	result = cvCreateImage(size, lep->depth, lep->nChannels);
	MakeAnaglyph(lep,rep,result, GrayAnaglyph);

	CCvImage* anaglyph = new CCvImage();
	anaglyph->Init(result);

	return anaglyph;
}
IImage*  CJPSFile::GetColorAnaglyph()
{
	IplImage* rep = this->m_rep->GetIplImage();
	IplImage* lep = this->m_lep->GetIplImage();

	if (rep == NULL || lep == NULL)
		return NULL;
	CvSize size;
	size.height = lep->height;
	size.width  = lep->width;

	IplImage* result = NULL;

	result = cvCreateImage(size, lep->depth, lep->nChannels);
	MakeAnaglyph(lep,rep,result, ColorAnaglyph);

	CCvImage* anaglyph = new CCvImage();
	anaglyph->Init(result);

	return anaglyph;
}
IImage*  CJPSFile::GetOptimizedAnglyph()
{
	IplImage* rep = this->m_rep->GetIplImage();
	IplImage* lep = this->m_lep->GetIplImage();

	if (rep == NULL || lep == NULL)
		return NULL;
	CvSize size;
	size.height = lep->height;
	size.width  = lep->width;

	IplImage* result = NULL;

	result = cvCreateImage(size, lep->depth, lep->nChannels);
	MakeAnaglyph(lep,rep,result, OptimizedAnaglyph);

	CCvImage* anaglyph = new CCvImage();
	anaglyph->Init(result);

	return anaglyph;
}


extern "C"  JPSFILE_API HRESULT  WINAPI createJPSFile(IJPSFile** jpsFile)
{
	if (jpsFile == NULL)
		return E_FAIL;


	CJPSFile* jfile = new CJPSFile();
	if (jfile == NULL)
		return E_FAIL;
	*jpsFile = jfile;
	return S_OK;
}