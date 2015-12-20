#include "MotorControl/MotorControlInterface.h"
#include "MotorControl/FaulhaberController/FaulhaberController.h"


namespace motor
{
	MotorControlInterface::MotorControlInterface(CL_Name &name, CL_Config &config)
		: _name(name),
		_config(config)
	{
		CreateInstancePtr();
	}

	MotorControlInterface::~MotorControlInterface()
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
	
	void MotorControlInterface::CreateInstancePtr()
	{
		switch(_name)
		{
		case FAULHABER: {
			instance.reset(new FaulhaberController(_config));
			}
		}
	}

	std::shared_ptr<MotorController>	MotorControlInterface::GetInstancePtr()
	{
		return instance;
	}
}/* namespace motor */