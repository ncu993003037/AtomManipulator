#pragma once

#ifndef _KINECHAIN_
#define _KINECHAIN_ 


#include "MotionGeneration/Math/MatLib.h"
#include <fstream>
#include <string>
#include "OpenGL_Model/GL/glut.h"

// Notations:
// frame 0 is the base frame of the kinematic chain
// link n is connected with frame n-1 and frame n
// the relative position and orientation of frame n w.r.t. frame n-1 is determined by qn (i.e q[n-1])

#ifndef PI
#define PI 3.14159265359
#endif

namespace motion
{
	class KineChain
	{
	public:
		KineChain(void); // initialize with zero DH parameter	
		~KineChain(void);

		static KineChain& GetInstance() // Singleton
		{
			static KineChain _instance;
			return _instance;
		}

		void InitKineChain(int dof);

		// Set Status (and perform FK) function
		void set_base (const double* o, const double *p);
		void set_base_twist (double *t_base);

		void setDH (double *DH_a, double *DH_alpha, double *DH_d, double *DH_theta);
		void setDH_txt (std::string file_name);

		void set_q (double *q);
		void set_q (double *q, int l);
		void set_qd (double *qd); 	

		// Get Status
		void get_q (double* q);

		void get_f_orie (double * orie, int n);	
		void get_f_orie (double * orie); 

		void get_f_pos (double *zaxis, int n);
		void get_f_pos (double *zaxis);

		void get_Euler (double *EulerAngle, int n);
		void get_Trans(double  *T, int n );
		void get_Trans(float  *T, int n );

		void get_Jac  (double *Jac, int n,  double *pn = NULL);
		void get_Jac_p (double *Jac, int n,  double *pn = NULL);
		void get_Jac_o (double *Jac, int n);
		
		void get_Jac_n_dot (double *Jac, double *Jac_dot, int n, bool half = false, double *pn = NULL); // get the Jacobian and Jacobian dot of frame n
		
		void compute_Euler_ang(double* RotM, double* EulerAng);	

		void FwdKine (void); // update f_o and f_p with q	
		
		// Draw
		void draw_stick(void); 

	private:
		void c_prod (double* a, double* b, double* c, double beta = 0); // c = a x b	
		void jaco (double *pn, double *pk, double *zk, double* j_c, bool half); // compute the kth column of the Jacobian of the frame n 
		void DH_tranf (int n, double*o, double*p);

		void jacodot(double *qd, double *tn, double *pn, double *pk, double *zk, double* j_dot_c, int k, bool half);
	
		// Draw
		void Init_physical_link(void);

	private:
		int DOF;
		int N_nonempty_link;
		int *nonempty_link;
		double *frame, *f_o, *f_p; // orientation and position of each frame w.r.t base	    

		// members
		double *DH, *DH_a, *DH_alpha, *DH_d, *DH_theta; 
		double *DH_ca, *DH_sa; // cos(alpha), sin(alpha)
	
		// f_o is a 3-by-3*(DOF+1) matrix; f_p is a 3-by-(DOF+1) matrix
		// the first set denotes that of the base

		double *q, *qd; // current angle 
		double *t_base; // twist of base

		// cache
		double *Jac_k; // temp_obj
		float T_end [16]; // Transformation matrix of end-effector (for GL)
	};
}/* namespace motion */

#endif