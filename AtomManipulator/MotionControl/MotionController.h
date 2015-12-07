#pragma once
#ifndef _MOTIONCONTROLLER_
#define _MOTIONCONTROLLER_ 

#include <time.h>
#include <Windows.h>

#include "MoPaC/KineChain.h"
#include "MoPac/Math/MatLib.h"

namespace motion
{
	class KineChain;

	namespace control
	{
		class USVt;

		class MotionController
		{
		public:
			explicit MotionController(KineChain *robot_);
			~MotionController(void);

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

			void Qua_rot2qua(double *rot, double *qua);
			void Qua_qua2rot(double *qua, double *rot);
			void Qua_pro_qp(double *q, double *p, double *r);
			void Qua_pro_qbarp(double *q, double *p, double *r);
			void Qua_pro_qpbar(double *q, double *p, double *r);

			// Controllers
			void CarVelTranControl(void);
			void CarVelControl(void);

			void CarAccTranControl(void);
			void CarAccControl(void);
			void CarAccPosControl(void);


			void JoiVelControl(void);
			void JoiAccControl(void);

			void update_CarVel(void);

		private:
			//USVt Jac;

			// End-effector
			//double *const o_n_9; // orientation
			//double *const p_n; // position

			// Cartesian space	
			// reference
			double p_r [3], pd_r[3], pdd_r[3];	
			double o_r [4], od_r[3], odd_r[3]; // o_r is stored in quaternion	
			double _p_err, _o_err; // for monitoring

			// Joint space
			// reference
			double *q_r, *qd_r, *qdd_r;
			// state
			double *const q; // linked to robot
			//double *const qd; // linked to robot
			double *qdd;

			double _q_err; //for monitoring

			// Gain
			int DOF;
			double delta_t; // control time interval
			double Bp, Kp, Bo, Ko;	
			double Bq, Kq;

			double eps;

			double *q_err, *qd_err;

		private:
			KineChain* _robot;

			void int_vel(void);
			void int_acc(void);
		};
	}/* namespace control */
}/* namespace motion */

#endif