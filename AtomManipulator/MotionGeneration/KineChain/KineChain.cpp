#include "MotorControl/MotorControlFactory.h" // should be placed before KineChain.h
#include "KineChain.h"


namespace motion
{
	/////////////////////////////
	//----Member functions----///
	/////////////////////////////
	KineChain::KineChain(void)
		: t_base(NULL),
		nonempty_link(NULL)
	{
	}
	
	KineChain::~KineChain(void)
	{
		delete [] q; 
		delete [] qd;
		delete [] frame;
		delete [] DH;
		delete [] Jac_k;
		delete [] nonempty_link;

		if (t_base != NULL)
			delete [] t_base;
	}

	void KineChain::InitKineChain(int dof)
	{
		DOF = dof;

		// joint
		q = new double [DOF]; 
		Mat_zeros(q, DOF);
		qd = new double [DOF]; 
		Mat_zeros(qd, DOF);

		// frame
		frame = new double [12 * (DOF+1) +3 ]; // 3 elements used as cache
		f_o = frame; 
		f_p = frame + 9 * (DOF+1) +3 ;	
		Mat_eye(f_o,3,3); // Initialize the orientation of the base
		Mat_zeros(f_p,3); // Initialize the position of the base

		// DH 
		DH = new double [6*DOF];

		DH_alpha = DH       ;
		DH_sa    = DH +   DOF; // sin(alpha)
		DH_ca    = DH + 2*DOF; // cos(alpha)
		DH_theta = DH + 3*DOF;
		DH_a	 = DH + 4*DOF;
		DH_d     = DH + 5*DOF;

		Mat_zeros(DH, 6*DOF);

		// the Jacobian of frame k-1
		Jac_k = new double[DOF*6]; 
		nonempty_link = new int [DOF];
	}

	void KineChain::setDH (double *DH_a, double *DH_alpha, double *DH_d, double *DH_theta)
	{//  in m 
		Mat_cpy(this->DH_alpha, DH_alpha, DOF);
		Mat_cpy(this->DH_theta, DH_theta, DOF);		
		Mat_cpy(this->DH_a,     DH_a,     DOF);
		Mat_cpy(this->DH_d,     DH_d,     DOF);

		for (int i =0; i<DOF; ++i)
		{
			DH_ca[i] = cos(DH_alpha[i]);
			DH_sa[i] = sin(DH_alpha[i]);
		}

		Init_physical_link();

		FwdKine();
	}

	void KineChain::setDH_txt (std::string file_name)
	{
		fstream DH_file;
		DH_file.open(file_name.c_str(),ios::in);
		if (DH_file.is_open()) 
		{
			for(int i=0;i<DOF;i++)
				DH_file>>DH_d[i] ;	

			for(int i=0;i<DOF;i++)
				DH_file>>DH_a[i] ;	

			for(int i=0;i<DOF;i++)
				DH_file>>DH_theta[i]; // theta

			for(int i=0;i<DOF;i++)
				DH_file>>DH_theta[i]; // theta_home

			for(int i=0;i<DOF;i++)
				DH_file>>DH_alpha[i];
	
			for(int i=0;i<DOF;i++)
			{
				DH_d[i] = DH_d[i]/ 1000;
				DH_a[i] = DH_a[i]/ 1000; // convert into m
		
				DH_theta[i] *= PI/180;
				DH_alpha[i] *= PI/180;		
				DH_ca[i] = cos(DH_alpha[i]);
				DH_sa[i] = sin(DH_alpha[i]);
			}
			DH_file.close();

			Mat_Show(DH, DOF, 6, "DH");
	
			Init_physical_link();

			FwdKine();
		}
		else 
			printf("Error loading DH file");
	}

	void KineChain::Init_physical_link(void)
	{
		N_nonempty_link = 1;
		nonempty_link[0] = 0;
		for (int i =0; i<DOF; ++i)
		{
			if ( DH_a[i] !=0 || DH_d[i] != 0)
			{
				nonempty_link[N_nonempty_link] = i+1; // f_p+3*(i+1) 
				++N_nonempty_link;
			}
		}
		--N_nonempty_link;
	}

	void KineChain::get_Jac_n_dot (double *Jac, double *Jac_dot, int n, bool half, double *pn )
	{// get the Jacobian dot of the nth frame
	
		int size_jac = half? 3*DOF: 6*DOF;

		if ( n < 1)
		{
			Mat_zeros(Jac, size_jac); 
			Mat_zeros(Jac_dot, size_jac); 
		}	
		else
		{	
			if (pn==NULL)
				pn = f_p+3*n; // position of frame n

			// Initialization
			double tn [6]; // Cartesian-space twist of frame n (Jac * qd)		
			if (half)
			{
				get_Jac_p(Jac, n, pn);
				Mat_Ax_y(Jac, 3, n, qd, tn); // compute the product of first n column since others are 0
			}
			else
			{
				get_Jac(Jac, n, pn); // the Jacobian of frame n		
				Mat_Ax_y(Jac, 6, n, qd, tn); // compute the product of first n column since others are 0
			}
			Mat_zeros(Jac_dot, size_jac); 					
		
			// Compute Jacobian dot
			for ( int i = 0 ; i < n; ++i) // compute the columns of Jac_dot
				jacodot(qd, tn, pn, f_p+3*i, f_o+9*i+6, Jac_dot+6*i, i+1, half); // only the first three elements of tn is needed
		}
	}

	void KineChain::jacodot(double *qd, double *tn, double *pn, double *pk, double *zk, double* j_dot_c, int k, bool half)
	{
		// Compute the kth column of the Jacobian_dot of the nth frame. 
    
		//  k - the index of the column
		// qd - joint velocity
		// tn - twist of frame n (J * qd)
		// pn - position of frame n
		// pn - position of frame k-1
		// zk - z-axis of frame k-1
		// half - compute the upper half of the column
	
		// output 
		// j_dot_c - the kth column of Jacobian_dot

		if (k==1 && t_base==NULL)
		{// base twist is zero
			c_prod(zk, tn, j_dot_c); // compute the 1st column of Jac_dot (z0 x xdot)
		}
		else
		{		
			double tk [6] = {0}; // twist of frame k-1
			if (k==1)
				Mat_cpy(tk, t_base,6);
			else
			{
				get_Jac(Jac_k, k-1); // compute the Jacobian of frame k-1
				Mat_Ax_y(Jac_k, 6, k-1, qd, tk); // compute the product of first i column since others are 0		

			}
	
			double zk_dot [3] = {0}; // z-axis dot of frame k-1
			c_prod(tk+3, zk, zk_dot); // z_k-1_dot = (Jac_k_qd_omega x zk)		

			// position difference between frame n and frame k-1
			double p_diff[3]; 
			p_diff[0] = pn[0] - pk[0];
			p_diff[1] = pn[1] - pk[1];
			p_diff[2] = pn[2] - pk[2];

			// linear velocity difference between frame n and frame k-1
			double t_diff [3];
			t_diff[0] = tn[0] - tk[0];
			t_diff[1] = tn[1] - tk[1];
			t_diff[2] = tn[2] - tk[2];


			// j_dot_c = zk_dot x p_diff + zk x Jac_qd_diff  (for position)
			Mat_zeros(j_dot_c, 3);
			c_prod( zk_dot,  p_diff, j_dot_c, 1);
			c_prod(     zk,  t_diff, j_dot_c, 1);		
		
			if (!half) // j_dot_c = zk_dot (for orientation)
				Mat_cpy(j_dot_c+3, zk_dot, 3);			

		}
	}

	void KineChain::FwdKine(void)
	{// update f_o and f_p with q

		double rel [12]; // 3*4  [R12 p12]	
		for (int n=1; n<=DOF; ++n)
		{	
			DH_tranf(n, rel, rel+9); // relative orientation and position of the nth frame w.r.t.the n-1th frame				
			Mat_AB_C (f_o+9*(n-1), 3, 3, rel, 3, 4, f_o+9*n); // [R2_tmp p2_tmp] = R1 * [R12 p12]		
			Mat_xy_plu (f_o+9*n+9, f_p+3*(n-1), f_p+3*n, 3) ; //  p2 = p2_tmp + p1	(p2_tmp is stored in f_o+9*n+9)		
		}

		std::shared_ptr<motor::MotorController> _motors = motor::MotorControlFactory::
			GetInstance().GetCurrentMotorController();
		if (_motors)
		{
			if (_motors->isThreadCreated())
				_motors->SetMotorInput((float *)q);
		}
	}

	void KineChain::compute_Euler_ang(double* RotM, double* EulerAng)
	{// Column-major version


		// Euler angle -- Matlab code
		//function [thetaXYZ] = FindAngXYZ(R)
		//% please see
		//% http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToEuler/index.htm
		//% R = [xc yc zc] in the world  xc yc zc are all 3x1 column 


		if (RotM[1] > 0.998) //if (R(2,1) > 0.998) % singularity at north pole
		{		
			EulerAng[0] = 0; 	//	thetaXYZ(1,1) = 0;
			EulerAng[1] = atan2(RotM[6],RotM[8]); //	thetaXYZ(2,1) = atan2(R(1,3),R(3,3));
			EulerAng[2] = 1.5707963; // thetaXYZ(3,1) = pi/2;
		}
		else if (RotM[1] < -0.998) //(R(2,1) < -0.998) % singularity at south pole
		{
			EulerAng[0] = 0; // thetaXYZ(1,1) = 0; 
			EulerAng[1] = atan2(RotM[6],RotM[8]); // thetaXYZ(2,1) = atan2(R(1,3),R(3,3));		
			EulerAng[2] = -1.5707963; // thetaXYZ(3,1) = -pi/2; 
		}
		else
		{	
			EulerAng[0] = atan2(-RotM[7],RotM[4]);//	thetaXYZ(1,1) = atan2(-R(2,3),R(2,2));
			EulerAng[1] = atan2(-RotM[2],RotM[0]);//	thetaXYZ(2,1) = atan2(-R(3,1),R(1,1));
			EulerAng[2] = asin(RotM[1]); //	thetaXYZ(3,1) = asin(R(2,1));
		}

	}

	void KineChain::draw_stick(void)
	{
		if (N_nonempty_link > 0)
		{
			// Robot sticks
			for (int i=0; i<N_nonempty_link; ++i )
			{
				double *v_st = f_p + 3 * nonempty_link[i];
				double *v_ed = f_p + 3 * nonempty_link[i+1];


				glPushMatrix();
				glTranslatef( v_st[0], v_st[1], v_st[2]);
				glPushMatrix();
				glColor3f( 1.0, 0.3, 0 ); // color of the joint
				glutSolidSphere(0.02,10,10);
				glPopMatrix();
				glPopMatrix();

				glLineWidth(5.0); 
				glColor3f(0.8, 0.8, 1.0); // color of link
				glBegin(GL_LINES);
				glVertex3f( v_st[0], v_st[1], v_st[2]);			
				glVertex3f( v_ed[0], v_ed[1], v_ed[2]);
				glEnd();

			}	

			// End-effector orientation


			get_Trans(T_end, DOF);

			glPushMatrix();		 
			glMultMatrixf(T_end);

			GLUquadricObj *quadratic = gluNewQuadric();
			float LINK_RADIUS  = 0.0025;
			float LINK_LENGTH = 0.1;
			glEnable(GL_COLOR_MATERIAL);

			// z axis
			glPushMatrix();		
			glColor3f(0.8,0,0); 
			gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,20,32);
			glTranslatef(0, 0,LINK_LENGTH);
			glutSolidCone(0.008,0.012,10,10);
			glPopMatrix();

			// y axis
			glPushMatrix();
			glColor3f(0,0.8,0);		
			glRotatef(-90,1,0,0);
			gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,20,32);
			glTranslatef(0, 0 ,LINK_LENGTH);
			glutSolidCone(0.008,0.012,10,10);
			glPopMatrix();

			// x axis
			glPushMatrix();
			glColor3f(0,0,0.8);	
			glRotatef(90,0,1,0);
			gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,20,32);
			glTranslatef(0, 0,LINK_LENGTH);
			glutSolidCone(0.008,0.012,10,10);
			glPopMatrix();

			glDisable(GL_COLOR_MATERIAL);

			glPopMatrix();		 
		}
	}

	/*-----SET ----*/
	void KineChain::set_q (double *q)
	{// set the first l elements
		memcpy(this->q,q,DOF*s_ele); 
		FwdKine();
	}
	void KineChain::set_q (double *q, int l)
	{// set the first l elements
		memcpy(this->q,q,l*s_ele); 
		FwdKine();
	}
	void KineChain::set_qd (double *qd) 
	{memcpy(this->qd,qd,DOF*s_ele);}
	void KineChain::set_base (const double* o, const double *p) 
	{
		memcpy(f_o, o, 9*s_ele); 
		memcpy(f_p, p, 3*s_ele); 
		FwdKine();
	}
	void KineChain::set_base_twist (double *t_base)
	{	memcpy(this->t_base, t_base, 6*s_ele);	}

	/*-------get-----------*/
	void KineChain::get_q (double* q) 
	{ memcpy(q, this->q, DOF*s_ele); };
	void KineChain::get_f_orie (double * orie, int n) 
	{ memcpy(orie, f_o+9*n, 9*s_ele);} 
	void KineChain::get_f_orie (double * orie)
	{ memcpy(orie, f_o, 9*DOF*s_ele);} 
	void KineChain::get_f_pos (double *zaxis, int n)
	{ memcpy(zaxis, f_p+3*n, 3*s_ele); };
	void KineChain::get_f_pos (double *zaxis)
	{ memcpy(zaxis, f_p, 3*DOF*s_ele); };
	void KineChain::get_Euler (double *EulerAngle, int n)
	{compute_Euler_ang(f_o+9*n, EulerAngle);}
	void KineChain::get_Trans(double  *T, int n )
	{
		double *f_o_n = f_o+9*n;
		memcpy(T,      f_o_n, 3*s_ele);
		memcpy(T+4,  f_o_n+3, 3*s_ele);
		memcpy(T+8,  f_o_n+6, 3*s_ele);
		memcpy(T+12, f_p+3*n, 3*s_ele);
		T[3] = 0;
		T[7] = 0;
		T[11] = 0;
		T[15] = 1;
	}
	void KineChain::get_Trans(float  *T, int n )
	{
		double *f_o_n = f_o+9*n;
		for(int i=0; i<3; ++i)
		{
			T[i] = (float) f_o_n[i];
			T[i+4] = (float) f_o_n[i+3];
			T[i+8] = (float) f_o_n[i+6];
			T[i+12]= (float) f_p[3*n + i];
		}	
		T[3] = 0;
		T[7] = 0;
		T[11] = 0;
		T[15] = 1;
	}
	void KineChain::get_Jac  (double *Jac, int n,  double *pn) // get the Jacobian of frame n
	{// the position of frame n of interest (w.r.t. base)
		if (pn==NULL)
			pn = f_p+3*n;
		Mat_zeros(Jac+6*n, 6*(DOF-n));		
		for (int i =0; i<n; ++i)	
			jaco(pn, f_p+3*i, f_o+9*i+6, Jac+6*i, false);					
	}
	void KineChain::get_Jac_p (double *Jac, int n,  double *pn ) // get the Jacobian of position of frame n
	{
		if (pn==NULL)
			pn = f_p+3*n; // position of frame n		
		Mat_zeros(Jac+3*n, 3*(DOF-n));		
		for (int i =0; i<n; ++i)	
			jaco(pn, f_p+3*i, f_o+9*i+6, Jac+3*i, true);
	}
	void KineChain::get_Jac_o (double *Jac, int n) // get the Jacobian of orientation of frame n
	{
		Mat_zeros(Jac, 3*DOF);
		for (int i =0; i<n; ++i)	
			Mat_cpy( Jac+3*i, f_o+9*i+6, 3);
	}

	/*-----elementary functions-----*/
	void KineChain::DH_tranf(const int n, double*o, double*p)
	{// the orientation and the position of the nth frame w.r.t. the n-1th frame
		double qn = q[n-1] + DH_theta[n-1] ;
		double cq = cos(qn); 
		double sq = sin(qn);	

		o[0] = cq;	o[3] = -sq*DH_ca[n-1];	o[6] =  sq*DH_sa[n-1];	
		o[1] = sq;	o[4] =  cq*DH_ca[n-1];	o[7] = -cq*DH_sa[n-1];	
		o[2] = 0;	o[5] =     DH_sa[n-1];  o[8] =     DH_ca[n-1]; 		

		p[0] = DH_a[n-1] * cq;
		p[1] = DH_a[n-1] * sq;
		p[2] = DH_d[n-1];
	}

	void KineChain::jaco(double *pn, double *pk, double *zk, double* j_c, bool half)
	{
		// Compute the kth column of the Jacobian of the nth frame. 

		// pn - position of frame n (i.e. f_p+3*n)
		// pk - poisiton of frame k-1 (i.e. f_p+3*(k-1) )
		// zk - zaxis of frame k-1 (i.e. f_o+9*(k-1)+6 )
		// j_c - a 6-by-1 vector if half == false; a 3-by-1 vector if half == true;
		// if half == true, it computes only the Jacobian of position

		double p_diff[3]; // difference between the position 	

		// position difference between frame n and frame k-1
		p_diff[0] = pn[0] - pk[0];
		p_diff[1] = pn[1] - pk[1];
		p_diff[2] = pn[2] - pk[2];

		// cross product: z x p_diff	
		j_c[0] = -zk[2] * p_diff[1] + zk[1]* p_diff[2];
		j_c[1] =  zk[2] * p_diff[0] - zk[0]* p_diff[2];
		j_c[2] = -zk[1] * p_diff[0] + zk[0]* p_diff[1];

		if (!half)
			Mat_cpy(j_c+3, zk, 3);		
	}

	void KineChain::c_prod (double* a, double* b, double* c, double beta) // c = a x b + beta*c
	{
		// Compute the cross product: c = a x b
		c[0] = -a[2] * b[1] + a[1] * b[2] + beta*c[0];
		c[1] =  a[2] * b[0] - a[0] * b[2] + beta*c[1];
		c[2] = -a[1] * b[0] + a[0] * b[1] + beta*c[2];
	}
}/* namespace motion */