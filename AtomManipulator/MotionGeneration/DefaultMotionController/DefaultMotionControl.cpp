#include "DefaultMotionControl.h"
#include "MotionGeneration/KineChain/KineChain.h"
//#include <vector>
//#include <cstdlib>
//#include <cstring>
//#include <string>
#include <iostream>

namespace motion
{
		DefaultMotionControl::DefaultMotionControl(const gConfig &config)
			: MotionGenerator(config),
			_config(config)
		{	
		}

		DefaultMotionControl::~DefaultMotionControl()
		{
			printf("Motion control destroyed\n");
		}

		void DefaultMotionControl::IKControlLoop()
		{
			// Position Error
			double p_err[3];
			Mat_xy_min( p_n,   p_r,  p_err,3);	// p_err =  p -  p_r 
			_p_err = Mat_norm(p_err, 3);// for  monitoring
			Mat_ax_mul( p_err, -Kp,  p_err,3);	// p_err =  -Kp * p_err

			// Orientation Error 
			double o_n [4], o_err[4];
			Qua_rot2qua(o_n_9, o_n); // convert current rotation into quaternion representation	
			Qua_pro_qpbar(o_r,o_n,o_err); // compute quaternion error, o_err = o_r * inv(o_n)
			_o_err = Mat_norm(o_err+1, 3);// for  monitoring
			Mat_ax_mul( o_err+1, Ko,  o_err+1,3); // o_err = Ko * o_err

			// Desired twist
			double tw_d[6];
			Mat_xy_plu(p_err,   pd_r,   tw_d, 3); 	
			Mat_xy_plu(o_err+1 ,od_r,  tw_d+3,3); 

			motion::KineChain::GetRobotKineChain().get_Jac(Jac.J, DOF);	
			Jac.svd();// Jac is corrupted

			// Pseudo inverse // qd = V * Sinv * Utde
			for (int i=0; i<Jac.k; ++i)
				Jac.Sinv[i] = Jac.S[i]/(Jac.S[i]*Jac.S[i] + eps*eps); // damping	

			Mat_ADBx_by(Jac.Vt, Jac.k, DOF,
				Jac.Sinv, 
				Jac.U, 6, Jac.k, tw_d, qd, 0, 'Y', 'Y');	

			// integration
			int_vel();
		}

		void DefaultMotionControl::JointControlLoop()
		{
			Mat_xy_min( q,  q_r,  q_err, DOF);	// q_err =  q -  q_r 
			Mat_xy_min(qd, qd_r, qd_err, DOF);	// qd_err = qd - qd_r 	
			_q_err = Mat_norm(q_err, DOF);// for  monitoring

			// qdd = -Bq * qd_err - Kq * q_err
			Mat_ax_mul( q_err, -Kq, q_err,  DOF);
			Mat_ax_mul(qd_err, -Bq, qd_err, DOF);

			Mat_xy_plu(qd_err, q_err, qdd, DOF);

			// integrate
			int_acc();
		}
}/* namespace motion */