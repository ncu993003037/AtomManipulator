#include "MotionGenerationFactory.h"
#include "DefaultMotionControl/DefaultMotionControl.h"


namespace motion
{
	// Factory method
	std::shared_ptr<MotionGenerator> MotionGenerationFactory::NewMotionGenerator(const gName name, const gConfig &config)
	{
		switch(name)
		{
		case DEFAULT: {
				if (_ptr)
				{
					if (_ptr->isThreadCreated())
					{
						if (_ptr->Shutdown())
							_ptr.reset(new DefaultMotionControl(config));
					}
				}
				else
				{
					_ptr.reset(new DefaultMotionControl(config));
				}
			}
		case VIC: 
			{
			}
		}

		return _ptr;
	}

	std::shared_ptr<MotionGenerator> MotionGenerationFactory::GetCurrentMotionGenerator()
	{
		return _ptr;
	}

	void MotionGenerationFactory::DeleteCurrentMotionGenerator()
	{
		if (_ptr)
			_ptr.reset();
	}
}/* namespace motion */