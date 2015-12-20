#pragma once
#ifndef _MOTORCONTROLLER_
#define _MOTORCONTROLLER_ 

#include <time.h>
#include <Windows.h>
#include <fstream>

namespace motor
{
	typedef struct {
		int _DOF_;
		int _expected_baud_rate_;
		int _sleep_time_micro_sec_;
	} CL_Config;

	class MotorController
	{
	public:
		explicit MotorController(const CL_Config &config);
		virtual ~MotorController();
		void Launch();
		bool Shutdown();

		void SetMotorInput(float *d);
		void SetMotorAction(bool action);
		int GetThreadOpened() const;

	protected:
		virtual bool Init() = 0;
		virtual void WriteCommand(float *motor_input) = 0;

	private:
		// Thread in class (WINAPI) or you can use boost thread
		static DWORD WINAPI StaticThreadOpen(void* Param);
		DWORD WINAPI Run();

		void inline usleep(__int64 usec); // for WINAPI

	private:
		HANDLE	_gThreadHandle; // Motor control thread handle
		DWORD	_gThreadID; //  Motor control thread ID
		bool	_gThreadOpened; // 指示手臂部控制thread是否打開
		bool	_gThreadLife; //  Motor control 是否工作旗標

		LARGE_INTEGER _StartTime;
		LARGE_INTEGER _CurrentTime;
		LARGE_INTEGER _nFreq;
		LARGE_INTEGER _ElapsedMicroseconds;

		float *_motor_input;

		bool _motor_action_go, _init_success;
		const CL_Config _config;
	};
}/* namespace motor */
#endif