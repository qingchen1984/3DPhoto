// JPSFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\JPSFile\JPSFile.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	IJPSFile* jfile = NULL;
	createJPSFile(&jfile, "11");
	jfile->LoadFromFile("qq.jps", 3);
	return 0;
}

