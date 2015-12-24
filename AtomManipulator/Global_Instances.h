#ifndef _GLOBAL_INSTANCES_H_
#define _GLOBAL_INSTANCES_H_


// Config
static const int ROBOT_DOF = 6;

// OpenGL
#include "OpenGL_Model/OpenGLControl.h"
static COpenGLControl openGLControl; // avoid to use extern property 

// Robot Model
#include "MotionGeneration/KineChain/KineChain.h"

// Motor Control API
#include "MotorControl/MotorControlInterface.h"

// Motion API
#include "MotionGeneration/MotionGenerationInterface.h"

inline void AtomManipulatorInitialization(void)
{
	// Motion Control
	double base_p [3] = {0, 0, 1.220};
	double base_o [9] = {0, -1, 0, -1, 0, 0, 0, 0, -1};

	motion::KineChain::GetRobotKineChain().InitKineChain(ROBOT_DOF);
	motion::KineChain::GetRobotKineChain().set_base(base_o, base_p);
	motion::KineChain::GetRobotKineChain().setDH_txt("./DH_Data/Atom_DH.txt");

	motion::gConfig atom_motion_config;
	atom_motion_config._DOF_ = ROBOT_DOF;
	atom_motion_config._sleep_time_micro_sec_ = 1000;
	motion::MotionGenerationInterface::GetMotionGenerationInterface().
		CreateInstancePtr(motion::DEFAULT, atom_motion_config);
}
#endif