#include "stdafx.h"
#include "OpenGL_Model\OpenGLControl.h"


using namespace std;
#define PI 3.14159265

//
//void COpenGLControl::SquareGo()
//{
//	double sqarePos[5][2] = {{0.1,0.},{0.,-0.2},{-0.2,0.},{0.,0.2},{0.1,0.}};
//
//	bool IKflag = true;
//	RActionPath.clear();
//	Rcount = 0;
//
//	for (int i = 0 ; i <= 100 ; i++)
//	{
//		degx = (rpX+(((StaticRX-rpX)/NumPts)*i));///180.0*PI;
//		degy = (rpY+(((StaticRY-rpY)/NumPts)*i));///180.0*PI;
//		degz = (rpZ+(((StaticRZ-rpZ)/NumPts)*i));///180.0*PI;
//
//		rdegx = degx/180.0*PI;
//		rdegy = degy/180.0*PI;
//		rdegz = degz/180.0*PI;
//		KineAll.TarRotMSw[0] = cos(rdegy)*cos(rdegz);
//		KineAll.TarRotMSw[1] = -cos(rdegx)*sin(rdegz)+sin(rdegx)*sin(rdegy)*cos(rdegz);
//		KineAll.TarRotMSw[2] = sin(rdegx)*sin(rdegz)+cos(rdegx)*sin(rdegy)*cos(rdegz);
//		KineAll.TarRotMSw[3] = cos(rdegy)*sin(rdegz);
//		KineAll.TarRotMSw[4] = cos(rdegx)*cos(rdegz)+sin(rdegx)*sin(rdegy)*sin(rdegz);
//		KineAll.TarRotMSw[5] = -sin(rdegx)*cos(rdegz)+cos(rdegx)*sin(rdegy)*sin(rdegz);
//		KineAll.TarRotMSw[6] = -sin(rdegy);
//		KineAll.TarRotMSw[7] = sin(rdegx)*cos(rdegy);
//		KineAll.TarRotMSw[8] = cos(rdegx)*cos(rdegy);
//
//
//		TrajInput[0] = pX + ( ( (0.5*1000)-pX)/NumPts )*i;
//		TrajInput[1] = pY + ( ( (0*1000)-pY)/NumPts )*i;
//		TrajInput[2] = pZ + ( ( (1.5*1000)-pZ)/NumPts )*i;
//
//		if ( KineAll.IK_Solve(TrajInput, KineAll.TarRotMSw) == 0 )
//		{
//			Dimension theta;
//			theta.Pos = new float[7];
//			for(int j = 0; j < 6; j++)
//			{
//				theta.Pos[j] = (float)KineAll.FK_LLeg->theta[j+1]*180.0/3.1415926;
//				//printf("%d = %f\n",j+1,theta.Pos[j]);
//			}
//			//printf("%f\n",theta.Pos[3]);	
//			//theta.Pos[6] = RClawTheta;
//
//			RActionPath.push_back(theta);
//		}
//		else
//		{
//			IKflag = false;
//			KineAll.cntIK = 0;
//			printf("[Warning]   IK fail !!\n");
//			break;
//		}
//
//		//printf("ith = %d, Count = %d \n", i, KineAll.cntIK);
//
//		KineAll.cntIK = 0;
//	}
//
//	if (IKflag)
//	{
//		pX = KineAll.CrdAll->data[18];
//		pY = KineAll.CrdAll->data[19];
//		pZ = KineAll.CrdAll->data[20];
//
//		rpX = degx;
//		rpY = degy;
//		rpZ = degz;
//	}
//	else
//	{
//		printf("[IK Fail]   ResetArm !!\n");
//		ResetRArm();
//
//		//Kinematics initialization
//		KineAll.InitKineTrains();
//		KineAll.FindFK();
//		////KineAll.FindCOG();
//		KineAll.ComputeJacobians();
//
//		KineAll.GetLegsCoords();
//
//		KineAll.ComputeEulerAng(KineAll.LLegRotM, KineAll.EuAngLL);
//
//		KineAll.initLL[0] = KineAll.CrdAll->data[18];
//		KineAll.initLL[1] = KineAll.CrdAll->data[19];
//		KineAll.initLL[2] = KineAll.CrdAll->data[20];
//
//		pX = KineAll.initLL[0];
//		pY = KineAll.initLL[1];
//		pZ = KineAll.initLL[2];
//
//		rpX = KineAll.EuAngLL[0]/3.14159265*180.;
//		rpY = KineAll.EuAngLL[1]/3.14159265*180.;
//		rpZ = KineAll.EuAngLL[2]/3.14159265*180.;
//	}
//
//	for (int k = 0 ; k < 5 ; k++)
//	{
//		for (int i = 0 ; i <= NumPts ; i++)
//		{
//			degx = (rpX+(((StaticRX-rpX)/NumPts)*i));///180.0*PI;
//			degy = (rpY+(((StaticRY-rpY)/NumPts)*i));///180.0*PI;
//			degz = (rpZ+(((StaticRZ-rpZ)/NumPts)*i));///180.0*PI;
//
//			rdegx = degx/180.0*PI;
//			rdegy = degy/180.0*PI;
//			rdegz = degz/180.0*PI;
//			KineAll.TarRotMSw[0] = cos(rdegy)*cos(rdegz);
//			KineAll.TarRotMSw[1] = -cos(rdegx)*sin(rdegz)+sin(rdegx)*sin(rdegy)*cos(rdegz);
//			KineAll.TarRotMSw[2] = sin(rdegx)*sin(rdegz)+cos(rdegx)*sin(rdegy)*cos(rdegz);
//			KineAll.TarRotMSw[3] = cos(rdegy)*sin(rdegz);
//			KineAll.TarRotMSw[4] = cos(rdegx)*cos(rdegz)+sin(rdegx)*sin(rdegy)*sin(rdegz);
//			KineAll.TarRotMSw[5] = -sin(rdegx)*cos(rdegz)+cos(rdegx)*sin(rdegy)*sin(rdegz);
//			KineAll.TarRotMSw[6] = -sin(rdegy);
//			KineAll.TarRotMSw[7] = sin(rdegx)*cos(rdegy);
//			KineAll.TarRotMSw[8] = cos(rdegx)*cos(rdegy);
//
//
//			TrajInput[0] = pX; //+ ( ( (StaticX*1000)-pX)/NumPts )*i;
//			TrajInput[1] = pY + ( ( (sqarePos[k][0]*1000))/NumPts )*i;
//			TrajInput[2] = pZ + ( ( (sqarePos[k][1]*1000))/NumPts )*i;
//
//			if ( KineAll.IK_Solve(TrajInput, KineAll.TarRotMSw) == 0 )
//			{
//				Dimension theta;
//				theta.Pos = new float[7];
//				for(int j = 0; j < 6; j++)
//				{
//					theta.Pos[j] = (float)KineAll.FK_LLeg->theta[j+1]*180.0/3.1415926;
//					//printf("%d = %f\n",j+1,theta.Pos[j]);
//				}
//				//printf("%f\n",theta.Pos[3]);	
//				//theta.Pos[6] = RClawTheta;
//
//				RActionPath.push_back(theta);
//			}
//			else
//			{
//				IKflag = false;
//				KineAll.cntIK = 0;
//				printf("[Warning]   IK fail !!\n");
//				break;
//			}
//
//			//printf("ith = %d, Count = %d \n", i, KineAll.cntIK);
//
//			KineAll.cntIK = 0;
//		}
//
//		if (IKflag)
//		{
//			pX = KineAll.CrdAll->data[18];
//			pY = KineAll.CrdAll->data[19];
//			pZ = KineAll.CrdAll->data[20];
//
//			rpX = degx;
//			rpY = degy;
//			rpZ = degz;
//		}
//		else
//		{
//			printf("[IK Fail]   ResetArm !!\n");
//			ResetRArm();
//
//			//Kinematics initialization
//			KineAll.InitKineTrains();
//			KineAll.FindFK();
//			////KineAll.FindCOG();
//			KineAll.ComputeJacobians();
//
//			KineAll.GetLegsCoords();
//
//			KineAll.ComputeEulerAng(KineAll.LLegRotM, KineAll.EuAngLL);
//
//			KineAll.initLL[0] = KineAll.CrdAll->data[18];
//			KineAll.initLL[1] = KineAll.CrdAll->data[19];
//			KineAll.initLL[2] = KineAll.CrdAll->data[20];
//
//			pX = KineAll.initLL[0];
//			pY = KineAll.initLL[1];
//			pZ = KineAll.initLL[2];
//
//			rpX = KineAll.EuAngLL[0]/3.14159265*180.;
//			rpY = KineAll.EuAngLL[1]/3.14159265*180.;
//			rpZ = KineAll.EuAngLL[2]/3.14159265*180.;
//		}
//	}
//
//	fstream mfile4;
//	mfile4.open("C:\\squarePath.txt", ios::out);
//	for( int i = 0; i < RActionPath.size(); i++)
//	{
//		for( int j = 0; j < 6; j++)
//			mfile4<<RActionPath[i].Pos[j]<<" ";
//		mfile4<<endl;
//	}
//	mfile4.close();
//
//	SetTimer(7,Timer,NULL);
//}
//
//void COpenGLControl::GOmanubb()
//{
//	bool IKflag = true;
//	RActionPath.clear();
//	Rcount = 0;
//
//	//double delta_theta[7] = {0};
//
//	//double Qref[6] = {  90., 0., 0., 0., 0., 0.};
//
//	//for (int i=0;i<6;i++)
//	//	delta_theta[i] = -1*(Qref[i]/100.);
//
//	//RActionPath.clear();
//	////indwrite = 0;
//	//Rcount = 0;
//
//	//for(int i = 1; i < 101; i++)
//	//{
//	//	//printf("RClawTheta = %f\n",RClawTheta);
//	//	Dimension theta;
//	//	theta.Pos = new float[7];
//
//	//	for(int j = 0; j < 6; j++)
//	//		theta.Pos[j] = RAini_theta[j]+i*delta_theta[j];
//
//	//	theta.Pos[6] = RClawTheta;
//
//	//	RActionPath.push_back(theta);
//	//}
//
//	//return true;
//
//	if(NumPts>0)
//	{
//		
//		for (int i = 0 ; i<=NumPts ; i++)
//		{
//			degx = (rpX+(((StaticRX-rpX)/NumPts)*i));///180.0*PI;
//			degy = (rpY+(((StaticRY-rpY)/NumPts)*i));///180.0*PI;
//			degz = (rpZ+(((StaticRZ-rpZ)/NumPts)*i));///180.0*PI;
//
//			rdegx = degx/180.0*PI;
//			rdegy = degy/180.0*PI;
//			rdegz = degz/180.0*PI;
//			KineAll.TarRotMSw[0] = cos(rdegy)*cos(rdegz);
//			KineAll.TarRotMSw[1] = -cos(rdegx)*sin(rdegz)+sin(rdegx)*sin(rdegy)*cos(rdegz);
//			KineAll.TarRotMSw[2] = sin(rdegx)*sin(rdegz)+cos(rdegx)*sin(rdegy)*cos(rdegz);
//			KineAll.TarRotMSw[3] = cos(rdegy)*sin(rdegz);
//			KineAll.TarRotMSw[4] = cos(rdegx)*cos(rdegz)+sin(rdegx)*sin(rdegy)*sin(rdegz);
//			KineAll.TarRotMSw[5] = -sin(rdegx)*cos(rdegz)+cos(rdegx)*sin(rdegy)*sin(rdegz);
//			KineAll.TarRotMSw[6] = -sin(rdegy);
//			KineAll.TarRotMSw[7] = sin(rdegx)*cos(rdegy);
//			KineAll.TarRotMSw[8] = cos(rdegx)*cos(rdegy);
//
//
//			TrajInput[0] = pX + ( ( (StaticX*1000)-pX)/NumPts )*i;
//			TrajInput[1] = pY + ( ( (StaticY*1000)-pY)/NumPts )*i;
//			TrajInput[2] = pZ + ( ( (StaticZ*1000)-pZ)/NumPts )*i;
//
//			if ( KineAll.IK_Solve(TrajInput, KineAll.TarRotMSw) == 0 )
//			{
//				Dimension theta;
//				theta.Pos = new float[7];
//				for(int j = 0; j < 6; j++)
//				{
//					theta.Pos[j] = (float)KineAll.FK_LLeg->theta[j+1]*180.0/3.1415926;
//					//printf("%d = %f\n",j+1,theta.Pos[j]);
//				}
//				//printf("%f\n",theta.Pos[3]);	
//				//theta.Pos[6] = RClawTheta;
//
//				RActionPath.push_back(theta);
//			}
//			else
//			{
//				IKflag = false;
//				KineAll.cntIK = 0;
//				cout<<"[Warning]   IK fail: #"<<i<<" point!!"<<endl;
//				break;
//			}
//
//			//printf("ith = %d, Count = %d \n", i, KineAll.cntIK);
//
//			KineAll.cntIK = 0;
//
//		}
//
//		if (IKflag)
//		{
//			pX = KineAll.CrdAll->data[18];
//			pY = KineAll.CrdAll->data[19];
//			pZ = KineAll.CrdAll->data[20];
//
//			rpX = degx;
//			rpY = degy;
//			rpZ = degz;
//
//			SetTimer(7,Timer,NULL);
//		}
//		else
//		{
//			printf("[IK Fail]   ResetArm !!\n");
//
//			//ResetRArm();
//			//
//			////Kinematics initialization
//			//KineAll.InitKineTrains();
//			//KineAll.FindFK();
//			//////KineAll.FindCOG();
//			//KineAll.ComputeJacobians();
//
//			//KineAll.GetLegsCoords();
//
//			//KineAll.ComputeEulerAng(KineAll.LLegRotM, KineAll.EuAngLL);
//
//			//KineAll.initLL[0] = KineAll.CrdAll->data[18];
//			//KineAll.initLL[1] = KineAll.CrdAll->data[19];
//			//KineAll.initLL[2] = KineAll.CrdAll->data[20];
//
//			//pX = KineAll.initLL[0];
//			//pY = KineAll.initLL[1];
//			//pZ = KineAll.initLL[2];
//
//			//rpX = KineAll.EuAngLL[0]/3.14159265*180.;
//			//rpY = KineAll.EuAngLL[1]/3.14159265*180.;
//			//rpZ = KineAll.EuAngLL[2]/3.14159265*180.;
//		}
//	}
//	//RMotion2();
//}
//
//
////void COpenGLControl::SCA()
////{
////	if( !breakflag )
////	{
////		RArm.ResetTheta(finalInput);
////		for(int i = 0 ; i < 6 ; i++) 
////			RAini_theta[i] = finalInput[i];
////
////		if( MotorAction)
////		{
////			float *Command;
////			Command = new float [6];
////			for(int i = 0 ; i < 6 ; i++)
////			{
////				if(i > 1)
////					Command[i] = finalInput[i];
////				else
////					Command[i] = -finalInput[i] ;
////			}
////
////			DSP1.WriteCommand(Command, 1);
////			DSP2.WriteCommand(Command, 2);
////			DSP3.WriteCommand(Command, 3);
////		}
////	}
////	//toc();
////}
//
//void COpenGLControl::RMotion2()
//{
//	//printf("RMotion2222222\n");
//	if(Rcount >= 0 && Rcount < RActionPath.size())// && catching->goflag)
//	{
//		//working = true;
//		//printf("working\n");
//
//		RArm.ResetTheta(RActionPath[Rcount].Pos);
//		//RArm.ClawTheta(RActionPath[Rcount].Pos[6]);
//
//		//printf("RActionPath[Rcount].Pos[6] = %f\n",RActionPath[Rcount].Pos[6]);
//
//
//		for(int i = 0 ; i < 7 ; i++) // Reset ¤âÁu initial position
//		{
//			RAini_theta[i] = RActionPath[Rcount].Pos[i];
//			//printf("RAini_theta[%d] = %f\n", i, RAini_theta[i]);
//		}
//		//printf("[0]%f [1]%f [2]%f [3]%f [4]%f [5]%f\n", RAini_theta[0], RAini_theta[1], RAini_theta[2], RAini_theta[3], RAini_theta[4], RAini_theta[5]);
//		//printf("%d\n",MotorAction);
//		if( MotorAction /*&& goflag*/ )
//		{
//			//printf("Motor\n");
//			float *Command;//, *Encoder;
//			Command = new float [7];
//			//Encoder = new float [7];
//			for(int i = 0 ; i < 7 ; i++)
//			{
//				//if(i > 1)
//					Command[i] = RActionPath[Rcount].Pos[i];
//				//else
//				//	Command[i] = -RActionPath[Rcount].Pos[i];
//			}
//			//Command[6] = RClawTheta;
//			//Command[8] = Timer+12;			
//			
//			//DSP.Write_and_Read(Command,Encoder);
//			//rTimer.StartTimer();
//			
//			#if MOTOR_MODE
//			DSP1.WriteCommand(Command,1);
//			DSP2.WriteCommand(Command,2);
//			DSP3.WriteCommand(Command,3);
//			DSP4.WriteCommand(Command, 4);
//			DSP5.WriteCommand(Command, 5);
//			DSP6.WriteCommand(Command, 6);
//			#endif
//
//			//rTimer.StopTimer();
//			//printf("Planning  time: %f ms\n",rTimer.GetElapsedTime());
//			//Encoder[0] = Command[0];
//
//			//Command_Record.push_back(Command);
//			
//			//Encoder_Record.push_back(Encoder);			
//			//CurRArmTheta = Encoder;
//		}
//
//		Rcount++;
//	}
//
//}
//
//void COpenGLControl::ResetRArm()
//{
//	cout<<"Reset Arm.."<<endl;
//
//	double delta_theta[7] = {0};
//	breakflag = true;
//	
//	for (int i=0;i<6;i++)
//		delta_theta[i] = -1*(RAini_theta[i]/500.);
//
//	RActionPath.clear();
//	//indwrite = 0;
//	Rcount = 0;
//
//	for(int i = 1; i < 501; i++)
//	{
//		//printf("RClawTheta = %f\n",RClawTheta);
//		Dimension theta;
//		theta.Pos = new float[7];
//
//		for(int j = 0; j < 6; j++)
//			theta.Pos[j] = RAini_theta[j]+i*delta_theta[j];
//		
//		//theta.Pos[6] = RClawTheta;
//
//		RActionPath.push_back(theta);
//	}
//
//	//Kinematics initialization
//	KineAll.InitKineTrains();
//	KineAll.FindFK();
//	////KineAll.FindCOG();
//	KineAll.ComputeJacobians();
//
//	KineAll.GetLegsCoords();
//
//	KineAll.ComputeEulerAng(KineAll.LLegRotM, KineAll.EuAngLL);
//
//	KineAll.initLL[0] = KineAll.CrdAll->data[18];
//	KineAll.initLL[1] = KineAll.CrdAll->data[19];
//	KineAll.initLL[2] = KineAll.CrdAll->data[20];
//
//	pX = KineAll.initLL[0];
//	pY = KineAll.initLL[1];
//	pZ = KineAll.initLL[2];
//
//	rpX = KineAll.EuAngLL[0]/3.14159265*180.;
//	rpY = KineAll.EuAngLL[1]/3.14159265*180.;
//	rpZ = KineAll.EuAngLL[2]/3.14159265*180.;
//
//	SetTimer(7,Timer,NULL);
//}
