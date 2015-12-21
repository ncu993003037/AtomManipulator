#ifndef _GLOBAL_INSTANCES_H_
#define _GLOBAL_INSTANCES_H_


#define KINECT 0

// Robot config
static const int ROBOT_DOF = 6;

#include "MotorControl/MotorControlInterface.h"
using namespace motor;

// Robot Model
#include "MotionControl/KineChain/KineChain.h"
motion::KineChain robot(ROBOT_DOF);

// Kinect
#if KINECT
	KinectSensor kinect;
#endif

inline void AtomManipulatorInitialization(void)
{
	// Motion Control
	double base_p [3] = {0, 0, 1.220};
	double base_o [9] = {0, -1, 0, -1, 0, 0, 0, 0, -1};
	//#if MOTOR
	//	robot.set_motor_control_ptr(&Motors);
	//#endif
	//robot.set_base(base_o, base_p);
	//robot.setDH_txt("./DH_Data/Atom_DH.txt");
}
#endif