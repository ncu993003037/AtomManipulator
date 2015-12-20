#include "MotorController.h"

namespace motor
{

	MotorController::MotorController(const CL_Config &config)
		: _config(config),
		_init_success(false),
		_gThreadHandle(NULL),
		_gThreadOpened(false),
		_motor_action_go(false)
	{
	}

	MotorController::~MotorController()
	{
	}

	void MotorController::Launch()
	{
		QueryPerformanceFrequency(&_nFreq); 

		// Find COMs
		_init_success = Init();
		if (!_init_success)
		{
			printf("Error [Motor]: Fail to initialize, Thread cannot start.\n");
			//return;
		}

		// Allocate
		_motor_input = new float[_config._DOF_];
		memset(_motor_input, 0, _config._DOF_*sizeof(float));

		if (!_gThreadOpened)
		{
			_gThreadLife = true;
			_gThreadHandle = CreateThread(NULL, 0, StaticThreadOpen, (void*) this, 0, &_gThreadID );
			_gThreadOpened = true;
			SetPriorityClass(_gThreadHandle, REALTIME_PRIORITY_CLASS);
			SetThreadPriority(_gThreadHandle, THREAD_PRIORITY_TIME_CRITICAL);
			SetThreadAffinityMask(_gThreadHandle, 1 << 0);
			printf("Info [Motor]: Thread opened\n");
		}
	}

	bool MotorController::Shutdown()
	{
		if (_gThreadLife && _gThreadOpened)
		{
			_gThreadLife = false; 

			if (_motor_action_go)
				_motor_action_go = false;
			if (_gThreadLife)
				_gThreadLife = false;

			Sleep(500);

			if ( NULL != _gThreadHandle && _gThreadOpened )
			{
				DWORD ret = WaitForSingleObject( _gThreadHandle, INFINITE);
				CloseHandle( _gThreadHandle );
				_gThreadOpened = false;
			}

			Sleep(500);

			if (NULL != _motor_input)
				delete [] _motor_input;

			printf("Info [Motor]: Thread has been closed\n");
		}
		else printf("Warning [Motor]: Thread already dead\n");

		return true;
	}

	DWORD WINAPI MotorController::StaticThreadOpen(void* Param_)
	{
		MotorController *pthis = (MotorController *) Param_;
		return pthis->Run();
	}

	DWORD WINAPI MotorController::Run()
	{
		QueryPerformanceCounter(&_StartTime);

		while (_gThreadLife)
		{
			QueryPerformanceCounter(&_CurrentTime);
			_ElapsedMicroseconds.QuadPart = _CurrentTime.QuadPart - _StartTime.QuadPart;
			_ElapsedMicroseconds.QuadPart *= 1000000; // Micro-seconds
			_ElapsedMicroseconds.QuadPart /= _nFreq.QuadPart;

			if (_ElapsedMicroseconds.QuadPart >= _config._sleep_time_micro_sec_)
			{
				printf("_ElapsedMicroseconds: %d\n",_ElapsedMicroseconds.QuadPart);
				QueryPerformanceCounter(&_StartTime);

				if(_motor_action_go)
					WriteCommand(_motor_input);
			}

			usleep(_config._sleep_time_micro_sec_);
		}

		return 0;
	}

	void MotorController::SetMotorInput(float *d) 
	{
		if (!_init_success || !_gThreadLife)
		{
			printf("Error [Motor]: Fail to write motor commands\n");
			return;
		}
		memcpy(_motor_input, d, _config._DOF_*sizeof(float));
	}

	void MotorController::SetMotorAction(bool action) 
	{ 
		_motor_action_go = action;
		if (_motor_action_go) printf("Info [Motor]: Turn action on\n");
		else printf("Info [Motor]: Turn action off\n");
	}

	int MotorController::GetThreadOpened() const {return _gThreadOpened;}

	void inline MotorController::usleep(__int64 usec_) //for WINAPI
	{ 
		HANDLE timer; 
		LARGE_INTEGER ft; 

		ft.QuadPart = -(10*usec_); // Convert to 100 nanosecond interval, negative value indicates relative time

		timer = CreateWaitableTimer(NULL, TRUE, NULL); 
		SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
		WaitForSingleObject(timer, INFINITE); 
		CloseHandle(timer); 
	}

}/* namespace motor */