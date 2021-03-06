//*************************************************************************************************************************
//1. 撰寫人:蔡乙至
//2. 撰寫日期:2008/06/04
//3. 撰寫目的，功能:  Forward and Inverse Kinematics
//4. 類別:
//5. 函數:
//6. 其他:
//************************************************************************************************************************
#pragma once
#include "DH_matrix.h"
#include <fstream>
#include <string>

class ArmKinematic
{
public:
	const static int  JDim = 7;
	const static int Cpara = 2; // claw parameters
	const static int Tpara = 9;//Total parameters

protected:
	float CurrentPos[3];
	float d[Tpara];
	float a[Tpara];
	float theta[Tpara];
	float alpha[Tpara];
	float theta_home[Tpara];
	float offset[4];//parameter: d a alpha theta

public:
	//vector<RangeLimit> Jointlimit;
	//-----------DH-Parameter------------------------
	DHmodel *DHarm;

	Matrix *R_copy;

	ArmKinematic(char* file);

	ArmKinematic(float POriginPos[], float Pd[], float Pa[], float Palpha[], float Ptheta_home[], float Ptheta[], float Lowlimit[], float Uplimit[], float EEoffset[]);

	void ResetTheta(float Postion[] ,float Ptheta[]);

	void ResetTheta(float Ptheta[]);

	void GetTheta(float *Ptheta);

	void ClawTheta(float Ptheta);

	~ArmKinematic();
};