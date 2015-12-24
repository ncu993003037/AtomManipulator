#pragma once
#ifndef _MOTIONGENERATIONINTERFACE_
#define _MOTIONGENERATIONINTERFACE_ 

#include <memory>
#include "MotionGenerator.h"

namespace motion
{
	typedef enum {
		DEFAULT,
		VIC // Virtual impedance control
	} gName;

	class MotionGenerationInterface
	{
	public:
		static MotionGenerationInterface& GetMotionGenerationInterface() // Singleton
		{
			static MotionGenerationInterface _mgi;
			return _mgi;
		}

		void CreateInstancePtr(const gName &name, const gConfig &config); // Factory method
		std::shared_ptr<MotionGenerator> GetInstancePtr(); // the shared pointer is more safe for destruction
		void ResetInstancePtr();

	private:
		std::shared_ptr<MotionGenerator> instance;
	};
}/* namespace motion */
#endif