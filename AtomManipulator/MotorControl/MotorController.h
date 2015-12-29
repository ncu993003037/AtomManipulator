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
	} gConfig;

	class MotorController
	{
	public:
		explicit MotorController(const gConfig &config);
		virtual ~MotorController();
		void Launch();
		bool Shutdown();

		void SetMotorInput(float *d);
		void SetMotorAction(bool action);
		int isThreadCreated() const;

	protected:
		virtual bool Init() = 0;
		virtual void WriteCommand(float *motor_input) = 0;

	private:
		// Thread in class (WINAPI) or you can use boost thread
		static DWORD WINAPI CreateStaticThread(void* Param);
		DWORD WINAPI Run();

		void inline usleep(__int64 usec); // for WINAPI

	private:
		HANDLE	_mThreadHandle; // Motor control thread handle
		DWORD	_mThreadID; //  Motor control thread ID
		bool	_mThreadCreated; // 指示手臂部控制thread是否打開
		bool	_mThreadLife; //  Motor control 是否工作旗標
		bool _mStopCalling;

		LARGE_INTEGER _StartTime;
		LARGE_INTEGER _CurrentTime;
		LARGE_INTEGER _nFreq;
		LARGE_INTEGER _ElapsedMicroseconds;

		float *_motor_input;

		bool _motor_action_go, _init_success;
		const gConfig _config;
	};
}/* namespace motor */
#endif