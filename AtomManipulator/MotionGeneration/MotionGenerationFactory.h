#pragma once
#ifndef _MOTIONGENERATIONFACTORY_
#define _MOTIONGENERATIONFACTORY_ 

#include <memory>
#include "MotionGenerator.h"

namespace motion
{
	typedef enum {
		DEFAULT,
		VIC // Virtual impedance control
	} gName;

	class MotionGenerationFactory
	{
	public:
		static MotionGenerationFactory& GetInstance() // Singleton
		{
			static MotionGenerationFactory _instance;
			return _instance;
		}

		std::shared_ptr<MotionGenerator> NewMotionGenerator(const gName name, const gConfig &config); // Factory method
		std::shared_ptr<MotionGenerator> GetCurrentMotionGenerator();
		void DeleteCurrentMotionGenerator();

	private:
		std::shared_ptr<MotionGenerator> _ptr;
	};
}/* namespace motion */
#endif