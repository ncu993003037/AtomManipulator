#include "DH_Model.h"


ArmKinematic::ArmKinematic(float POriginPos[], float Pd[], float Pa[], float Palpha[], float Ptheta_home[], float Ptheta[], float Lowlimit[], float Uplimit[], float EEoffset[])
{
	////*********************************************************************************
	////----------------Initialize Parameter----------------------------
	//float POriginPos[] = {   0,   0,  0.84};
	////                    base    J1   J2    J3    J4    J5   J6    J7  J8(claw)
	//float Pd[]         = {   0,-0.301,   0, 0.224,    0,0.198,   0,    0,    0,    0};
	//float Pa[]         = {   0,     0,   0,  0.01,-0.01,    0,   0,0.124,0.072,0.022};
	//float Palpha[]     = { -90,   -90,  90,    90,  -90,   90,  90,    0,    0,    0};
	//float Ptheta_home[]= {   0,   -90, -90,    90,    0,    0,  90,    0,    0,   90};
	////float Palpha[]     = {  90,  -90, 90,  90,-90,  90,  90,   0,  0};
	////float Ptheta_home[]= {   0,  -90, 90, -90,  0,   0, -90, 180,  0};
	//float Ptheta[]     = {   0,    0,  0,   0,  0,   0,   0,   0,  0, 0};
	//float Lowlimit []  = {-180,-180,-190,-145,-180,-65,-115};
	//float Uplimit  []  = { 180,  30, 190,  30, 180, 55, 100};
	////*********************************************************************************
	//----------------Copy Parameter-----------------------------
	for(int i =0 ; i < 3 ; i++)
		CurrentPos[i] = POriginPos[i];
	for(int i =0 ; i < Tpara ; i++)
		d[i] = Pd[i];
	for(int i =0 ; i < Tpara ; i++)
		a[i] = Pa[i];
	for(int i =0 ; i < Tpara ; i++)
		alpha[i] = Palpha[i];
	for(int i =0 ; i < Tpara ; i++)
		theta_home[i] = Ptheta_home[i];
	for(int i =0 ; i < Tpara ; i++)
		theta[i] = Ptheta[i];	
	//for(int i =0 ; i < JDim-1; i++)
	//{
	//	RangeLimit limittemp = { Lowlimit[i],Uplimit[i] };
	//	Jointlimit.push_back(limittemp);
	//}
	for(int i =0 ; i < 4 ; i++)
		EEoffset[i] = offset[i];	
	//---------------Build Arm DH-model------------------------
	DHarm = new DHmodel(JDim,Tpara,CurrentPos,d,a,alpha,theta_home,theta,offset);
}

ArmKinematic::ArmKinematic(char *file)
{
	for(int i = 0 ; i < 4 ; i++)
		offset[i] = 0;

	std::ifstream DHpara;
	DHpara.open(file,ios::in);
	float Lowlimit [JDim-1];
	float Uplimit  [JDim-1];

	if(DHpara.is_open())
	{
		while(!DHpara.eof())
		{
			DHpara.ignore(1000,'#');

			char buffer[15];
			DHpara.getline(buffer,15,':');
			string str = buffer;
			
			if(str == "POS")
			{
				DHpara >> CurrentPos[0];
				for(int i = 1 ; i < 3 ; i++){
					DHpara.ignore(10,',');
					DHpara >> CurrentPos[i];				
				}			
			}
			else if(str == "d")
			{
				DHpara >> d[0];
				for(int i = 1 ; i < Tpara ; i++){	
					DHpara.ignore(10,',');
					DHpara >> d[i];
				}
			}
			else if(str == "a")
			{
				DHpara >> a[0];
				for(int i = 1 ; i < Tpara ; i++){	
					DHpara.ignore(10,',');
					DHpara >> a[i];
				}
			}
			else if(str == "alpha")
			{
				DHpara >> alpha[0];
				for(int i = 1 ; i < Tpara ; i++){	
					DHpara.ignore(10,',');
					DHpara >> alpha[i];
				}
			}
			else if(str == "theta_home")
			{
				DHpara >> theta_home[0];
				for(int i = 1 ; i < Tpara ; i++){	
					DHpara.ignore(10,',');
					DHpara >> theta_home[i];
				}
			}
			else if(str == "Lowlimit")
			{
				DHpara >> Lowlimit[0];
				for(int i = 1 ; i < JDim-1 ; i++){	
					DHpara.ignore(10,',');
					DHpara >> Lowlimit[i];
				}
			}
			else if(str == "Uplimit")
			{
				DHpara >> Uplimit[0];
				for(int i = 1 ; i < JDim-1 ; i++){	
					DHpara.ignore(10,',');
					DHpara >> Uplimit[i];
				}
			}
			else if(str == "EEoffset")
			{
				DHpara >> offset[0];
				for(int i = 1 ; i < 4 ; i++){	
					DHpara.ignore(10,',');
					DHpara >> offset[i];
				}
			}

			if(!DHpara.tellg())
			{
				printf("ERROR: file format is not correct\n");
				break;
			}
		}
		DHpara.close();
	}
	else
		printf("ERROR: File can't open\n");
	
	for(int i =0 ; i < Tpara ; i++)
		theta[i] = 0;	
	//for(int i =0 ; i < JDim-1; i++)
	//{
	//	RangeLimit limittemp = { Lowlimit[i],Uplimit[i] };
	//	Jointlimit.push_back(limittemp);
	//}	
	//---------------Build Arm DH-model------------------------
	DHarm = new DHmodel(JDim,Tpara,CurrentPos,d,a,alpha,theta_home,theta,offset);
}

void ArmKinematic::ResetTheta(float Position[] ,float Ptheta[])
{
	for(int i =1 ; i < JDim ; i++) //只傳手臂的角度，脖子與夾爪不管
		theta[i] = Ptheta[i-1];
	for(int i =0 ; i < 2 ; i++)
		CurrentPos[i] = Position[i];
	theta[0] = Position[2];

	DHarm->ReSetParameter(CurrentPos,d,a,alpha,theta_home,theta);
}

void ArmKinematic::ResetTheta(float Ptheta[])
{
	//for(int i =1 ; i < JDim ; i++) //只傳手臂的角度，脖子與夾爪不管
		//theta[i] = Ptheta[i-1];
	memcpy(theta+1, Ptheta, 6*sizeof(float));

	DHarm->ReSetParameter(CurrentPos,d,a,alpha,theta_home,theta);
}

void ArmKinematic::GetTheta(float *Ptheta)
{
	for(int k = 1 ; k < JDim ; k++)
		Ptheta[k-1] = theta[k];
}

void ArmKinematic::ClawTheta(float Ptheta)
{
	//printf("XXXXXXXXXXXXXXXX\n");
	//printf("Ptheta = %f\n", Ptheta);
	theta[7] = Ptheta;
	DHarm->ReSetParameter(CurrentPos,d,a,alpha,theta_home,theta);
}

ArmKinematic::~ArmKinematic()
{
	delete  DHarm;
}
