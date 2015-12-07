#include "OpenGL_Model\DH_matrix.h"
#include <math.h>

#ifndef PI
#define PI 3.14159
#endif

DHmatrix::DHmatrix()
{
	d          = 0;
	a          = 0;
	alpha      = 0;
	theta_home = 0;
	theta      = 0;
	A.ReSize(4,4);
}
DHmatrix::DHmatrix(float Pd, float Pa, float Palpha, float Ptheta_home, float Ptheta)
{
	d          = Pd			;
	a          = Pa			;
	alpha      = Palpha*PI/180		;
	theta_home = Ptheta_home*PI/180 ;
	theta      = Ptheta*PI/180		;

	//---------------initialize matrix--------------------------------------------------------
	A.ReSize(4,4);
	A(1,1) = cos(theta+theta_home);                A(1,2) = (-1)*sin(theta+theta_home)*cos(alpha);
	A(1,3) = sin(theta+theta_home)*sin(alpha);     A(1,4) = a*cos(theta+theta_home);
	A(2,1) = sin(theta+theta_home);                A(2,2) = cos(theta+theta_home)*cos(alpha);
	A(2,3) = (-1)*cos(theta+theta_home)*sin(alpha);A(2,4) = a*sin(theta+theta_home);
	A(3,1) = 0;                                    A(3,2) = sin(alpha);
	A(3,3) = cos(alpha);                           A(3,4) = d;
	A(4,1) = 0;                                    A(4,2) = 0;
	A(4,3) = 0;									   A(4,4) = 1;	
}
Matrix DHmatrix::T()
{
	return A;
}
Matrix DHmatrix::T(float Pd, float Pa, float Palpha, float Ptheta_home, float Ptheta)
{
	if(Pd == d && Pa == a && Palpha == alpha && Ptheta_home == theta_home && Ptheta == theta)
	{
		return A;
	}
	else
	{
		d          = Pd			;
		a          = Pa			;
		alpha      = Palpha*PI/180		;
		theta_home = Ptheta_home*PI/180 ;
		theta      = Ptheta*PI/180		;

		A.ReSize(4,4);
		A(1,1) = cos(theta+theta_home);                A(1,2) = (-1)*sin(theta+theta_home)*cos(alpha);
		A(1,3) = sin(theta+theta_home)*sin(alpha);     A(1,4) = a*cos(theta+theta_home);
		A(2,1) = sin(theta+theta_home);                A(2,2) = cos(theta+theta_home)*cos(alpha);
		A(2,3) = (-1)*cos(theta+theta_home)*sin(alpha);A(2,4) = a*sin(theta+theta_home);
		A(3,1) = 0;                                    A(3,2) = sin(alpha);
		A(3,3) = cos(alpha);                           A(3,4) = d;
		A(4,1) = 0;                                    A(4,2) = 0;
		A(4,3) = 0;									   A(4,4) = 1;									
		return A;
	}
}

//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//---------------------以下為DHmodel-----------------------------------


DHmodel::DHmodel(int JDim, int Tpara, float origin_Pos[], float d[], float a[], float alpha[], float theta_home[], float theta[], float EEoffset[])
{
	Scale_IK = 1000;
	Dim         = JDim;
	Para        = Tpara;
	Pd          = new float [Tpara];
	Pa          = new float [Tpara];
	Palpha      = new float [Tpara];
	Ptheta_home = new float [Tpara];
	Ptheta      = new float [Tpara];
	R           = new Matrix[Tpara];//多了個夾爪
	position    = new Matrix[JDim];

	prevX = -41.439613;
	prevY = -0.002471;
	prevZ = 1542.843628;
	
	//------------initialize parameter-------------------------
	for(int i = 0 ; i < 3 ; i++)
		OriginPos[i] = origin_Pos[i];
	for(int i = 0 ; i < Tpara ; i++)
		Pd[i] = d[i];
	for(int i = 0 ; i < Tpara ; i++)
		Pa[i] = a[i];
	for(int i = 0 ; i < Tpara ; i++)
		Palpha[i] = alpha[i];
	for(int i = 0 ; i < Tpara ; i++)
		Ptheta_home[i] = theta_home[i];
	for(int i = 0 ; i < Tpara ; i++)
		Ptheta[i] = theta[i];
	//for(int i = 0 ; i < JDim-1; i++)
	//{
	//	m_JointLimit.push_back(Joint_Limit[i]);
	//	m_JointLimit[i].lower_limit = m_JointLimit[i].lower_limit*PI/180;
	//	m_JointLimit[i].upper_limit = m_JointLimit[i].upper_limit*PI/180;
	//}
	for(int i = 0 ; i < 4 ; i++)
		offset[i] = EEoffset[i];


	//initialize the position and theta
	R_Pos2world.ReSize(4,4);
	R_Pos2world<<1   <<0   <<0  <<OriginPos[0]
			   <<0   <<1   <<0  <<OriginPos[1]
			   <<0   <<0   <<1  <<OriginPos[2]
			   <<0   <<0   <<0  <<1            ;
				
	//used to get the position vector of the homogeneous transformation matrix						
	zero_pointer.ReSize(4,1);
	zero_pointer<<0<<0<<0<<1;

	R[0] = R_Pos2world*Rtemp.T(Pd[0],Pa[0],Palpha[0],Ptheta_home[0],Ptheta[0]);
	position[0] = R[0]*zero_pointer;

	//------------calculate Arm Joint angle--------------------------------
	for(int i = 1 ; i < JDim ; i++)
	{
		R[i] = R[i-1]*Rtemp.T(Pd[i],Pa[i],Palpha[i],Ptheta_home[i],Ptheta[i]);
		position[i]  = R[i]*zero_pointer;
	}
	//對End-Effector做一些偏移
	/*R[Dim-1] = R[Dim-1]*Rtemp.T(offset[0], offset[1], offset[2], offset[3], 0)
					   *Rtemp.T(        0,         0,-offset[2],-offset[3], 0);*/
	
	//cout<<R[7];
	//================計算兩片夾爪的位置====================================
	/*Matrix Rbias(4,4);
	Rbias = R[6]*Rtemp.T(Pd[8] ,Pa[8],Palpha[7],Ptheta_home[7],Ptheta[7]);
	R[Dim  ] = Rbias*Rtemp.T( 0,-Pa[9],Palpha[9], Ptheta_home[9],   0)
					*Rtemp.T( 0,     0,Palpha[8],-Ptheta_home[9]+Ptheta_home[8], Ptheta[8]);

	R[Dim+1] = Rbias*Rtemp.T( 0, Pa[9],Palpha[9], Ptheta_home[9],         0)
		            *Rtemp.T( 0,     0,Palpha[8],-Ptheta_home[9]-Ptheta_home[8],-Ptheta[8]);*/
	Matrix Rbias(4,4);
	Rbias = R[6]*Rtemp.T(0,0,Palpha[7],Ptheta_home[7],0);
	R[Dim  ] = Rbias*Rtemp.T(0, 0, 0, 0, 0);
	R[Dim+1] = Rbias*Rtemp.T(0, 0, 0, 0, 0);
}
void DHmodel::ReSetParameter(float Position[] ,float d[], float a[], float alpha[], float theta_home[], float theta[])
{
	//------------initialize parameter-------------------------
	for(int i = 0 ; i < 3 ; i++)
		OriginPos[i] = Position[i];
	for(int i = 0 ; i < Para ; i++)
		Pd[i] = d[i];
	for(int i = 0 ; i < Para ; i++)
		Pa[i] = a[i];
	for(int i = 0 ; i < Para ; i++)
		Palpha[i] = alpha[i];
	for(int i = 0 ; i < Para ; i++)
		Ptheta_home[i] = theta_home[i];
	for(int i = 0 ; i < Para ; i++)
		Ptheta[i] = theta[i];

	R_Pos2world(1,4) = OriginPos[0];
	R_Pos2world(2,4) = OriginPos[1];
	R_Pos2world(3,4) = OriginPos[2];

	R[0] = R_Pos2world*Rtemp.T(Pd[0],Pa[0],Palpha[0],Ptheta_home[0],Ptheta[0]);
	position[0] = R[0]*zero_pointer;

	//------------calculate World-theta  World-position-----------------------------------
	for(int i = 1 ; i < Dim ; i++)
	{
		R[i] = R[i-1]*Rtemp.T(Pd[i],Pa[i],Palpha[i],Ptheta_home[i],Ptheta[i]);
		position[i]  = R[i]*zero_pointer;
	}
	//對End-Effector做一些偏移
	/*R[Dim-1] = R[Dim-1]*Rtemp.T(offset[0], offset[1], offset[2], offset[3], 0)
					   *Rtemp.T(        0,         0,-offset[2],-offset[3], 0);*/
	//cout<<R[7];
	//================計算兩片夾爪的位置====================================
	/*Matrix Rbias(4,4);
	Rbias = R[5]*Rtemp.T(Pd[7] ,Pa[7],Palpha[6],Ptheta_home[6],Ptheta[6]);
	R[Dim  ] = Rbias*Rtemp.T( 0,-Pa[8],Palpha[8], Ptheta_home[8],   0)
					*Rtemp.T( 0,     0,Palpha[7],-Ptheta_home[8]+Ptheta_home[7], Ptheta[7]);

	R[Dim+1] = Rbias*Rtemp.T( 0, Pa[8],Palpha[8], Ptheta_home[8],         0)
		            *Rtemp.T( 0,     0,Palpha[7],-Ptheta_home[8]-Ptheta_home[7],-Ptheta[7]);

	R[Dim  ] = R[Dim  ]*Rtemp.T(0, 0, 0, 0, 0);
	R[Dim+1] = R[Dim+1]*Rtemp.T(0, 0, 0, 0, 0);*/
	Matrix Rbias(4,4);
	Rbias = R[6]*Rtemp.T(0,0,Palpha[7],Ptheta_home[7],0);
	R[Dim  ] = Rbias*Rtemp.T(0, Ptheta[7]/1000, 0, 0, 0);
	R[Dim+1] = Rbias*Rtemp.T(0, -Ptheta[7]/1000, 0, 0, 0);
	//printf("============   %d  ============\n",Ptheta[7]);

	
}



Matrix DHmodel::Cross(Matrix a, Matrix b)
{
	Matrix M(3,1); M = 0;
	M(1,1) = a(2,1)*b(3,1) - a(3,1)*b(2,1);
	M(2,1) = a(3,1)*b(1,1) - a(1,1)*b(3,1);
	M(3,1) = a(1,1)*b(2,1) - a(2,1)*b(1,1);
    
	return M;
}

void DHmodel::GetJacobian(Matrix &J)
{
	Matrix K(3,1);
	Matrix O(3,1);
	Matrix a(3,1);
	a = 0; a(3,1) = 1; 
	
	for (int i = 1; i < 7 ; i++ )
	{	
		K = R[i-1].SubMatrix(1,3,1,3)*a;
		O = (R[Dim-1].SubMatrix(1,3,4,4) - R[i-1].SubMatrix(1,3,4,4))*Scale_IK;
		J.SubMatrix(1,3,i,i) = Cross(K,O);
		J.SubMatrix(4,6,i,i) = K;
	}	
}

//void DHmodel::IK(float goal[], int goal_N, float m_theta[])
//{
//	// goal point is in the Cartisien coordinate
//	//===========================================
//	int i,j;
//	int m_Dim = 6;
//	float min_limit, max_limit;
//
//	int iter_N = 2000; // iteration number
//	float k=-0.000005;
//	float theta_factor = 1000;
//	float hs=500000000;
//	
//	float N = 6;
//	float alpha_gain = 0 , h=0;
//
//	Matrix Current_Pos(N,1), Goal_Pos(N,1), Delta_Pos(N,1), d_H(m_Dim,1), delta_theta(m_Dim,1);
//	Matrix Mtheta(m_Dim,1);
//	Matrix J(N,m_Dim), J_pesudo(m_Dim,N), DLS(N,m_Dim) ,JJ(N,N);
//	IdentityMatrix I(m_Dim);
//	IdentityMatrix I_N(N);
//
//	Goal_Pos(1,1) = Scale_IK*goal[0]; 
//	Goal_Pos(2,1) = Scale_IK*goal[1]; 
//	Goal_Pos(3,1) = Scale_IK*goal[2];
//	Goal_Pos(4,1) = goal[3]*PI/180;
//	Goal_Pos(5,1) = goal[4]*PI/180;
//	Goal_Pos(6,1) = goal[5]*PI/180;
//
//	//================= elbow 部分的控制參數 ============================
//	//Matrix elbow(3,1)  , Delta_elbow(3,1);
//	//Matrix eJ(3,m_Dim) , eJ_pesudo(m_Dim,3) , eJJ(3,3) , eDLS(3,m_Dim);
//	//eJ = 0; eJ_pesudo = 0; eJJ = 0; eDLS = 0;
//	//elbow(1,1) = goal[0]; 
//	//elbow(2,1) = goal[1]+0.01; 
//	//elbow(3,1) = goal[2]+0.38;
//
//	for (i = 0 ; i < iter_N ; i++)
//	{
//
//		ReSetParameter(OriginPos, Pd,  Pa , Palpha , Ptheta_home , Ptheta );
//		//----------將"角度"轉成"radian"--------------------------------
//		for(int j = 0; j < 7 ;j++ )
//			Ptheta[j] = Ptheta[j]*PI/180;
//		//for(int i = 0; i < 8 ;i++ )
//		//	cout<<Ptheta[i]<<" , ";
//		//cout<<endl;
//		
//		// get Delta_H for GPM
//		//======================================
//		//for (j = 0 ; j< m_Dim ; j++)
//		//{
//		//	min_limit = m_JointLimit[j].lower_limit;
//		//	max_limit = m_JointLimit[j].upper_limit;
//		//	d_H(j+1,1) = (max_limit-min_limit)*(max_limit-min_limit)*(2*Ptheta[j+1]-max_limit-min_limit);
//		//	d_H(j+1,1) = d_H(j+1,1)/(4*(max_limit-Ptheta[j+1])*(max_limit-Ptheta[j+1])*(Ptheta[j+1]-min_limit)*(Ptheta[j+1]-min_limit));
//		//}
//
//		// IK
//		//======================================
//		GetJacobian(J);
//		
//		for (j = 0 ; j< m_Dim ; j++)
//			Mtheta(j+1,1) = Ptheta[j+1];
//	
//		Current_Pos.SubMatrix(1,3,1,1) = R[m_Dim].SubMatrix(1,3,4,4)*Scale_IK;
//		
//		//-------------------Rotation Matrix To Euler Angle-----------------------------------
//		float angle[3] = {0};		
//		float RotM[3][3];
//		for(int x=0;x<3;x++)
//			for(int y=0;y<3;y++)
//				RotM[x][y] = R[m_Dim](x+1,y+1);
//		RotM2Euler(RotM,angle);
//		//-------------------------------------------------------------------------------------
//		Current_Pos(4,1) = angle[0];
//		Current_Pos(5,1) = angle[1];
//		Current_Pos(6,1) = angle[2];
//		
//		Delta_Pos   = Goal_Pos - Current_Pos; //Delta X ( x_dot = J * Theta_dot)
//		Delta_Pos.SubMatrix(4,goal_N,1,1) = theta_factor*Delta_Pos.SubMatrix(4,goal_N,1,1); //放大角度誤差使其快速收斂
//		printf("%f  %f  %f\n",angle[0],angle[1],angle[2]);
//		JJ = J*J.t(); //JJ = J * Jtranspose	
//
//		//Delta_elbow = (elbow - R[4].SubMatrix(1,3,4,4))*300;//elbow的位置差
//		//eJ.SubMatrix(1,3,1,3) = J.SubMatrix(1,3,1,3);
//		//eJJ = eJ*eJ.t();
//		//------------------DLS Singularity Avoidance-----------------------------------------
//		h = (float)sqrt(JJ.Determinant());
//		if(h < hs)
//			alpha_gain = 5000*(1-h/hs);
//		else
//			alpha_gain = 0;
//
//		DLS = JJ + alpha_gain*I_N;
//		J_pesudo = J.t()*DLS.i();
//		//------------------------------------------------------------------------------------
//
//		////------------------DLS Singularity Avoidance-----------------------------------------
//		//h = (float)sqrt(eJJ.Determinant());
//		//if(h < hs)
//		//	alpha_gain = 40000*(1-h/hs);
//		//else
//		//	alpha_gain = 0;
//		//IdentityMatrix eI_N(3);
//		//eDLS = eJJ + alpha_gain*eI_N;
//		//eJ_pesudo = eJ.t()*eDLS.i();
//		////------------------------------------------------------------------------------------		
//
//		//========================主要運算式============================
//		//右邊第二項為GPM Joint Limit Avoidance
//		delta_theta = J_pesudo*Delta_Pos + k*(I-J_pesudo*J)*d_H /*+ eJ_pesudo * Delta_elbow*/;
//		//==============================================================
//
//		if(delta_theta.MaximumAbsoluteValue() > 0.01)
//			delta_theta = delta_theta/delta_theta.MaximumAbsoluteValue()*0.01;
//
//		//-------------Update theta & "radian" 轉 "角度"--------------------------
// 		for (j = 0 ; j< m_Dim ; j++)
//			Ptheta[j+1] = (Ptheta[j+1] + delta_theta(j+1,1))*180/PI;
//
//		//-------------如果ERROR小於這區間就停止----------------------------------
//		if(fabs(Delta_Pos(1,1)) < 1  && fabs(Delta_Pos(2,1)) < 1 && fabs(Delta_Pos(3,1)) < 1 &&
//		   fabs(Delta_Pos(4,1)) < 1  && fabs(Delta_Pos(5,1)) < 1 && fabs(Delta_Pos(6,1)) < 1)
//		   break;
//	}
//	//cout<<Delta_Pos;
//	for (j = 0 ; j < m_Dim ; j++)
//		m_theta[j] = Ptheta[j+1];
//	for (j = 0 ; j < 6 ; j++)
//		EECoor[j] = Current_Pos(j+1,1);
//
//	printf("Joint: %8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f\n",m_theta[0],m_theta[1],m_theta[2],m_theta[3],m_theta[4],m_theta[5],m_theta[6]);
//	printf("Error: %8.4f,%8.4f,%8.4f,%8.4f,%8.4f,%8.4f\n",Delta_Pos(1,1),Delta_Pos(2,1),Delta_Pos(3,1),Delta_Pos(4,1),Delta_Pos(5,1),Delta_Pos(6,1));
//}

//bool DHmodel::Check_IK(float goal[], int goal_N, float m_theta[])
//{
//	// goal point is in the Cartisien coordinate
//	//===========================================
//	int i,j;
//	int m_Dim = 6;
//	float min_limit, max_limit;
//
//	int iter_N = 2000; // iteration number
//	float k=-0.000005;
//	float theta_factor = 1000;
//	float hs=500000000;
//	
//	float N = 6;
//	float alpha_gain = 0 , h=0;
//
//	Matrix Current_Pos(N,1), Goal_Pos(N,1), Delta_Pos(N,1), d_H(m_Dim,1), delta_theta(m_Dim,1);
//	Matrix Mtheta(m_Dim,1);
//	Matrix J(N,m_Dim), J_pesudo(m_Dim,N), DLS(N,m_Dim) ,JJ(N,N);
//	IdentityMatrix I(m_Dim);
//	IdentityMatrix I_N(N);
//
//	Goal_Pos(1,1) = Scale_IK*goal[0]; 
//	Goal_Pos(2,1) = Scale_IK*goal[1]; 
//	Goal_Pos(3,1) = Scale_IK*goal[2];
//	Goal_Pos(4,1) = goal[3]*PI/180;
//	Goal_Pos(5,1) = goal[4]*PI/180;
//	Goal_Pos(6,1) = goal[5]*PI/180;
//
//	//================= elbow 部分的控制參數 ============================
//	//Matrix elbow(3,1)  , Delta_elbow(3,1);
//	//Matrix eJ(3,m_Dim) , eJ_pesudo(m_Dim,3) , eJJ(3,3) , eDLS(3,m_Dim);
//	//eJ = 0; eJ_pesudo = 0; eJJ = 0; eDLS = 0;
//	//elbow(1,1) = goal[0]; 
//	//elbow(2,1) = goal[1]+0.01; 
//	//elbow(3,1) = goal[2]+0.38;
//
//	for (i = 0 ; i < iter_N ; i++)
//	{
//
//		ReSetParameter(OriginPos, Pd,  Pa , Palpha , Ptheta_home , Ptheta );
//		//----------將"角度"轉成"radian"--------------------------------
//		for(int j = 0; j < 7 ;j++ )
//			Ptheta[j] = Ptheta[j]*PI/180;
//		//for(int i = 0; i < 8 ;i++ )
//		//	cout<<Ptheta[i]<<" , ";
//		//cout<<endl;
//		
//		// get Delta_H for GPM
//		//======================================
//		for (j = 0 ; j< m_Dim ; j++)
//		{
//			min_limit = m_JointLimit[j].lower_limit;
//			max_limit = m_JointLimit[j].upper_limit;
//			d_H(j+1,1) = (max_limit-min_limit)*(max_limit-min_limit)*(2*Ptheta[j+1]-max_limit-min_limit);
//			d_H(j+1,1) = d_H(j+1,1)/(4*(max_limit-Ptheta[j+1])*(max_limit-Ptheta[j+1])*(Ptheta[j+1]-min_limit)*(Ptheta[j+1]-min_limit));
//		}
//
//		// IK
//		//======================================
//		GetJacobian(J);
//		
//		for (j = 0 ; j< m_Dim ; j++)
//			Mtheta(j+1,1) = Ptheta[j+1];
//	
//		Current_Pos.SubMatrix(1,3,1,1) = R[m_Dim].SubMatrix(1,3,4,4)*Scale_IK;
//		
//		//-------------------Rotation Matrix To Euler Angle-----------------------------------
//		float angle[3] = {0};		
//		float RotM[3][3];
//		for(int x=0;x<3;x++)
//			for(int y=0;y<3;y++)
//				RotM[x][y] = R[m_Dim](x+1,y+1);
//		RotM2Euler(RotM,angle);
//		//-------------------------------------------------------------------------------------
//		Current_Pos(4,1) = angle[0];
//		Current_Pos(5,1) = angle[1];
//		Current_Pos(6,1) = angle[2];
//
//		//if(i==0)
//		//{
//		//	for (j = 0 ; j < 6 ; j++)
//		//		EECoor[j] = Current_Pos(j+1,1);
//		//	prevX = EECoor[0];
//		//	prevY = EECoor[1];
//		//	prevZ = EECoor[2];
//		//	//for (j = 0 ; j < 6 ; j++)
//		//		printf("%f  %f  %f\n",prevX, prevY, prevZ);
//		//}
//		
//		Delta_Pos   = Goal_Pos - Current_Pos; //Delta X ( x_dot = J * Theta_dot)
//		Delta_Pos.SubMatrix(4,goal_N,1,1) = theta_factor*Delta_Pos.SubMatrix(4,goal_N,1,1); //放大角度誤差使其快速收斂
//		JJ = J*J.t(); //JJ = J * Jtranspose	
//
//		//Delta_elbow = (elbow - R[4].SubMatrix(1,3,4,4))*300;//elbow的位置差
//		//eJ.SubMatrix(1,3,1,3) = J.SubMatrix(1,3,1,3);
//		//eJJ = eJ*eJ.t();
//		//------------------DLS Singularity Avoidance-----------------------------------------
//		h = (float)sqrt(JJ.Determinant());
//		if(h < hs)
//			alpha_gain = 5000*(1-h/hs);
//		else
//			alpha_gain = 0;
//
//		DLS = JJ + alpha_gain*I_N;
//		J_pesudo = J.t()*DLS.i();
//		//------------------------------------------------------------------------------------
//
//		////------------------DLS Singularity Avoidance-----------------------------------------
//		//h = (float)sqrt(eJJ.Determinant());
//		//if(h < hs)
//		//	alpha_gain = 40000*(1-h/hs);
//		//else
//		//	alpha_gain = 0;
//		//IdentityMatrix eI_N(3);
//		//eDLS = eJJ + alpha_gain*eI_N;
//		//eJ_pesudo = eJ.t()*eDLS.i();
//		////------------------------------------------------------------------------------------		
//
//		//========================主要運算式============================
//		//右邊第二項為GPM Joint Limit Avoidance
//		delta_theta = J_pesudo*Delta_Pos + k*(I-J_pesudo*J)*d_H /*+ eJ_pesudo * Delta_elbow*/;
//		//==============================================================
//
//		if(delta_theta.MaximumAbsoluteValue() > 0.01)
//			delta_theta = delta_theta/delta_theta.MaximumAbsoluteValue()*0.01;
//
//		//-------------Update theta & "radian" 轉 "角度"--------------------------
// 		for (j = 0 ; j< m_Dim ; j++)
//			Ptheta[j+1] = (Ptheta[j+1] + delta_theta(j+1,1))*180/PI;
//
//		//-------------如果ERROR小於這區間就停止----------------------------------
//		if(fabs(Delta_Pos(1,1)) < 1  && fabs(Delta_Pos(2,1)) < 1 && fabs(Delta_Pos(3,1)) < 1 &&
//		   fabs(Delta_Pos(4,1)) < 10  && fabs(Delta_Pos(5,1)) < 10 && fabs(Delta_Pos(6,1)) < 10)
//		{
//			for (j = 0 ; j < m_Dim ; j++)
//				m_theta[j] = Ptheta[j+1];
//			for (j = 0 ; j < 6 ; j++)
//				EECoor[j] = Current_Pos(j+1,1);
//
//			prevX = EECoor[0];
//			prevY = EECoor[1];
//			prevZ = EECoor[2];
//			//printf("%f  %f  %f\n",prevX, prevY, prevZ);
//			printf("i = %d\n",i);
//
//			printf("1: IK DONE !\n");
//			return true;
//
//			break;
//		}
//	}
//	//cout<<Delta_Pos;
//	for (j = 0 ; j < m_Dim ; j++)
//		m_theta[j] = Ptheta[j+1];
//	for (j = 0 ; j < 6 ; j++)
//		EECoor[j] = Current_Pos(j+1,1);
//
//
//	if(fabs(Delta_Pos(1,1)) < 5  && fabs(Delta_Pos(2,1)) < 5 && fabs(Delta_Pos(3,1)) < 5 &&
//	   fabs(Delta_Pos(4,1)) < 150  && fabs(Delta_Pos(5,1)) < 150 && fabs(Delta_Pos(6,1)) < 150)
//	{
//		prevX = EECoor[0];
//		prevY = EECoor[1];
//		prevZ = EECoor[2];
//		//printf("%f  %f  %f\n",prevX, prevY, prevZ);
//		printf("i = %d\n",i);
//
//		printf("2: margin !\n");
//		return true;
//	}
//	else
//	{
//		printf("IK FAIL :( \n");
//		printf("Joint: %8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f\n",m_theta[0],m_theta[1],m_theta[2],m_theta[3],m_theta[4],m_theta[5],m_theta[6]);
//		printf("Error: %8.4f,%8.4f,%8.4f,%8.4f,%8.4f,%8.4f\n",Delta_Pos(1,1),Delta_Pos(2,1),Delta_Pos(3,1),Delta_Pos(4,1),Delta_Pos(5,1),Delta_Pos(6,1));
//		return false;
//	}
//
//	//printf("Joint: %8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f\n",m_theta[0],m_theta[1],m_theta[2],m_theta[3],m_theta[4],m_theta[5],m_theta[6]);
//	//printf("Error: %8.4f,%8.4f,%8.4f,%8.4f,%8.4f,%8.4f\n",Delta_Pos(1,1),Delta_Pos(2,1),Delta_Pos(3,1),Delta_Pos(4,1),Delta_Pos(5,1),Delta_Pos(6,1));
//	
//}

void DHmodel::RotM2Euler(float m[][3] , float Euler[])
{
	if (m[1][0] > 0.998)
	{ // singularity at north pole
		Euler[1] = atan2(m[0][2],m[2][2]);		
		Euler[2] = PI/2;
		Euler[0] = 0;
		return;
	}	
	if (m[1][0] < -0.998)
	{ // singularity at south pole
		Euler[1] = atan2(m[0][2],m[2][2]);		
		Euler[2] = -PI/2;
		Euler[0] = 0;
		return;
	}	
	Euler[1] = atan2(-m[2][0],m[0][0]);
	Euler[0] = atan2(-m[1][2],m[1][1]);
	Euler[2] =  asin(m[1][0]);
}

DHmodel::~DHmodel()
{
	delete [] Pd;
	delete [] Pa;
	delete [] Palpha;	
	delete [] Ptheta_home;
	delete [] Ptheta;
	delete [] R;
	delete [] position;
}

