#include "MotionControl/MotionController.h"



namespace motion
{
	namespace control
	{
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
		}; // class USVt
		/////////////////////////////////////////////////////

		MotionController::MotionController()
			: Bp(1), Kp(1), Bo(1), Ko(1), Bq(1), Kq(1),
				//q(robot_->q), qd(robot_->qd), 
			//	//o_n_9 (robot_.f_o + 9 *robot_.DOF),
			//	//p_n(robot_.f_p + 3 * robot_.DOF),
			//	//Jac(6, robot_.DOF, 1e-9),
				delta_t(50e-6), // simulation sampling time
				eps(1e-3) // damping for singular value
		{
			qdd = new double [DOF];
			Mat_zeros(qdd, DOF);

			q_r = new double [DOF];
			qd_r = new double [DOF];
			qdd_r = new double [DOF];	
			Mat_zeros(q_r, DOF);
			Mat_zeros(qd_r, DOF);
			Mat_zeros(qdd_r, DOF);

			//set_p_r(Robot.f_p+3*DOF);
			//Mat_zeros(pd_r,3);
			//Mat_zeros(pdd_r,3);
			//set_o_r(Robot.f_o+9*DOF);
			//Mat_zeros(od_r,3);
			//Mat_zeros(odd_r,3);

			// private members
			q_err = new double [DOF];
			qd_err = new double [DOF];
		}

		MotionController::~MotionController(void)
		{
		}
	}/* namespace control */
}/* namespace motion */