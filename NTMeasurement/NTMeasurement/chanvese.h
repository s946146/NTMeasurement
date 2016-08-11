#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>

#define PI 3.14159265358979323846
#define RED 0
#define GREEN 1
#define BLUE 2
#define DIVIDE_EPS ((double)1e-16)

class chanvese
{
public:
    chanvese();
    chanvese(int* ImageMap, int w, int h, int* _seed);

    double *Phi, *PrevPhi;
    double c1, c2;
    int *grayImage, *Image, *seed;
    int Width, Height;
    double Mu, Nu, LambdaA, LambdaB, PhiTol, dt;
    int MaxIter;

    double* ChanVeseProcedure();
    void ChanVeseInitPhi();
    void GetChanVeseCofficient(double *Phi, int x, int y, double &F1, double &F2, double &F3, double &F4, double &F, double &Delta);
    void RegionAverage(double &_c1, double &_c2, double *_phi, int *_grayImage);
    int *ChanVesePlot();
    void setMu(int _Mu);
    void setNu(int _Nu);
    void setLambdaA(int _LambdaA);
    void setLambdaB(int _LambdaB);
};
