#pragma once
#ifndef _MOTORCONTROLFACTORY_
#define _MOTORCONTROLFACTORY_ 

#include <memory>
#include "MotorController.h"

namespace motor
{
	typedef enum {
		FAULHABER
	} gName;

	class MotorControlFactory
	{
	public:
		static MotorControlFactory& GetInstance() // Singleton
		{
			static MotorControlFactory _instance;
			return _instance;
		}

		std::shared_ptr<MotorController> NewMotorController(const gName name, const gConfig &config); // Factory method
		std::shared_ptr<MotorController>	GetCurrentMotorController(); // the shared pointer is more safe for destruction
		void DeleteCurrentMotorController();

	private:
		std::shared_ptr<MotorController> _ptr;
	};
}/* namespace motor */
#endif