
#include "stdafx.h"
#include "ImageProcessing.h"

using namespace System::Drawing;

/* Preprocessing */
// Median Filter + Anisotropic Diffusion + Gamma Correction
Bitmap ^ ImageProcessing::Preprocessing(Bitmap ^ _ImgBMP){

	Bitmap ^ temp = gcnew Bitmap(_ImgBMP->Width, _ImgBMP->Height);
	temp = Median_Filter(_ImgBMP);
	temp = AnisotropicDiffusion(temp, 5, 30);
	temp = GammaCorrection(temp, 2);

	//closing
	//temp = Dilation(temp);
	//temp = Erosion(temp);

	return temp;
}

Bitmap ^ ImageProcessing::Closing(Bitmap ^ _Img){

	Bitmap ^ temp = gcnew Bitmap(_Img->Width, _Img->Height);
	temp = Dilation(_Img);
	temp = Erosion(temp);

	return temp;
}

/* Median Filter*/
Bitmap ^ImageProcessing::Median_Filter(Bitmap ^ _ImgBMP){

	Bitmap ^ temp = gcnew Bitmap(_ImgBMP->Width, _ImgBMP->Height);
	int idx;
	int Array[9];

	for(int j = 0 ; j < _ImgBMP->Height ; j++){
		for(int i = 0 ; i < _ImgBMP->Width ; i++){
			
			idx = 0;		
			
			for(int p = i-1; p <= i+1; p++){		// Window  3*3
				for(int q = j-1; q <= j+1; q++){
					if (p >= 0 && q >= 0 && p < _ImgBMP->Width && q < _ImgBMP->Height){
						Array[idx] = _ImgBMP->GetPixel(p, q).R;
						idx++;
					}
				}//q
			}//p
			
			sort(Array, Array+(idx-1));
			
			if(idx%2==0)	// Window ず逼翴计琌案计
                temp->SetPixel(i, j, Color::FromArgb((Array[idx/2-1]+Array[idx/2])/2, (Array[idx/2-1]+Array[idx/2])/2, (Array[idx/2-1]+Array[idx/2])/2));
            else		// Window ず逼翴计琌计
                temp->SetPixel(i, j, Color::FromArgb(Array[idx/2], Array[idx/2], Array[idx/2]));
		}//i
	}//j
	//temp->Save("PreMedianImg.bmp");
	return temp;
}

/* 2-D Anisotropic Diffusion*/
// Input璶矪瞶皚iterationedge magnitude 
// Return矪瞶Ч皚
Bitmap ^ ImageProcessing::AnisotropicDiffusion(Bitmap ^ _ImgBMP, int _iter, float _k){	
	int _w = _ImgBMP->Width;
	int _h = _ImgBMP->Height;
	
	float ** tmpPix1 = new float * [_w];
	float ** tmpPix2 = new float * [_w];
	float ** tmpPix3 = new float * [_w];
	float ** zetaPix = new float * [_w];
	for(int i=0 ; i<_w ; i++){
		tmpPix1[i] = new float [_h];
		tmpPix2[i] = new float [_h];
		tmpPix3[i] = new float [_h];
		zetaPix[i] = new float [_h];
	}
	float * NeiGradient = new float [4];
	float * NeiValue = new float [4];
	float * AvgGradient = new float [4];

	// initial
	for (int j = 0 ; j < _h ; j++){
		for (int i = 0 ; i < _w ; i++){
			tmpPix1[i][j] = _ImgBMP->GetPixel(i, j).R;
			tmpPix2[i][j] = _ImgBMP->GetPixel(i, j).R;
			tmpPix3[i][j] = _ImgBMP->GetPixel(i, j).R;
		}//i
	}//j

	for (int it = 0 ; it < _iter ; it++){
		for (int j = 1 ; j < _h-1 ; j++){
			for (int i = 1 ; i < _w-1 ; i++){
				
				// tmp1 => – iteration ﹍篈 ----------------
				NeiGradient[0] = tmpPix1[i + 1][j] - tmpPix1[i][j];
				NeiGradient[1] = tmpPix1[i - 1][j] - tmpPix1[i][j];
				NeiGradient[2] = tmpPix1[i][j + 1] - tmpPix1[i][j];
				NeiGradient[3] = tmpPix1[i][j - 1] - tmpPix1[i][j];
				
				// tmp2 => avgerage --------------------------------
				tmpPix2[i][j] = 0.0;
				double Mean = 0;
				for (int m = -1; m < 2; m++){
					for (int n = -1; n < 2; n++){
						Mean += (double)tmpPix1[i - n][j - m];
					}//n
				}//m
				Mean /= (double)9;
				tmpPix2[i][j] = Mean;

				// zeta => local std -------------------------------
				zetaPix[i][j] = 0.0;
				for (int m = -1; m < 2; m++){
					for (int n = -1; n < 2; n++){
						zetaPix[i][j] = zetaPix[i][j] + pow( (tmpPix1[i-n][j-m] - tmpPix2[i][j]), 2 );
					}//n
				}//m
				zetaPix[i][j] = sqrt (zetaPix[i][j]/(double)9);
				
				// temp3 --------------------------------------------
				tmpPix3[i][j] = 0.0;
				AvgGradient[0] = tmpPix2[i + 1][j] - tmpPix2[i][j];
				AvgGradient[1] = tmpPix2[i - 1][j] - tmpPix2[i][j];
				AvgGradient[2] = tmpPix2[i][j + 1] - tmpPix2[i][j];
				AvgGradient[3] = tmpPix2[i][j - 1] - tmpPix2[i][j];
	
				double mu = 0.0;
				for (int n = 0; n < 4; n++){
					NeiValue[n] = exp( -pow( (zetaPix[i][j] * AvgGradient[n] / _k), 2 ) ) * NeiGradient[n];
					mu += exp( -pow( (zetaPix[i][j] * AvgGradient[n] / _k), 2 ) );
					tmpPix3[i][j] += NeiValue[n];
				}
					
				if(mu < 0.001)
					mu = 0.01;
				tmpPix3[i][j] *= ( (double )1.0 / mu );
				tmpPix3[i][j] += tmpPix1[i][j];
					
				if(tmpPix3[i][j] < 0)	
					tmpPix3[i][j] = 0;
				if(tmpPix3[i][j] > 255)	
					tmpPix3[i][j] = 255;
			}//i
		}//j

		// copy ------------------------------------------
		for (int j = 0 ; j < _h ; j++)
			for (int i = 0 ; i < _w ; i++)
				tmpPix1[i][j] = tmpPix3[i][j];
	}//iter

	// return --------------------------------------------
	Bitmap ^ temp = gcnew Bitmap(_ImgBMP->Width, _ImgBMP->Height);
	for (int j = 0 ; j < _h ; j++)
		for (int i = 0 ; i < _w ; i++)
			temp->SetPixel(i, j, Color::FromArgb((int)tmpPix3[i][j], (int)tmpPix3[i][j], (int)tmpPix3[i][j]));

	// free memory
	for(int i=0 ; i<_w ; i++)	
		delete [] tmpPix1[i];
	delete [] tmpPix1;
	
	for(int i=0 ; i<_w ; i++)	
		delete [] tmpPix2[i];
	delete [] tmpPix2;
	
	for(int i=0 ; i<_w ; i++)	
		delete [] tmpPix3[i];
	delete [] tmpPix3;
	
	for(int i=0 ; i<_w ; i++)	
		delete [] zetaPix[i];
	delete [] zetaPix;
	
	free(NeiGradient);
	NeiGradient = NULL;
	free(NeiValue);		
	NeiValue = NULL;
	free(AvgGradient);	
	AvgGradient = NULL;

	return temp;
}

/* Gamma Correction */
Bitmap ^ ImageProcessing::GammaCorrection(Bitmap ^ _ImgBMP, double gamma){

	int ** tmp = new int * [_ImgBMP->Width];
	for(int i=0 ; i<_ImgBMP->Width ; i++)
		tmp[i] = new int [_ImgBMP->Height];

	double *table = new double [256];

	// 狡籹皚
	for(int j=0 ; j < _ImgBMP->Height ; j++)
		for(int i=0 ; i < _ImgBMP->Width ; i++)
			tmp[i][j] = _ImgBMP->GetPixel(i, j).R;
		
	
	// 锣传 Table 
	for (int t=0 ; t<256 ; t++){
		table[t] = 255* pow ((double)t/(double)255, gamma);
		if( table[t] >255)
			table[t]=255;
	}

	// Gamma Correction
	Bitmap ^ temp = gcnew Bitmap(_ImgBMP->Width, _ImgBMP->Height);
	for(int j=0 ; j < _ImgBMP->Height ; j++){
		for(int i=0 ; i < _ImgBMP->Width ; i++) {
			int tmpIntensity = tmp[i][j];
			int value = (int)table[tmpIntensity];
			temp->SetPixel(i, j, Color::FromArgb(value, value, value));
		}
	}

	// free memory
	for(int i=0 ; i<_ImgBMP->Width ; i++)
		delete [] tmp[i];
	delete [] tmp;
	free(table); table = NULL;

	return temp;
}

//Sobel
Bitmap ^ ImageProcessing::Sobel(Bitmap ^ _Img, int mode){
	int w = _Img->Width;
	int h = _Img->Height;
	Bitmap ^ temp = gcnew Bitmap(w, h);

	float Gx, Gy, G;
	int *MaskH = NULL;
	MaskH = (int *)malloc(3*3*sizeof(int));

	int *MaskV = NULL;
	MaskV = (int *)malloc(3*3*sizeof(int));

	MaskH[0*3+0] = -1; MaskH[1*3+0] = -2; MaskH[2*3+0] = -1;
	MaskH[0*3+1] =  0; MaskH[1*3+1] =  0; MaskH[2*3+1] =  0;
	MaskH[0*3+2] =  1; MaskH[1*3+2] =  2; MaskH[2*3+2] =  1;

	MaskV[0*3+0] = -1; MaskV[1*3+0] =  0; MaskV[2*3+0] = 1;
	MaskV[0*3+1] = -2; MaskV[1*3+1] =  0; MaskV[2*3+1] = 2;
	MaskV[0*3+2] = -1; MaskV[1*3+2] =  0; MaskV[2*3+2] = 1;

	int I = 0;
	Color tmpColor;
	for (int y=0 ; y<h ; y++) {
		for (int x=0 ; x<w ; x++)
		{
			Gx = Gy = 0;
			//convolution
			for (int i=-1 ; i<2 ; i++) 
				for (int j=-1 ; j<2 ; j++)
				{
					//耞琌禬娩
					if (y+i >= 0 && y+i < h && x+j >= 0 && x+j < w)
						I = _Img->GetPixel(x+j, y+i).R; 

					Gx += (I * MaskH[(j+1)*3 + (i+1)]);
					Gy += (I * MaskV[(j+1)*3 + (i+1)]); 
				}

			switch(mode)
			{
			case 0: G = abs(Gx);	break;
			case 1: G = abs(Gy);	break;	
			case 2: G = sqrt( pow(Gx, 2) + pow(Gy, 2) );	break;
			}
			
			if(G < 0)	G = 0;
			if(G > 255)	G = 255;
			
			tmpColor = System::Drawing::Color::FromArgb(G, G, G);
			temp->SetPixel(x, y, tmpColor);
		}
	}

	//free memory
	free(MaskH);
	free(MaskV);	

	return temp;
}

//Laplacian
Bitmap ^ ImageProcessing::Laplacian(Bitmap ^ _Img){
	
	int w, h;
	w = _Img->Width;
	h = _Img->Height;
	Bitmap ^ temp = gcnew Bitmap(w, h);
	
	int *Mask = NULL;
	Mask = (int *)malloc(3*3*sizeof(int));

	Mask[0*3+0] = -1; Mask[1*3+0] = -1; Mask[2*3+0] = -1;
	Mask[0*3+1] = -1; Mask[1*3+1] =  8; Mask[2*3+1] =  8;
	Mask[0*3+2] = -1; Mask[1*3+2] = -1; Mask[2*3+2] = -1;

	int I = 0;
	float G;
	Color tmpColor;
	for (int y=0 ; y<h ; y++) {
		for (int x=0 ; x<w ; x++){
			
			G = 0;
			for (int i=-1 ; i<2 ; i++) 
				for (int j=-1 ; j<2 ; j++)
				{
					//耞琌禬娩
					I = 0;
					if (y+i >= 0 && y+i < h && x+j >= 0 && x+j < w)
						I = _Img->GetPixel(x+j, y+i).R; 

					G += (I * Mask[(j+1)*3 + (i+1)]);
				}

			if(G < 0)	G = 0;
			if(G > 255)	G = 255;

			tmpColor =Color::FromArgb(G, G, G);
			temp->SetPixel(x, y, tmpColor);
		}
	}

	free(Mask);

	return temp;
}

//Gradient
Bitmap ^ ImageProcessing::Gradient(Bitmap ^ _Img, int mode){
	int w = _Img->Width;
	int h = _Img->Height;
	Bitmap ^ temp = gcnew Bitmap(w, h);

	float G;

	int I1, I2;
	Color tmpColor;
	for (int y=1 ; y<h-1 ; y++) {
		for (int x=0 ; x<w ; x++)
		{

			if( mode == 0 ){
				I1 = _Img->GetPixel(x, y).R;
				I2 = _Img->GetPixel(x, y-1).R;

			}else if( mode == 1 ){
				I1 = _Img->GetPixel(x, y).R;
				I2 = _Img->GetPixel(x, y+1).R;
			}
			G =  I2-I1;

			if(G < 0)	G = 0;
			if(G > 255)	G = 255;
			
			tmpColor = Color::FromArgb(G, G, G);
			temp->SetPixel(x, y, tmpColor);
		}
	}

	/*for (int y=0 ; y<h ; y++){
		
		I1 = temp->GetPixel(w-1, y).R;
		
		cout << I1 << endl;
	}*/

	return temp;
}

Bitmap ^ ImageProcessing::InitialSegmentation(Bitmap ^ _Img){
	int w = _Img->Width;
	int h = _Img->Height;
	Bitmap ^ result = gcnew Bitmap(_Img);
	bool isFind = false;

	//upper edge
	Bitmap ^ gradient = gcnew Bitmap(w, h);
	gradient = Gradient(_Img, 0);

	//find start point of upper edge 
	vector<int> arr;
	for (int y=0 ; y<h ; y++)
		arr.push_back(gradient->GetPixel(0, y).R);
	sort(arr.begin(), arr.end());

	int upperStart, upperEnd, lowerStart, lowerEnd;
	int I1, I2, I3;
	for(int y=1 ; y<h-1 ; y++){
		
		I1 = gradient->GetPixel(0, y-1).R;
		I2 = gradient->GetPixel(0, y).R;
		I3 = gradient->GetPixel(0, y+1).R;

		if(I2-I1>0 && I2-I3>0 && I2>=arr.at(h*0.8)){
			isFind = true;
			gradient->SetPixel(0, y, Color::FromArgb(255, 0, 0));
			upperStart = y;
			break;
		}
	}

	if(!isFind){
		printf("Start Point of upper edge isn't found.\n");

		gradient->SetPixel(0, h/2, Color::FromArgb(255, 0, 0));
		upperStart = h/2;
	}

	//find end point of upper edge
	isFind = false;

	arr.clear();
	for (int y=0 ; y<h ; y++)
		arr.push_back(gradient->GetPixel(w-1, y).R);
	sort(arr.begin(), arr.end());
	
	for(int y=1 ; y<h-1 ; y++){
		
		I1 = gradient->GetPixel(w-1, y-1).R;
		I2 = gradient->GetPixel(w-1, y).R;
		I3 = gradient->GetPixel(w-1, y+1).R;

		if(I2-I1>0 && I2-I3>0 && I2>=arr.at(h*0.8)){
			isFind = true;
			gradient->SetPixel(w-1, y, Color::FromArgb(255, 0, 0));
			upperEnd = y;
			break;
		}
	}

	if(!isFind){
		printf("End Point of upper edge isn't found.\n");

		gradient->SetPixel(w-1, h/2, Color::FromArgb(255, 0, 0));
		upperEnd = h/2;
	}

	result = ShortestPath(upperStart, upperEnd, gradient, result, Color::FromArgb(255, 0, 0));

	//lower edge
	gradient = Gradient(_Img, 1);

	//find start point of lower point
	isFind = false;

	arr.clear();
	for (int y=0 ; y<h ; y++)
		arr.push_back(gradient->GetPixel(0, y).R);
	sort(arr.begin(), arr.end());

	for(int y=h-2 ; y>1 ; y--){
		
		I1 = gradient->GetPixel(0, y+1).R;
		I2 = gradient->GetPixel(0, y).R;
		I3 = gradient->GetPixel(0, y-1).R;

		if(I2-I1>0 && I2-I3>0 && I2>=arr.at(h*0.8)){
			isFind = true;
			gradient->SetPixel(0, y, Color::FromArgb(0, 255, 0));
			lowerStart = y;
			break;
		}
	}

	if(!isFind){
		printf("Start Point of lower edge isn't found.\n");

		gradient->SetPixel(0, h/2, Color::FromArgb(255, 0, 0));
		lowerStart = h/2;
	}

	//find end point of lower edge
	arr.clear();
	for (int y=0 ; y<h ; y++)
		arr.push_back(gradient->GetPixel(w-1, y).R);
	sort(arr.begin(), arr.end());

	for(int y=h-2 ; y>1 ; y--){
		
		I1 = gradient->GetPixel(w-1, y+1).R;
		I2 = gradient->GetPixel(w-1, y).R;
		I3 = gradient->GetPixel(w-1, y-1).R;

		if(I2-I1>0 && I2-I3>0 && I2>=arr.at(h*0.8)){
			isFind = true;
			gradient->SetPixel(w-1, y, Color::FromArgb(0, 255, 0));
			lowerEnd = y;
			break;
		}
	}

	if(!isFind){
		printf("End Point of lower edge isn't found.\n");

		gradient->SetPixel(w-1, h/2, Color::FromArgb(255, 0, 0));
		lowerEnd = h/2;
	}

	result = ShortestPath(lowerStart, lowerEnd, gradient, result, Color::FromArgb(0, 255, 0));

	return result;
}

//according to gradient, finds shortest path from A to B
Bitmap ^ ImageProcessing::ShortestPath(int A, int B, Bitmap ^ _gradient, Bitmap ^ _Img, Color color) {
	
	int w, h; 
	w = _gradient->Width;
	h = _gradient->Height;

	_gradient->SetPixel(w-1, B, Color::FromArgb(255, 255, 255));

	int startPoint = A, max, nextPoint, I;
	for(int x=0; x<w; x++){
		max = 0;
		for(int y=startPoint-1; y<startPoint+2; y++){
			if(y > 0 && y < h){
				I = _gradient->GetPixel(x, y).R;
				if(I > max){
					max = I;
					nextPoint = y;
				}
			}
		}
		_Img->SetPixel(x, nextPoint, color);
	}
	
	return _Img;
}

//Graph Cut
Mat ImageProcessing::BitmapToMat(Bitmap^ bmp)
{
	int w, h;
	w = bmp->Width;
	h = bmp->Height;

    Mat Img;
	Img.create(h, w, CV_8UC3);

	Color color;
	for(int y=0; y<Img.rows; y++){
		for(int x=0; x<Img.cols; x++){
			color = bmp->GetPixel(x, y);
			
			Img.at<Vec3b>(y, x)[0] = color.B;
			Img.at<Vec3b>(y, x)[1] = color.G;
			Img.at<Vec3b>(y, x)[2] = color.R;
		}
	}

    return Img;
}

Bitmap ^ ImageProcessing::GraphCut(Bitmap^ _Img, Bitmap^ _InitialSeg){
	
	int w, h;
	w = _Img->Width;
	h = _Img->Height;

	Mat Img;
	Img = BitmapToMat(_Img);

	Mat mask;
	mask.create(Img.size(), CV_8UC1);
    
	int R, G, B;
	for(int y=0; y<mask.rows; y++){
		for(int x=0; x<mask.cols; x++){
			R = _InitialSeg->GetPixel(x, y).R;
			G = _InitialSeg->GetPixel(x, y).G;
			B = _InitialSeg->GetPixel(x, y).B;
			
			if( R == 255 && G == 0 && B == 0 ){
				mask.at<uchar>(y, x) = GC_FGD;
			}		
			else if( R == 0 && G == 0 && B == 0)
				mask.at<uchar>(y, x) = GC_BGD;
			else
				mask.at<uchar>(y, x) = GC_PR_FGD;
		}
	}
	
	Mat bgdModel, fgdModel;

	Rect rect;
	/*rect.x = 0;
	rect.y = 0;
	rect.width = w;
	rect.height = h;*/
	
	grabCut( Img, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK );

	Bitmap ^ result = gcnew Bitmap(w, h);
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			if( mask.at<uchar>(y, x) & 1 ){
				result->SetPixel(x, y, Color::FromArgb(255, 255, 255));
			}else{
				result->SetPixel(x, y, Color::FromArgb(0, 0, 0));
			}
		}
	}

	return result;
}

Bitmap ^ImageProcessing::Dilation(Bitmap ^ _ImgBMP){

	Bitmap ^ temp = gcnew Bitmap(_ImgBMP->Width, _ImgBMP->Height);
	int max;
	int arr[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};

	for(int y = 1 ; y < _ImgBMP->Height-1 ; y++){
		for(int x = 1 ; x < _ImgBMP->Width-1 ; x++){
			
			int max = -9999;
			for(int j = -1; j<= 1; j++){
				for(int i = -1; i <= 1; i++){
					if(arr[i][j] == 1){
						int value = _ImgBMP->GetPixel(x+i, y+j).R;
						if(value > max)
							max = value;
					}
				}
			}
			temp->SetPixel(x, y, Color::FromArgb(max, max, max));
		}
	}
			
	return temp;
}

Bitmap ^ImageProcessing::Erosion(Bitmap ^ _ImgBMP){

	Bitmap ^ temp = gcnew Bitmap(_ImgBMP->Width, _ImgBMP->Height);
	int min;
	int arr[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};

	for(int y = 1 ; y < _ImgBMP->Height-1 ; y++){
		for(int x = 1 ; x < _ImgBMP->Width-1 ; x++){
			
			int min = 9999;
			for(int j = -1; j<= 1; j++){
				for(int i = -1; i <= 1; i++){
					if(arr[i][j] == 1){
						int value = _ImgBMP->GetPixel(x+i, y+j).R;
						if(value < min)
							min = value;
					}
				}
			}
			temp->SetPixel(x, y, Color::FromArgb(min, min, min));
		}
	}
			
	return temp;
}

Bitmap ^ImageProcessing::HistogramEqualization(Bitmap ^ _Img){
	
    Bitmap ^ temp = gcnew Bitmap(_Img->Width, _Img->Height);

    int histogram[256];
    int lookuptable[256];
    for(int i=0; i<256; i++)
    {
        histogram[i] = 0;
        lookuptable[i] = 0;
    }

    for(int y=0; y<_Img->Height; y++)
    {
        for(int x=0; x<_Img->Width; x++)
        {
            int gray = _Img->GetPixel(x, y).R;
            histogram[gray]++;
        }
    }

    int sum = 0;
    for (int i = 0; i < 256; i++) 
    {
        sum += histogram[i];
		lookuptable[i] = 255 * sum / (_Img->Width * _Img->Height);
    }

	for (int y = 0; y < _Img->Height; y++)
    {
        for (int x = 0; x < _Img->Width; x++)
        {         
            int gray = _Img->GetPixel(x, y).R;
            gray = lookuptable[gray];

            temp->SetPixel(x, y, Color::FromArgb(gray, gray, gray));
        }
    }

	return temp;
}
