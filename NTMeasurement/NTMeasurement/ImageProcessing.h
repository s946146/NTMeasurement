#pragma once

#include "cstdlib"
#include "iostream"
#include "algorithm"
#include "vector"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace System::IO;	
using namespace System::Drawing;
using namespace cv;

class ImageProcessing
{
public:
	ImageProcessing();

	//Preprocessing----------------------------------------------------------------
	//Median Filter + Anisotropic Diffusion + Gamma Correction
	Bitmap ^ Preprocessing(Bitmap ^ _ImgBMP);
	Bitmap ^ Closing(Bitmap ^ _ImgBMP);
	Bitmap ^HistogramEqualization(Bitmap ^ _Img);

	//Median Filter
	Bitmap ^ Median_Filter(Bitmap ^ _ImgBMP);
	
	//2-D Anisotropic Diffusion，傳入：要處理的影像、iteration(5)、edge magnitude(30)
	Bitmap ^ AnisotropicDiffusion(Bitmap ^ _ImgBMP, int _iter, float _k);
	
	//Gamma Correction
	Bitmap ^ GammaCorrection(Bitmap ^ _ImgBMP, double gamma);

	Bitmap ^ Dilation(Bitmap ^ _ImgBMP);
	Bitmap ^ Erosion(Bitmap ^ _ImgBMP);

	//Segmentation------------------------------------------------------------------
	//Sobel
	Bitmap ^ Sobel(Bitmap ^ _Img, int mode);
	
	//Grandient
	Bitmap ^ Gradient(Bitmap ^ _Img, int mode);
	
	//Laplacian
	Bitmap ^ Laplacian(Bitmap ^ _Img);
	
	//Find Start and End point
	Bitmap ^ InitialSegmentation(Bitmap ^ _Img);

	Bitmap ^ ShortestPath(int A, int B, Bitmap ^_gradient, Bitmap ^ _Img, Color color);
	
	//Graph cut
	Mat BitmapToMat(Bitmap^ bmp);
	Bitmap ^ GraphCut(Bitmap^ _Img, Bitmap^ _InitialSeg);

};