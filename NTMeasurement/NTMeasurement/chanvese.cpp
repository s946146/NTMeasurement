#include "stdafx.h"
#include "chanvese.h"

chanvese::chanvese()
{
}

chanvese::chanvese(int* ImageMap, int w, int h, int* _seed){

    Width = w;
    Height = h;

    grayImage = (int*) malloc(Width*Height*sizeof(int));
    memcpy(grayImage, ImageMap, Width*Height*sizeof(int));

	seed = (int*) malloc(Width*Height*sizeof(int));
    memcpy(seed, _seed, Width*Height*sizeof(int));

    Mu = 0.2;
    Nu = 0.0;
    LambdaA = 1.0;
    LambdaB = 1.0;
    MaxIter = 2;
    PhiTol = (double)0.1;
    c1 = 0;
    c2 =0;
    dt = 0.5;

}

double* chanvese::ChanVeseProcedure(){

    double Delta, F1, F2, F3, F4, F;

    ChanVeseInitPhi();
    RegionAverage(c1, c2, Phi, grayImage);

    PrevPhi = (double*) malloc(Width*Height*sizeof(double));

    for(int i=0; i<MaxIter; i++){

        memcpy(PrevPhi, Phi, Width*Height*sizeof(double));

        for(int y=1; y<Height-1; y++){

            for(int x=1; x<Width-1; x++){
                GetChanVeseCofficient(Phi, x, y, F1, F2, F3, F4, F, Delta);

                double P_xy = Phi[x*Height+y] - dt*Delta*(Nu + LambdaA*pow((grayImage[x*Height+y]-c1),2) - LambdaB*pow((grayImage[x*Height+y]-c2),2));
                // Update level set function
                Phi[x*Height+y] = F1*Phi[(x+1)*Height+y]+F2*Phi[(x-1)*Height+y]+F3*Phi[x*Height+y+1]+F4*Phi[x*Height+y-1]+F*P_xy;
            }
        }

        // Update border values of phi
        for (int x = 0; x <Width; x++){
            Phi[x*Height] = Phi[x*Height+1];
            Phi[x*Height+(Height-1)] = Phi[x*Height+(Height-2)];
        }
        for (int y = 0; y <Height; y++){
            Phi[y] = Phi[Height+y];
            Phi[(Width-1)*Height+y] = Phi[(Width-2)*Height+y];
        }

        //Check stopping condition
        double diff, diffNorm = 0.0;
        for(int y=0; y<Height; y++){
            for(int x=0; x<Width; x++){
                if(Phi[x*Height+y] >= 0){
                    diff = Phi[x*Height+y]-PrevPhi[x*Height+y];
                    diffNorm += diff*diff;
                }
            }
        }
        diffNorm = sqrt(diffNorm/(double)(Height*Width));

        if(i >= 2 && diffNorm <= PhiTol)
            break;
    }

	return Phi;
}

void chanvese::GetChanVeseCofficient(double *phi, int x, int y, double &F1, double &F2, double &F3, double &F4, double &F, double &delta){

    double C1 = 1/sqrt(DIVIDE_EPS+pow((phi[(x+1)*Height+y]-Phi[x*Height+y]),2)+pow((phi[x*Height+y+1]-phi[x*Height+y-1]),2)/4.0);
    double C2 = 1/sqrt(DIVIDE_EPS+pow((phi[x*Height+y]-Phi[(x-1)*Height+y]),2)+pow((phi[(x-1)*Height+y+1]-phi[(x-1)*Height+y-1]),2)/4.0);
    double C3 = 1/sqrt(DIVIDE_EPS+pow((phi[(x+1)*Height+y]-Phi[(x-1)*Height+y]),2)/4.0+pow((phi[x*Height+y]-phi[(x+1)*Height+y]),2));//1/sqrt(DIVIDE_EPS+pow((phi[(x+1)*Height+y]-Phi[(x-1)*Height+y]),2)/4.0+pow((phi[x*Height+y+1]-phi[x*Height+y]),2));
    double C4 = 1/sqrt(DIVIDE_EPS+pow((phi[(x+1)*Height+y-1]-Phi[(x-1)*Height+y-1]),2)/4.0+pow((phi[x*Height+y-1]-phi[(x+1)*Height+y-1]),2));

    double H = 1.0;
    delta = H/(PI*(H*H+phi[x*Height+y]*phi[x*Height+y]));

    double Factor = dt*delta*Mu;
    F = 1/(1+Factor*(C1+C2+C3+C4));
    Factor = Factor*F;

    F1 = Factor*C1;
    F2 = Factor*C2;
    F3 = Factor*C3;
    F4 = Factor*C4;
}

void chanvese::ChanVeseInitPhi(){

    Phi = (double*) malloc(Width*Height*sizeof(double));
    for(int y=0; y<Height; y++){
        for(int x=0; x<Width; x++){ 
			Phi[x*Height+y] = seed[x*Height+y];
            /*if(seed[x*Height+y] == 1)
				Phi[x*Height+y] = 0;
			else 
				Phi[x*Height+y] = 1;*/
			//Phi[x*Height+y] = 5.0/(5.0 + (x-Width/2)*(x-Width/2) + (y-Height/2)*(y-Height/2)) - 0.5;
			//Phi[x*Height+y] = (sin(PI/5*x))*(sin(PI/5*y));
        }
    }

}

void chanvese::RegionAverage(double &_c1, double &_c2, double *_phi,int *_grayImage){

    int sum1 = 0, sum2 = 0;
    int pixel1 = 0, pixel2 = 0;
    for(int y=0; y<Height; y++){
        for(int x=0; x<Width; x++){
            if(_phi[x*Height+y] >= 0){
                pixel1++;
                sum1+=_grayImage[x*Height+y];
            }
            else{
                pixel2++;
                sum2+=_grayImage[x*Height+y];
            }
        }
    }
    _c1 = (double)sum1/pixel1;
    _c2 = (double)sum2/pixel2;
}

int* chanvese::ChanVesePlot(){

    Image = (int*) malloc(Width*Height*3*sizeof(int));

    for(int y=0; y<Height; y++){
        for(int x=0; x<Width; x++){
            if(Phi[x*Height+y] < 0){
                Image[Width*Height*RED+x*Height+y] = 255;
                Image[Width*Height*GREEN+x*Height+y] = 0;
                Image[Width*Height*BLUE+x*Height+y] = 0;
            }else{
                Image[Width*Height*RED+x*Height+y] = grayImage[x*Height+y];
                Image[Width*Height*GREEN+x*Height+y] = grayImage[x*Height+y];
                Image[Width*Height*BLUE+x*Height+y] = grayImage[x*Height+y];
            }
        }
    }
    return Image;
}

void chanvese::setMu(int _Mu){
    Mu = _Mu;
}

void chanvese::setNu(int _Nu){
    Nu = _Nu;
}

void chanvese::setLambdaA(int _LambdaA){
    LambdaA = _LambdaA;
}

void chanvese::setLambdaB(int _LambdaB){
    LambdaB = _LambdaB;
}
