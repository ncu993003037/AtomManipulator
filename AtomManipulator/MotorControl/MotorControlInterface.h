#pragma once
#ifndef _MOTORCONTROLINTERFACE_
#define _MOTORCONTROLINTERFACE_ 

#include <memory>
#include "MotorController.h"

namespace motor
{
	typedef enum {
		FAULHABER
	} gName;

	class MotorControlInterface
	{
	public:
		static MotorControlInterface& GetMotorControlInterface() // Singleton
		{
			static MotorControlInterface _mci;
			return _mci;
		}

		void CreateInstancePtr(const gName &name, const gConfig &config); // Factory method
		std::shared_ptr<MotorController>	GetInstancePtr(); // the shared pointer is more safe for destruction
		void ResetInstancePtr();

	private:
		std::shared_ptr<MotorController> instance;
	};
}/* namespace motor */
#endif