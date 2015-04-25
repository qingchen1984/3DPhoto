// demo.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
// печатает информацию об использовании программы
void Usage()
{
		printf("Usage: demo n input.jpg out.jpg\n");
		printf("n = 0 :make true anaglyph\n");
		printf("n = 1 :make gray anaglyph\n");
		printf("n = 2 :make color anaglyph\n");
		printf("n = 3 :make half color anaglyph\n");
		printf("n = 4 :make optimized anaglyph\n");
		printf("n = 5 :make tile stereo\n");
		printf("n = 6 :make VGA video\n");
		printf("n = 7 :make HD 720p video \n");
		printf("n = 8 :make HD 1080p video \n");
		printf("n = 9 :convert macro video.");
}

int _tmain(int argc, _TCHAR* argv[])
{
	int rc = 0;
	if (argc < 4)
	{
		Usage();
		return -1;
	}
	DWORD ts = GetTickCount();
	int options = atoi(argv[1]);
	rc = Make3DAnaglyph(argv[2], argv[3], options);
	printf("Processing time: %d ms\n", GetTickCount()-ts);
	switch(rc)
	{
	case 0:
		printf("done.\n");
		break;
	case -1:
		printf("Error. Cannot load source image.\n");
		break;
	case -2:
		printf("Error.  Cannot create destination image. Out of memory?\n");
		break;
	case -3:
		printf("Error. Cannot create depth map. Out of memory?\n");
		break;
	case -4:
		printf("Error. Invalid params.\n");
		break;
	case -5:
		printf("Error. Cannot create anaglyph. Out of memory?\n");
		break;
	case -6:
		printf("Error. Cannot save anaglyph.\n");
		break;
	case -7:
		printf("Error. Cannot make depth map.\n");
		break;

	}

	return 0;
}
