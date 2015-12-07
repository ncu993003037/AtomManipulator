#pragma once
#ifndef _MOTORCONTROL_
#define _MOTORCONTROL_ 

#include <time.h>
#include <Windows.h>
#include <fstream>

namespace motor
{
	namespace faulhaber
	{
		class DSPdriver;

		static const int sleep_time_micro_sec = 6000;
		static const int expected_baud_rate = 115200;
		//===========角度/count = encoder line*4/360*減速比==============
		//齒輪比[300,400,200,268,200,268,268]
		//encoder line*4[3000,200
		static const int COUNT_RATIO[] = {-1111, 1422, 1138, 1422, 1138, -1707};
	
		class MotorControl 
		{
		public:
			MotorControl(int DOF_);
			~MotorControl(void);

			bool SetThreadStart(void);
			void SetThreadClose(void);
			void SetMotorInput(float *d_);
			void SetMotorAction(bool action_);

			int GetThreadOpened(void) const;

		private:
			DSPdriver *_DSP;
			const int _DOF;
			float *_motor_input;
			bool _motor_action_go, _init_success;

			HANDLE	_gThreadHandle; // Motor control thread handle
			DWORD	_gThreadID; //  Motor control thread ID
			bool	_gThreadOpened; // 指示手臂部控制thread是否打開
			bool	_gThreadLife; //  Motor control 是否工作旗標

			LARGE_INTEGER _StartTime;
			LARGE_INTEGER _CurrentTime;
			LARGE_INTEGER _nFreq;
			LARGE_INTEGER _ElapsedMicroseconds;

		private:
			bool COMPortInitial(int baudrate_);

			// Thread in class (WINAPI) or you can use boost thread
			static DWORD WINAPI StaticThreadStart(void* Param_);
			DWORD WINAPI OnRun(void);
			void OnStart(void);
			void OnClose(void);

			void inline usleep(__int64 usec_); // for WINAPI
		};
	}/* namespace faulhaber */
}/* namespace motor */

#endif