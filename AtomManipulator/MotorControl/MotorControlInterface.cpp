#include "MotorControlInterface.h"
#include "FaulhaberController/FaulhaberController.h"


namespace motor
{
	void MotorControlInterface::CreateInstancePtr(const gName &name, const gConfig &config)
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