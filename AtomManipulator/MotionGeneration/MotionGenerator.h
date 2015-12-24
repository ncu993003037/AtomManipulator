#pragma once
#ifndef _MOTIONGENERATOR_
#define _MOTIONGENERATOR_ 

#include <time.h>
#include <Windows.h>
#include "MotionGeneration/KineChain/KineChain.h"

namespace motion
{
	typedef struct {
		int _DOF_;
		int _sleep_time_micro_sec_;
	} gConfig;

	class USVt
	{
	public:
		USVt( Mat_int Jrow, Mat_int Jcol, double eps = 1e-6):
		  m(Jrow), n(Jcol), r(0), _m(Jrow), _n(Jcol)
		  {
			  k = min(m, n);
			  _k = k;

			  J = new double [_m*_n];
			  Jdot = new double [_m*_n];
			  U = new double [_m*_k];
			  S = new double [_k];
			  Sinv = new double [_k];
			  Vt = new double [_k*_n];						
		  }


		  ~USVt(void)
		  {
			  delete [] J;
			  delete [] Jdot;
			  delete [] U; 
			  delete [] S; 
			  delete [] Sinv; 
			  delete [] Vt; 
		  }


		  void svd(void)
		  {// update U, S, Vt
			  Mat_svd(J, m, n, U, S, Vt); 
			  r = 6; // update rank
			  for(int i=0; i<k; ++i)
				  if (S[i]<thre) 
				  {
					  r = i; 
					  break;
				  }
		  }

		  void set_m(int m_new)
		  {
			  m = m_new;
			  k = min(m, n);
			  if (m > _m)
			  {
				  _m = m;			
				  delete [] J; 
				  delete [] Jdot; 			
				  delete [] U;

				  J = new double [_m*_n];
				  Jdot = new double [_m*_n];			
				  U = new double [_m*_k];
			  }		
			  if (k > _k)
			  {
				  _k = k; 
				  delete [] U; 
				  delete [] S; 
				  delete [] Sinv; 
				  delete [] Vt; 

				  U = new double [_m*_k];
				  S = new double [_k];
				  Sinv = new double [_k];
				  Vt = new double [_k*_n];
			  }
		  }

		  void set_n(int n_new)
		  {
			  n = n_new;
			  k = min(m, n);
			  if (n > _n)
			  {
				  _n = n;			

				  delete [] J; 
				  delete [] Jdot; 			
				  delete [] Vt;

				  J = new double [_m*_n];
				  Jdot = new double [_m*_n];			
				  Vt = new double [_k*_n];		

			  }		
			  if (k > _k)
			  {
				  _k = k; 
				  delete [] U; 
				  delete [] S; 
				  delete [] Sinv; 
				  delete [] Vt; 

				  U = new double [_m*_k];
				  S = new double [_k];
				  Sinv = new double [_k];
				  Vt = new double [_k*_n];
			  }
		  }

		  double *J;
		  double *U;
		  double *S;
		  double *Vt; 

		  double *Jdot;
		  double* Sinv;
		  Mat_int k;
		  Mat_int r;
		  Mat_int m;
		  Mat_int n;
		  double thre;

	private:
		Mat_int _m; // used to store the size of allocated array
		Mat_int _n;
		Mat_int _k;
	};
	////////////////////////////////////////////////////////////////////////////////////////

	class MotionGenerator
	{
	public:
		explicit MotionGenerator(const gConfig &config);
		virtual ~MotionGenerator();
		
		void Launch();
		bool Shutdown();

		int GetThreadOpened() const;

		void set_p_r (double *p_r);
		void set_pd_r (double *pd_r);
		void set_pdd_r (double *pdd_r);

		void set_o_r (double *o_r);
		void set_od_r (double *od_r);
		void set_odd_r (double *odd_r);

		void set_q_r (double *q_r);
		void set_qd_r (double *qd_r);
		void set_qdd_r (double *qdd_r);

		void set_BK_p(double damping, double bandwidth);
		void set_BK_o(double damping, double bandwidth);
		void set_BK_q(double damping, double bandwidth);

		void set_ik_control_mode(bool switcher);

	protected:
		virtual void IKControlLoop() = 0;
		virtual void JointControlLoop() = 0;

	protected:
		USVt Jac;

		// End-effector
		double *o_n_9; // orientation
		double *p_n; // position

		// Cartesian space	
		// reference
		double p_r[3], pd_r[3], pdd_r[3];	
		double o_r[4], od_r[3], odd_r[3]; // o_r is stored in quaternion	
		double _p_err, _o_err; // for monitoring

		// Joint space
		// reference
		double *q_r, *qd_r, *qdd_r;
		// state
		double *q; // linked to robot
		double *qd; // linked to robot
		double *qdd;

		double _q_err; //for monitoring

		double *q_err, *qd_err;

		double delta_t; // control time interval
		const int DOF;

		double eps;

		// Gain
		double Bp, Kp, Bo, Ko;	
		double Bq, Kq;

	protected:
		void Qua_rot2qua(double *rot, double *qua);
		void Qua_qua2rot(double *qua, double *rot);
		void Qua_pro_qp(double *q, double *p, double *r);
		void Qua_pro_qbarp(double *q, double *p, double *r);
		void Qua_pro_qpbar(double *q, double *p, double *r);

		void int_vel(void);
		void int_acc(void);

	private:
		// Thread in class (WINAPI) or you can use boost thread
		static DWORD WINAPI StaticThreadOpen(void* Param);
		DWORD WINAPI Run();

		void inline usleep(__int64 usec); // for WINAPI

	private:
		HANDLE	_gThreadHandle; // Motor control thread handle
		DWORD	_gThreadID; //  Motor control thread ID
		bool	_gThreadOpened; // 指示手臂部控制thread是否打開
		bool	_gThreadLife; //  Motor control 是否工作旗標

		LARGE_INTEGER _StartTime;
		LARGE_INTEGER _CurrentTime;
		LARGE_INTEGER _nFreq;
		LARGE_INTEGER _ElapsedMicroseconds;

		const gConfig _config;

		bool ik_flag;
	};
}/* namespace motion */
#endif