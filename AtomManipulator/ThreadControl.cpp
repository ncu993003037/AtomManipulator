#include "ThreadControl.h"



// Motor Control
//std::shared_ptr<motor::MotorController> atom_motors;

// Kinect
#if KINECT
	KinectSensor kinect;
#endif
	
motion::KineChain robot(ROBOT_DOF);

void AtomManipulatorInitialization(void)
{
	// Motion Control
	double base_p [3] = {0, 0, 1.220};
	double base_o [9] = {0, -1, 0, -1, 0, 0, 0, 0, -1};
//#if MOTOR
//	robot.set_motor_control_ptr(&Motors);
//#endif
	robot.set_base(base_o, base_p);
	robot.setDH_txt("./DH_Data/Atom_DH.txt");
}