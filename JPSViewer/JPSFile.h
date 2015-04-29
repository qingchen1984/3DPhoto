#ifndef _JPSFile_h_
#define _JPSFile_h_

#ifdef JPSFILE_EXPORTS
#define JPSFILE_API __declspec(dllexport)
#else
#define JPSFILE_API __declspec(dllimport)
#endif

// JPS File Interface 
class IImage
{
public:
	virtual bool Init(int widht, int height, int bpp, unsigned char* pixels) = 0;
	virtual bool Load(const char* lpFilename) = 0;
	virtual bool Save(const char* lpFilename) = 0;
	virtual bool IsEmpty() = 0;
	virtual int  Width() = 0;
	virtual int  Height() = 0;
	virtual int  Bpp() = 0;
	virtual int  Channels() = 0;
	virtual int  WidthStep() = 0;
	virtual unsigned char* Pixels() = 0;
};
 class  IJPSFile
{
public:	
	virtual void __stdcall Rlease() = 0;
	virtual bool __stdcall LoadFromFile(LPCSTR lpFileName) = 0;
	virtual bool __stdcall SaveToFile(const char* lpFileName) = 0;
	virtual bool __stdcall Init(IImage* src) = 0;
	virtual IImage*  __stdcall GetLep() = 0;
	virtual IImage*  __stdcall GetRep() = 0;
	virtual IImage*  __stdcall GetJPS() = 0;
	virtual IImage*  __stdcall GetTrueAnaglyph() = 0;
	virtual IImage*  __stdcall GetGrayAnaglyph() = 0;
	virtual IImage*  __stdcall GetColorAnaglyph()= 0;
	virtual IImage*  __stdcall GetOptimizedAnglyph() = 0;
};


extern "C"  JPSFILE_API HRESULT  WINAPI createJPSFile(IJPSFile** jpsFile);

#endif// _JPSFile_h_