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
	public:
		static MotorControlInterface& GetMotorControlInterface()
		{
			static MotorControlInterface _mci;
			return _mci;
		}

		void CreateInstancePtr(const CL_Name &name, const CL_Config &config);
		std::shared_ptr<MotorController>	GetInstancePtr();
		void ResetInstancePtr();

	private:
		std::shared_ptr<MotorController> instance;
	};
}/* namespace motor */
#endif