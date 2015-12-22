#ifndef _GLOBAL_INSTANCES_H_
#define _GLOBAL_INSTANCES_H_


#define KINECT 0

// Robot config
static const int ROBOT_DOF = 6;

// OpenGL
#include "OpenGL_Model/OpenGLControl.h"
static COpenGLControl openGLControl;

// Robot Model
#include "MotionControl/KineChain/KineChain.h"
using namespace motion;

// Motor Control API
#include "MotorControl/MotorControlInterface.h"
using namespace motor;

// Kinect
#if KINECT
	KinectSensor kinect;
#endif

inline void AtomManipulatorInitialization(void)
{
	// Motion Control
	double base_p [3] = {0, 0, 1.220};
	double base_o [9] = {0, -1, 0, -1, 0, 0, 0, 0, -1};

	KineChain::GetRobotKineChain().InitKineChain(ROBOT_DOF);
	KineChain::GetRobotKineChain().set_base(base_o, base_p);
	KineChain::GetRobotKineChain().setDH_txt("./DH_Data/Atom_DH.txt");
}
#endif