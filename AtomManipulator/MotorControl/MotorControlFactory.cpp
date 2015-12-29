#include "MotorControlFactory.h"
#include "FaulhaberController/FaulhaberController.h"


namespace motor
{
	std::shared_ptr<MotorController> MotorControlFactory::NewMotorController(const gName name, const gConfig &config)
	{
		switch(name)
		{
		case FAULHABER: 
			{
				if (_ptr)
				{
					if (_ptr->isThreadCreated())
					{
						if (_ptr->Shutdown())
							_ptr.reset(new FaulhaberController(config));
					}
				}
				else
				{
					_ptr.reset(new FaulhaberController(config));
				}
			}
		}
		return _ptr;
	}

	std::shared_ptr<MotorController>	MotorControlFactory::GetCurrentMotorController()
	{
		return _ptr;
	}

	void MotorControlFactory::DeleteCurrentMotorController()
	{
		if (_ptr)
			_ptr.reset();
	}
}/* namespace motor */