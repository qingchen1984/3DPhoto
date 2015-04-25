#include "_3DAnaglyph.h"
const int cnBufSize = 5;
bool CheckFileNameExt(const char* lpExt, const char* lpFileName)
{
	char Ext[cnBufSize];
	char* pExt = Ext;
	int len = strlen(lpFileName);
	if (len < cnBufSize)
		return false;
	char c = lpFileName[len];
	while (c != '.')
	{
		len--;
		if (len <= 0)
			return false;
		c = lpFileName[len];
	}
	if (strlen(lpFileName)- len > cnBufSize)
		return false;
	strcpy(Ext,&lpFileName[len]);
	pExt = _strlwr(Ext);
	return strcmp(lpExt, pExt) == 0;
}

static bool _isCVImageName(const char* lpFileName)
{
		//проверяем расширение имени файла для входного изображения
		bool res = false;
		res = CheckFileNameExt(".jpg",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".jpeg",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".png",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".bmp",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".pbm",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".pgm",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".ppm",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".sr",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".ras",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".tif",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".tiff",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".avi",lpFileName);
		if (res) return res;
		res = CheckFileNameExt(".jps",lpFileName);
		return res;
}

bool AcceptFileName(const char* lpFileName, int options, bool isSrc)
{
	if (options < 0 || options > MacroVideo)
		return false;
	if (isSrc)
	{
		return _isCVImageName(lpFileName);
	}
	else
	{
		if (options <= TiledImages)
			return _isCVImageName(lpFileName);
		else if (options <= HDAvi1080)
			return CheckFileNameExt(".avi", lpFileName);
		else if (options == AnimatedGif)
			return CheckFileNameExt(".gif", lpFileName);
		else if (options == MacroVideo)
			return CheckFileNameExt(".avi", lpFileName);
		else
			return false;
	}
	return false;
}