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
#include "MotorControl/MotorControlFactory.h"

// Motion API
#include "MotionGeneration/MotionGenerationFactory.h"

inline void AtomManipulatorInitialization(void)
{
	// Motion Control
	double base_p [3] = {0, 0, 1.220};
	double base_o [9] = {0, -1, 0, -1, 0, 0, 0, 0, -1};

	motion::KineChain::GetInstance().InitKineChain(ROBOT_DOF);
	motion::KineChain::GetInstance().set_base(base_o, base_p);
	motion::KineChain::GetInstance().setDH_txt("./DH_Data/Atom_DH.txt");

	motion::gConfig atom_motion_config;
	atom_motion_config._DOF_ = ROBOT_DOF;
	atom_motion_config._sleep_time_micro_sec_ = 1000;
	motion::MotionGenerationFactory::GetInstance().
		NewMotionGenerator(motion::DEFAULT, atom_motion_config);
}
#endif