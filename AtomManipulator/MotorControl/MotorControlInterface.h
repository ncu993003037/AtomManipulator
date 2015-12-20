#pragma once
#ifndef _MOTORCONTROLINTERFACE_
#define _MOTORCONTROLINTERFACE_ 

#include <memory>
#include "MotorControl/MotorController.h"

namespace motor
{
	typedef enum {
		FAULHABER
	} CL_Name;

	class MotorControlInterface
	{
		explicit MotorControlInterface(CL_Name &name, CL_Config &config);
		~MotorControlInterface();

	public:
		std::shared_ptr<MotorController>	GetInstancePtr();

	private:
		void CreateInstancePtr();

	private:
		const CL_Name _name;
		const CL_Config _config;

		std::shared_ptr<MotorController> instance;
	};
}/* namespace motor */
#endif