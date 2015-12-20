#pragma once
#ifndef _FAULHABERCONTROLLER_
#define _FAULHABERCONTROLLER_ 

#include "MotorControl/MotorController.h"

namespace motor
{
	class DSPdriver;

	//===========角度/count = encoder line*4/360*減速比==============
	//齒輪比[300,400,200,268,200,268,268]
	//encoder line*4[3000,200
	static const int COUNT_RATIO[] = {-1111, 1422, 1138, 1422, 1138, -1707};
	
	class FaulhaberController : public MotorController
	{
	public:
		explicit FaulhaberController(const CL_Config &config);
		virtual ~FaulhaberController();

	protected:
		virtual bool Init();
		virtual void WriteCommand(float *motor_input);

	private:
		DSPdriver *_DSP;
		const CL_Config _config;
	};
}/* namespace motor */

#endif