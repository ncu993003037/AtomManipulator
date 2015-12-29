#include "MotionGenerator.h"
#include <iostream>


namespace motion
{
	MotionGenerator::MotionGenerator(const gConfig &config)
		: _config(config),
		Bp(1), Kp(1), Bo(1), Ko(1), Bq(1), Kq(1),
		DOF(config._DOF_),
		Jac(6, config._DOF_, 1e-9),
		_mThreadHandle(NULL),
		_mThreadCreated(false),
		ik_flag(false),
		_mStopCalling(true),
		eps(1e-3) // damping for singular value
	{
		delta_t = _config._sleep_time_micro_sec_ / 1e6;

		o_n_9 = new double [9];
		p_n = new double [3];
		KineChain::GetInstance().get_f_orie(o_n_9, DOF);
		KineChain::GetInstance().get_f_pos(p_n, DOF);

		q = new double [DOF];
		qd = new double [DOF];
		KineChain::GetInstance().get_q(q);
		Mat_zeros(qd, DOF);

		qdd = new double [DOF];
		Mat_zeros(qdd, DOF);

		q_r = new double [DOF];
		qd_r = new double [DOF];
		qdd_r = new double [DOF];	
		Mat_zeros(q_r, DOF);
		Mat_zeros(qd_r, DOF);
		Mat_zeros(qdd_r, DOF);

		KineChain::GetInstance().get_f_pos(p_r, DOF);
		Mat_zeros(pd_r, 3);
		Mat_zeros(pdd_r, 3);
		KineChain::GetInstance().get_f_orie(o_r, DOF);
		Mat_zeros(od_r, 3);
		Mat_zeros(odd_r, 3);

		q_err = new double [DOF];
		qd_err = new double [DOF];

		printf("Info [Motion]: Create Motion Generation API \n");
	}

	MotionGenerator::~MotionGenerator()
	{
		_mStopCalling = true;
		if (isThreadCreated())
			Shutdown();	
		printf("Info [Motion]: Motion Generation API destroyed \n");
	}

	void MotionGenerator::Launch()
	{
		QueryPerformanceFrequency(&_nFreq); 

		if (!_mThreadCreated)
		{
			_mThreadLife = true;
			_mThreadHandle = CreateThread(NULL, 0, CreateStaticThread, (void*) this, 0, &_mThreadID );
			_mThreadCreated = true;
			_mStopCalling = false;
			SetPriorityClass(_mThreadHandle, REALTIME_PRIORITY_CLASS);
			SetThreadPriority(_mThreadHandle, THREAD_PRIORITY_TIME_CRITICAL);
			SetThreadAffinityMask(_mThreadHandle, 1 << 0);
			printf("Info [Motion]: Thread opened\n");
		}
	}

	bool MotionGenerator::Shutdown()
	{
		if (_mThreadLife && _mThreadCreated)
		{
			_mThreadLife = false; 

			Sleep(500);

			if ( NULL != _mThreadHandle )
			{
				DWORD ret = WaitForSingleObject( _mThreadHandle, INFINITE);
				CloseHandle( _mThreadHandle );
				_mThreadCreated = false;
			}

			printf("Info [Motion]: Thread has been closed\n");
		}
		else printf("Warning [Motion]: Thread already dead\n");

		return true;
	}

	DWORD WINAPI MotionGenerator::CreateStaticThread(void* Param_)
	{
		MotionGenerator *pthis = (MotionGenerator *) Param_;
		return pthis->Run();
	}

	DWORD WINAPI MotionGenerator::Run()
	{
		QueryPerformanceCounter(&_StartTime);

		while (_mThreadLife)
		{
			QueryPerformanceCounter(&_CurrentTime);
			_ElapsedMicroseconds.QuadPart = _CurrentTime.QuadPart - _StartTime.QuadPart;
			_ElapsedMicroseconds.QuadPart *= 1000000; // Micro-seconds
			_ElapsedMicroseconds.QuadPart /= _nFreq.QuadPart;

			//printf("Info [Motion]: _ElapsedMicroseconds = %d\n",_ElapsedMicroseconds.QuadPart);
			QueryPerformanceCounter(&_StartTime);

			if (!_mStopCalling)
			{
				KineChain::GetInstance().get_f_orie(o_n_9, DOF);
				KineChain::GetInstance().get_f_pos(p_n, DOF);

				// Do something
				if (ik_flag)
					IKControlLoop();
				else
					JointControlLoop();
				////////////////////////

				KineChain::GetInstance().set_q(q);
				KineChain::GetInstance().set_qd(qd);
				motion::KineChain::GetInstance().FwdKine();
			}

			usleep(_config._sleep_time_micro_sec_);
		}

		return 0;
	}

	int MotionGenerator::isThreadCreated() const {return _mThreadCreated;}

	inline void MotionGenerator::usleep(__int64 usec_) //for WINAPI
	{ 
		HANDLE timer; 
		LARGE_INTEGER ft; 

		ft.QuadPart = -(10*usec_); // Convert to 100 nanosecond interval, negative value indicates relative time

		timer = CreateWaitableTimer(NULL, TRUE, NULL); 
		SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
		WaitForSingleObject(timer, INFINITE); 
		CloseHandle(timer); 
	}


	void MotionGenerator::set_p_r (double *p_r)
	{	memcpy(this->p_r, p_r, s_ele*3); }
	void MotionGenerator::set_pd_r (double *pd_r)
	{	memcpy(this->pd_r, pd_r, s_ele*3); }
	void MotionGenerator::set_pdd_r (double *pdd_r)
	{	memcpy(this->pdd_r, pdd_r, s_ele*3); }

	void MotionGenerator::set_o_r (double *o_r)
	{// or is a rotation matrix
		Qua_rot2qua(o_r, this->o_r);
	}
	void MotionGenerator::set_od_r (double *od_r)
	{	memcpy(this->od_r, od_r, s_ele*3); }
	void MotionGenerator::set_odd_r (double *odd_r)
	{	memcpy(this->odd_r, odd_r, s_ele*3); }

	void MotionGenerator::set_q_r (double *q_r)
	{	memcpy(this->q_r, q_r, s_ele*DOF); }
	void MotionGenerator::set_qd_r (double *qd_r)
	{	memcpy(this->qd_r, qd_r, s_ele*DOF); }
	void MotionGenerator::set_qdd_r (double *qdd_r)
	{	memcpy(this->qdd_r, qdd_r, s_ele*DOF); }

	void MotionGenerator::set_ik_control_mode(bool switcher)
	{ ik_flag = switcher; }

	// Quaternion
	void MotionGenerator::Qua_qua2rot(double *q, double *r)
	{// world frame
		r[0] = q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3];
		r[1] = 2* ( q[1]*q[2] - q[0]*q[3]);
		r[2] = 2* ( q[1]*q[3] + q[0]*q[2]);

		r[3] = 2* ( q[1]*q[2] + q[0]*q[3]);
		r[4] = q[0]*q[0] - q[1]*q[1] + q[2]*q[2] - q[3]*q[3];
		r[5] = 2* ( q[2]*q[3] - q[0]*q[1]);

		r[6] = 2* ( q[1]*q[3] - q[0]*q[2]);
		r[7] = 2* ( q[2]*q[3] + q[0]*q[1]);
		r[4] = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
	}
	void MotionGenerator::Qua_rot2qua( double *r, double *q )
	{// body frame to world frame 
		double nor;
		double norm_q;

		if     ( (r[4] >= -r[8]) && (r[0] >= -r[4]) && (r[0] >= -r[8]) )
		{
			nor = sqrt( 1+r[0]+r[4]+r[8]  );
			q[0] = 0.5 * nor;
			q[1] = 0.5 * (r[5]-r[6]) / nor;
			q[2] = 0.5 * (r[6]-r[2]) / nor;
			q[3] = 0.5 * (r[1]-r[3]) / nor;		
		

		}
		else if ((r[4] <= -r[8]) && (r[0] >=  r[4]) && (r[0] >=  r[8]))
		{
			nor  = sqrt( 1+r[0]-r[4]-r[8]  );
			q[0] = 0.5 * (r[5]-r[7]) / nor;
			q[1] = 0.5 * nor;
			q[2] = 0.5 * (r[1]+r[3]) / nor;
			q[3] = 0.5 * (r[6]+r[2]) / nor;
		}
		else if ((r[4] >=  r[8]) && (r[0] <=  r[4]) && (r[0] <= -r[8]))
		{
			nor = sqrt( 1-r[0]+r[4]-r[8]  );
			q[0] = 0.5 * (r[6]-r[2]) / nor;
			q[1] = 0.5 * (r[1]+r[3]) / nor;
			q[2] = 0.5 * nor;
			q[3] = 0.5 * (r[5]+r[7]) / nor;
		}
		else if ((r[4] <=  r[8]) && (r[0] <= -r[4]) && (r[0] <=  r[8]))
		{
			nor = sqrt( 1-r[0]-r[4]+r[8]  );
			q[0] = 0.5 * (r[1]-r[3]) / nor;
			q[1] = 0.5 * (r[6]+r[2]) / nor;
			q[2] = 0.5 * (r[5]+r[7]) / nor;
			q[3] = 0.5 * nor;

		}
		else
		{
			cout<<"Conversion error "<<endl;
			Mat_Show(r,3,3,"RMatrix");
			system("pause");
		}
		norm_q = Mat_norm(q, 4);
		q[0] /= norm_q;
		q[1] /= norm_q;
		q[2] /= norm_q;
		q[3] /= norm_q;
	}
	void MotionGenerator::Qua_pro_qp(double *q, double *p, double *r)
	{// r = q * p

		r[0] = q[0]*p[0] - q[1]*p[1] - q[2]*p[2] - q[3]*p[3]; // q0*p0 - q13tp13
		r[1] = q[0]*p[1] + p[0]*q[1] + ( -q[3] * p[2] + q[2] * p[3]  ); // q0*p13 + p0*q13 + q13 x p13
		r[2] = q[0]*p[2] + p[0]*q[2] + (  q[3] * p[1] - q[1] * p[3]  ); 
		r[3] = q[0]*p[3] + p[0]*q[3] + ( -q[2] * p[1] + q[1] * p[2]  );
	}
	void MotionGenerator::Qua_pro_qbarp(double *q, double *p, double *r)
	{// r = qbar * p ( if norm(q) = 1, qbar = qinv)

		r[0] = q[0]*p[0] + q[1]*p[1] + q[2]*p[2] + q[3]*p[3]; // q0*p0 + q13tp13
		r[1] = q[0]*p[1] - p[0]*q[1] - ( -q[3] * p[2] + q[2] * p[3]  ); // q0*p13 - p0*q13 + q13 x p13
		r[2] = q[0]*p[2] - p[0]*q[2] - (  q[3] * p[1] - q[1] * p[3]  );
		r[3] = q[0]*p[3] - p[0]*q[3] - ( -q[2] * p[1] + q[1] * p[2]  ); 
	}

	void MotionGenerator::Qua_pro_qpbar(double *q, double *p, double *r)
	{// r = q * p

		r[0] =  q[0]*p[0] + q[1]*p[1] + q[2]*p[2] + q[3]*p[3]; // q0*p0 + q13tp13
		r[1] = -q[0]*p[1] + p[0]*q[1] - ( -q[3] * p[2] + q[2] * p[3]  ); // -q0*p13 + p0*q13 + q13 x p13
		r[2] = -q[0]*p[2] + p[0]*q[2] - (  q[3] * p[1] - q[1] * p[3]  ); 
		r[3] = -q[0]*p[3] + p[0]*q[3] - ( -q[2] * p[1] + q[1] * p[2]  ); 
	}

	// Integration
	void MotionGenerator::int_vel(void)
	{
		for (int n = 0 ; n < DOF ; ++n)
			q[n] += qd[n] * delta_t;

		/*for (int n=0; n<DOF;++n)
		{
		if (q[n] > PI)
		q[n] = PI;
		else if (q[n] < -PI)			
		q[n] = -PI;
		}*/

		KineChain::GetInstance().FwdKine();
	}
	void MotionGenerator::int_acc(void)
	{
		for (int n = 0 ; n < DOF ; ++n)
		{
			q[n] +=  qd[n]  * delta_t + 0.5 * qdd[n] * delta_t * delta_t;
			qd[n] += qdd[n] * delta_t;
		}

		/*for (int n=0; n<DOF;++n)
		{
		if (q[n] > PI)
		q[n] = PI;
		else if (q[n] < -PI)			
		q[n] = -PI;
		}*/

		KineChain::GetInstance().FwdKine();
	}

	void MotionGenerator::set_BK_p(double damping, double bandwidth)
	{
		Kp = bandwidth * bandwidth;
		Bp = 2 * damping * bandwidth;	
	}
	void MotionGenerator::set_BK_o(double damping, double bandwidth)
	{
		Ko = bandwidth * bandwidth;
		Bo = 2 * damping * bandwidth;	
	}
	void MotionGenerator::set_BK_q(double damping, double bandwidth)
	{
		Kq = bandwidth * bandwidth;
		Bq = 2 * damping * bandwidth;	
	}
}/* namespace motor */