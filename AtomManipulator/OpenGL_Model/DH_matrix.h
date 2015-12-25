//*************************************************************************************************************************
//1. 级糶:讲
//2. 级糶ら戳:2008/06/04
//3. 级糶ヘ:  Forward and Inverse Kinematics
//4. 摸:
//5. ㄧ计:
//6. ㄤ:
//************************************************************************************************************************
#pragma once
#include <iostream>
#include "newmat10\include.h"
#include "newmat10\newmat.h"
#include "newmat10\newmatio.h"
#include "newmat10\newmatap.h"
#include <vector>
//#include "BasicRRT.h"


using namespace std;

//typedef struct RangeLimit
//{
//	float lower_limit;
//	float upper_limit;
//}RangeLimit;


class DHmatrix
{

protected:
	float d;
	float a;
	float alpha;
	float theta_home;
	float theta;
	Matrix A;
	
public:
	DHmatrix();
	DHmatrix(float Pd,float Pa,float Palpha,float Ptheta_home,float Ptheta);
	Matrix T();
	Matrix T(float Pd,float Pa,float Palpha,float Ptheta_home,float Ptheta);
};


class DHmodel
{

protected:
	float *Pd;
	float *Pa;
	float *Palpha;
	float *Ptheta_home;
	float *Ptheta;
	float OriginPos[3];
	float offset[4];
	int Dim;
	int Para;
	int Scale_IK;

private:
	DHmatrix Rtemp;
	Matrix zero_pointer;
	Matrix R_Pos2world;


public:
	Matrix *R;
	Matrix *position;

	DHmodel(int JDim, int Tpara, float origin_Pos[], float d[], float a[], float alpha[], float theta_home[], float theta[] , float EEoffset[]);

	void ReSetParameter(float Position[] , float d[], float a[], float alpha[], float theta_home[], float theta[]);

	//-----------Inverse kinematics-------------------
public:
	float EECoor[6]; //E.E. Coordinate

	Matrix Cross(Matrix a, Matrix b);

	void GetJacobian(Matrix& J);

	//std::vector<RangeLimit> m_JointLimit;
	
	void RotM2Euler(float m[][3] , float Euler[]);

	void IK(float goal[], int goal_N,float m_theta[]);
	bool Check_IK(float goal[], int goal_N, float m_theta[]);

	float prevX, prevY, prevZ;

	~DHmodel();
};

