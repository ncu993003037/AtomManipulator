#include "MotionGenerationInterface.h"
#include "DefaultMotionController/DefaultMotionControl.h"


namespace motion
{
	// Factory method
	void MotionGenerationInterface::CreateInstancePtr(const gName &name, const gConfig &config)
	{
		switch(name)
		{
		case DEFAULT: {
			instance.reset(new DefaultMotionControl(config));
			}
		case VIC: {
			//instance.reset(new FaulhaberController(config));
			}
		}
	}

	std::shared_ptr<MotionGenerator> MotionGenerationInterface::GetInstancePtr()
	{
		return instance;
	}

	void MotionGenerationInterface::ResetInstancePtr()
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
}/* namespace motion */