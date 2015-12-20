#ifndef _THREADCONTROL_H_
#define _THREADCONTROL_H_


#define KINECT 0

// Robot config
static const int ROBOT_DOF = 6;

#include "MotorControl/MotorControlInterface.h"
extern motor::MotorControlInterface MCI;

// Kinect
#if KINECT
	KinectSensor kinect;
#endif

// Robot Model
#include "MotionControl/KineChain/KineChain.h"
extern motion::KineChain robot;

void AtomManipulatorInitialization(void); 

#endif