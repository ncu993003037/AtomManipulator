#ifndef _THREADCONTROL_H_
#define _THREADCONTROL_H_


#define MOTOR 1
#define KINECT 0
#define ROBOT_DOF 6

// Global variables
extern float Timer;
extern float finalInput[ROBOT_DOF];
extern bool working, breakflag, gRenderKineWorking;

// Rendering
#include "OpenGL_Model/DH_Model.h"
struct Dimension { float *Pos; };
extern float RAini_theta[ROBOT_DOF+1]; //手臂起始位置
extern std::vector <Dimension> RActionPath;
extern	ArmKinematic RArm;

// Motor Control
#include "MotorControl/MotorControl.h"
#if MOTOR
	extern motor::faulhaber::MotorControl Motors;
#endif

// Kinect
#if KINECT
	extern KinectSensor kinect;
#endif

// Motion Control
#include "MotionControl/KineChain/KineChain.h"
extern motion::KineChain robot;

void create_all_threads(void);

#endif