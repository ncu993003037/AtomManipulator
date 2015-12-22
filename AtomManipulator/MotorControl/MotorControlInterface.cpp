#include "MotorControl/MotorControlInterface.h"
#include "MotorControl/FaulhaberController/FaulhaberController.h"


namespace motor
{
	// Factory method
	void MotorControlInterface::CreateInstancePtr(const CL_Name &name, const CL_Config &config)
	{
		switch(name)
		{
		case FAULHABER: {
			instance.reset(new FaulhaberController(config));
			}
		}
	}

	std::shared_ptr<MotorController>	MotorControlInterface::GetInstancePtr()
	{
		return instance;
	}

	void MotorControlInterface::ResetInstancePtr()
	{
		if (instance)
		{
			if (instance->GetThreadOpened())
			{
				if (instance->Shutdown())
					instance.reset();
			}
			else
				instance.reset();
		}
	}
}/* namespace motor */