#include "_3DAnaglyph.h"

static double* _GetMatMatrix(IplImage* src)
{
	double* b = NULL;
	double* a = (double*)src->imageData;
	b = (double*)malloc(src->width*src->height*sizeof(double));
	if (b == NULL)
		return b;
	int c = 0;
	for (int i = 0; i < src->width; i++)
		for (int j = 0; j < src->height; j++)
			b[c++] = a[j*src->width + i];
	return b;
}
/*	
	выполняет формирование лапласиана из изображений pSrc
	записывает полученный лапласиан в разреженную матрицу.	
*/
cs_sparse* _GetLaplasian1(IplImage* pSrc)
{
	cs_sparse* res = NULL;
	double*    p = NULL;
	p = _GetMatMatrix(pSrc);
	if (p == NULL)
		return NULL;

	double winII[9][9];
	double tvals[81];
	int win_inds_cols[81];
	int win_inds_rows[81];
	int win_indx[9];
	int win_size = 1;
	int neb_size = (win_size*2+1);
	neb_size *=neb_size;
	int neb_size2 = neb_size*neb_size;
	CvSize s; s.width = win_size*2+1; s.height = s.width;
	IplImage* winI = NULL;
	winI = cvCreateImage(s, IPL_DEPTH_64F,1);
	if (winI == NULL)
	{
		free(p);	
		return NULL;
	}
	int img_size = pSrc->width*pSrc->height;
	int sum = 0;
	int tlen = img_size*neb_size2;
	int cc = 0;
	CvScalar win_mu;
	double win_mu_d = 0;
	double win_var = 0;
	double* winIP = (double*)winI->imageData;
	res = cs_spalloc(img_size, img_size, img_size, 1, 1);
	for (int i = win_size; i < pSrc->width-win_size;i++)
	{
		for(int j = win_size; j < pSrc->height-win_size; j++)
		{
			// вычисление среднего значения по окну win_size вокруг точки i,j
			cc = 0;
			for (int k = i - win_size; k <= i+win_size; k++)
			{

				for (int m = j - win_size; m <= j+win_size; m++)
				{
					winIP[cc] = p[k*pSrc->height +m];
					win_indx[cc] = k*pSrc->height + m;
				    cc++;
				}
			}
			win_mu = cvAvg(winI);
			win_mu_d = win_mu.val[0];
			double winI_mul = 0;
			for (int g = 0; g < 9; g++)
				winI_mul += winIP[g]*winIP[g];
			winI_mul /= neb_size;
			win_var = winI_mul - win_mu_d*win_mu_d + 1.e-7/neb_size;
			win_var = 1/win_var;
			// вычитаем среднее из winI
			for (int g = 0;g < 9; g++)
				winIP[g] -= win_mu_d;
/*
			// переиндексация winIP по колонкам
			for (int g = 0; g < 3; g++)
				for (int t = 0; t < 3; t++)
					winIC[t*3 + g] = winIP[g*3 +t];
			for (int g = 0; g < 9; g++)
				for (int t = 0; t < 9; t++)
				{
					winII[t][g] =(1+ winIC[g]*winIC[t]*win_var)/neb_size;
				}
*/
			for (int ii = 0; ii < 9; ii++)
				for (int jj = 0; jj < 9; jj++)
					winII[ii][jj] = (1+winIP[ii]*winIP[jj]*win_var)/neb_size;

			// формируем tvals, win_inds_cols и win_inds_rows
			cc = 0;
			for (int g = 0; g < 9; g++)
				for (int t = 0; t < 9; t++)
				{
					tvals[cc] = winII[g][t];
					win_inds_cols[cc] = win_indx[g];
					win_inds_rows[cc] = win_indx[t];
					cc++;
				}
			for (int g = 0; g < 81; g++)
				cs_entry(res, win_inds_rows[g], win_inds_cols[g],tvals[g]);
		}
	}
//#ifdef _DEBUG
//	FILE* F = fopen("A.txt", "w+t");
//	for (int i = 0; i < res->nzmax; i++)
//		fprintf(F,"%i\t%i\t%f\n", res->i[i],res->p[i],res->x[i]); 
//	fclose(F);
//#endif
	cs_sparse* res1 =  cs_compress(res);
	cs_spfree(res);
	res = res1;
	cs_sparse* B;// = res;//cs_transpose(res, 1);
	double* S = (double*)malloc(res->n*sizeof(double));
	for (int j = 0; j < res->n;j++)
	{
		S[j] = 0;
		for (int k = res->p[j]; k < res->p[j+1]; k++)
			S[j] +=  res->x[k];
	}
	//cs_spfree(B);
	B =  cs_spalloc(img_size, img_size, img_size, 1, 1);
	for (int i = 0; i < B->n; i++)
		cs_entry(B, i,i, S[i]);
	cs_sparse* res2 = cs_compress(B);
	cs_sparse* res3 = cs_add(res2, res, 1,-1);
	free(S);cs_spfree(B);cs_spfree(res2);cs_spfree(res);
	free(p);
	return res3;
}
/*
	преобразование изображения в диагональную матрицу
*/
static cs_sparse* _SparseDiag(IplImage* pImage)
{
	/// 
	double* p = _GetMatMatrix(pImage);
	int count = 0;
	int img_size = pImage->width*pImage->height;
	cs_sparse* res = NULL;	
	res = cs_spalloc(img_size, img_size, img_size, 1, 1);
	if (res != NULL)
	{
		// добавление значений в матрицу
		int c = 0;
		for (int i = 0; i < img_size; i++)
				cs_entry(res, i, i, p[i]);
		cs_sparse* res1 =  cs_compress(res);
		cs_spfree(res);
		free(p);
		return res1;
	}
	return NULL;
}
/*
	выполняет интерполяцию изображения. 
*/
int Interpolate1(IplImage* pSrc, IplImage* pConstMap, IplImage* pConstVals, IplImage* pResult)
{
	//todo:
	int res = 0;
	int img_size = pSrc->width*pSrc->height;
	cs_sparse* A = _GetLaplasian1(pSrc);
	if (A == NULL)
		return -1;

	cs_sparse* D = _SparseDiag(pConstMap);
	if (D == NULL)
	{
		cs_spfree(A);
		return -1;
	}
	double lambda = 0.1;
	cs_sparse* A1 = cs_add(A,D,1, lambda);
	if (A1 == NULL)
	{
		cs_spfree(A);
		cs_spfree(D);
		return -1;
	}
	double* d = (double*)cs_calloc(img_size, sizeof(double));
//	double* d1 = (double*)pConstMap->imageData;
//	double* d2 = (double*)pConstVals->imageData;

	double* d1 = _GetMatMatrix(pConstMap);
	double* d2 = _GetMatMatrix(pConstVals);

#ifdef _DEBUG
	FILE* F = fopen("bb.txt", "w+t");
#endif 
	for (int i = 0; i < img_size; i++)
	{
		d[i] = lambda*d1[i]*d2[i];
#ifdef _DEBUG
		fprintf(F, "%lg\n", d[i]);
#endif
	}
#ifdef _DEBUG
	fclose(F);
#endif 
	if (cs_cholsol(1,A1, d) == 0)
	{
		cs_spfree(A);
		cs_spfree(D);
		cs_spfree(A1);
		cs_free(d);
		return -1;
	}
	
	cs_spfree(A);
	cs_spfree(D);
	cs_spfree(A1);
	double* d3 = (double*) pResult->imageData;
	for (int i = 0; i < pResult->height; i++)
		for (int j = 0; j < pResult->width; j++)
			d3[i*pResult->width + j] = d[j*pResult->height + i];
	cs_free(d);
	free(d1);
	free(d2);
	return res;
}
