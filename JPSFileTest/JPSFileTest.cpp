// JPSFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\JPSFile\JPSFile.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	IJPSFile* jfile = NULL;
	createJPSFile(&jfile);
	if (!jfile->LoadFromFile("out1.jps"))
	{
		printf("Cannot load file.\n");
		return -1;
	}

	IImage* lep = jfile->GetLep();
	IImage* rep = jfile->GetRep();
	if (lep)
	{
		lep->Save("lep.jpg");
		lep->Rlease();
	}
	if (rep)
	{
		rep->Save("rep.jpg");
		rep->Rlease();
	}
	IImage* tanaglyph = jfile->GetTrueAnaglyph();
	if (tanaglyph != NULL)
	{
		tanaglyph->Save("tanaglyph.jpg");
		tanaglyph->Rlease();
	}

	IImage* canaglyph = jfile->GetColorAnaglyph();
	if (canaglyph != NULL)
	{
		canaglyph->Save("canaglyph.jpg");
		canaglyph->Rlease();
	}

	IImage* ganaglyph = jfile->GetGrayAnaglyph();
	if (ganaglyph != NULL)
	{
		ganaglyph->Save("ganaglyph.jpg");
		ganaglyph->Rlease();
	}

	IImage* oanaglyph = jfile->GetOptimizedAnglyph();
	if (oanaglyph != NULL)
	{
		oanaglyph->Save("oanaglyph.jpg");
		oanaglyph->Rlease();
	}
	jfile->SaveToFile("test.jps");
	jfile->Rlease();
	return 0;
}

