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
	~CCvImage();

	virtual bool __stdcall Init(int widht, int height, int bpp,  int cannels, unsigned char* pixels);
	virtual bool __stdcall Load(const char* lpFilename);
	virtual bool __stdcall Save(const char* lpFilename);
	virtual bool __stdcall IsEmpty();
	virtual int  __stdcall Width();
	virtual int  __stdcall Height();
	virtual int  __stdcall Bpp();
	virtual int  __stdcall Channels();
	virtual int  __stdcall WidthStep();
	virtual unsigned char* __stdcall Pixels();	
};


CCvImage::CCvImage()
{
	this->m_image = NULL;
}
CCvImage::~CCvImage()
{
	Clear();
}

void CCvImage::Clear()
{
	if (this->m_image != NULL)
		cvReleaseImage(&this->m_image);
	this->m_image = NULL;
}


bool CCvImage::Init(int width, int height, int bpp,  int cannels, unsigned char* pixels)
{
	this->Clear();
	CvSize s;
	s.height = height;
	s.width  = width;

	m_image = cvCreateImage(s,bpp, cannels );
	if (m_image != NULL)
	{
		// copy data
		
		return true;
	}
	else
		return false;
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
	return cvSaveImage(lpFilename, this->m_image) == 0;
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
	virtual void __stdcall Rlease();
	virtual IImage*  __stdcall GetLep();
	virtual IImage*  __stdcall GetRep();
	virtual IImage*  __stdcall GetJPS();
	virtual IImage*  __stdcall GetTrueAnaglyph();
	virtual IImage*  __stdcall GetGrayAnaglyph();
	virtual IImage*  __stdcall GetColorAnaglyph();
	virtual IImage*  __stdcall GetOptimizedAnglyph();
};


CJPSFile::CJPSFile() 
{
	m_image = NULL;
}

CJPSFile::~CJPSFile()
{

}

bool CJPSFile::LoadFromFile(LPCSTR lpFileName)
{
	
 	if (lpFileName == NULL)
		return false;

	if (!CheckFileNameExt(".jps",  lpFileName))
		return false;

	IplImage* img = NULL;
	img = cvLoadImage(lpFileName);
	if (img != NULL ) 
	{
		__try
		{
			
		}
		__finally
		{
			cvReleaseImage(&img);
		}
		return true;
	}
	return false;
}
bool CJPSFile::SaveToFile(const char* lpFileName)
{
	return false;
}
bool CJPSFile::Init(IImage* src)
{
	return false;
}

void CJPSFile::Rlease()
{
	delete this;
}


IImage*  CJPSFile::GetLep()
{
	return NULL;
}
IImage*  CJPSFile::GetRep()
{
	return NULL;
}
IImage*  CJPSFile::GetJPS()
{
	return NULL;
}
IImage*  CJPSFile::GetTrueAnaglyph()
{
	return NULL;
}
IImage*  CJPSFile::GetGrayAnaglyph()
{
	return NULL;
}
IImage*  CJPSFile::GetColorAnaglyph()
{
	return NULL;
}
IImage*  CJPSFile::GetOptimizedAnglyph()
{
	return NULL;
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