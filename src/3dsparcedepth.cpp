#include "_3DAnaglyph.h"

void MakeSparceDepthMap(IplImage* src, IplImage* depth_map)
{
const int resize_f = 1;
const float canny_thr = 30;
//глобальные переменные
IplImage* gray  = NULL;				// исходное полутоновое изображение 
IplImage* gray_resized = NULL;		// исходное полутоновое изображение, уменьшенное в n раз
IplImage* gauss = NULL;				// дефокусированное изображение
IplImage* canny = NULL;				// изображение границ между объектами
IplImage* nabla1 = NULL;			// градиент gray_resized
IplImage* nabla2 = NULL;			// градиент gauss
IplImage* sigma = NULL;				// оценка пятна размытия для каждой точки границы
IplImage* fsigma = NULL;
IplImage* depth = NULL;				// нормализованная разреженная карта глубины
	// todo: проверка входных параметров
	//--- . . . . 
	// подготовка изображений для преобразования 
	 CvSize s; s.height = src->height / resize_f; s.width = src->width / resize_f;
	 gray = cvCreateImage(cvSize(src->width,src->height), IPL_DEPTH_8U, 1);
	 gray_resized = cvCreateImage(s, IPL_DEPTH_8U, 1);
	 gauss = cvCreateImage(s, IPL_DEPTH_8U,1);
	 canny = cvCreateImage(s, IPL_DEPTH_8U,1);
	 nabla1 = cvCreateImage(s, IPL_DEPTH_32F,1);
	 nabla2 = cvCreateImage(s, IPL_DEPTH_32F,1);
	 sigma = cvCreateImage(s, IPL_DEPTH_32F, 1);
	 fsigma = cvCreateImage(s, IPL_DEPTH_32F, 1);
	 depth = cvCreateImage(s, IPL_DEPTH_8U, 1);
 	 // преобразование исходного изображения в полутоновое 
     cvCvtColor(src, gray, CV_BGR2GRAY);
	 // уменьшение изображения 
  	 cvResize(gray, gray_resized);
	 // получение дефокусированного изображения
	 //cvSmooth(gray_resized, gauss, CV_GAUSSIAN, 3,3);
	 GaussFilter(gray_resized, gauss);
	 // получение маски canny
	 cvCanny(gray_resized, canny, canny_thr, canny_thr*2,3);
	 // получиение градиентов
	 MakeMaskGradient(gray_resized, canny, nabla1);
	 MakeMaskGradient(gauss, canny, nabla2);

	 // получение оценки расстояния
	 GetSigmaFromGradients(nabla1, nabla2, sigma);
	 //JointBilateralFilter(sigma, gray_resized, fsigma);
	 // нормировка расстояний
	 NormSigma(sigma, depth);
	 cvResize(depth, depth_map);
#ifdef _DEBUG
	 cvSaveImage("debug_sparse.png",depth_map);
#endif 
	 // освобождение памяти
	 cvReleaseImage(&gray);
	 cvReleaseImage(&gray_resized);
	 cvReleaseImage(&gauss);
	 cvReleaseImage(&canny);   
	 cvReleaseImage(&nabla1);
	 cvReleaseImage(&nabla2);
	 cvReleaseImage(&sigma);
	 cvReleaseImage(&fsigma);
	 cvReleaseImage(&depth);
}
