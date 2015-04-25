// test02.cpp: решение большой разреженной системы уравнений. 
// программа читает разреженную матрицу, сформированную с помощью 
// MATLAB, правую часть уравнения и решает систему линейных уравнений
// с помощью библиотеки CSparce. результат решения записывает 
// в виде текстового файла	
#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	cs_sparse* A = NULL;
	DWORD t,tc = GetTickCount();
	// чтение матрицы A из файла "data.txt"
	printf("converting 'data.txt'.....");
	FILE* F = fopen("data.txt", "r+t");
	if (F == NULL)
		return -1;
	FILE* F1 = fopen("data1.txt", "w+t");
	int r,c;
	double v;
	while(fscanf(F, "%d %d %lg", &r, &c,&v) > 0)
		fprintf(F1,"%d %d %lg\n",r-1,c-1,v);
	fclose(F);
	fclose(F1);
	t = GetTickCount() - tc;
	printf("%ld ms\n", t);
	tc = GetTickCount();
	F = fopen("data1.txt", "r+t");
	printf("loading matrix.....");
	A = cs_load(F);
	cs_sparse* A1 = cs_compress(A);
	cs_spfree(A);
	fclose(F);F = NULL;
	t = GetTickCount() - tc;
	printf("%ld ms\n", t);
	tc = GetTickCount();
	// чтение правой части (b) из файла "b.txt"
	printf("loading vector.....");
	F = fopen("b.txt", "r+t");
	if (F == NULL)
		return -1;
	double* b = (double*)malloc(A1->m*sizeof(double));
	memset(b, 0, A1->m*sizeof(double));

	for (int i = 0; i < A1->m; i++)
	{
		fscanf(F, " %lg",&b[i]);
	}
	fclose(F);
	t = GetTickCount() - tc;
	printf("%ld ms\n", t);
	tc = GetTickCount();
	printf("solving .....");
	if (cs_cholsol(1, A1, b) != 1)
	{
		printf("error.\n");
		return -1;
	}
	t = GetTickCount() - tc;
	printf("%ld ms\n", t);
	tc = GetTickCount();
	printf("saving result .....");
	F = fopen("b1.txt", "w+t");
	for (int i = 0; i < A1->m; i++)
	{
		fprintf(F, " %lg\n",b[i]);
	}
	cs_spfree(A1);
	fclose(F);
	t = GetTickCount() - tc;
	printf("%ld ms\n", t);
	return 0;
}

