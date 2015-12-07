#include "ThreadControl.h"



bool working = false, breakflag = false, gRenderKineWorking = false;
float finalInput[ROBOT_DOF] = {0.};
float RAini_theta[ROBOT_DOF+1] = {0}; //手臂起始位置

std::vector <Dimension> RActionPath;
ArmKinematic RArm("OpenGL_Model\\DHParameter.txt");

// Motor Control
#if MOTOR
	motor::faulhaber::MotorControl Motors(ROBOT_DOF);
#endif

// Kinect
#if KINECT
	KinectSensor kinect;
#endif

motion::KineChain robot(ROBOT_DOF);

int AtomManipulatorInitialization(void)
{
#if MOTOR
	if (!Motors.GetThreadOpened())
		Motors.SetThreadStart();
#endif

	// Motion Control
	double base_p [3] = {0, 0, 1.220};
	double base_o [9] = {0, -1, 0, -1, 0, 0, 0, 0, -1};
#if MOTOR
	robot.set_motor_control_ptr(&Motors);
#endif
	robot.set_base(base_o, base_p);
	robot.setDH_txt("./DH_Data/Atom_DH.txt");

#if MOTOR 
	return Motors.GetThreadOpened();
#else
	return 0;
#endif
}