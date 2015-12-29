#include "MotorController.h"

namespace motor
{

	MotorController::MotorController(const gConfig &config)
		: _config(config),
		_init_success(false),
		_mThreadHandle(NULL),
		_mThreadCreated(false),
		_motor_action_go(false),
		_mStopCalling(true)
	{
		//printf("%d %d %d\n",_config._DOF_,_config._expected_baud_rate_,_config._sleep_time_micro_sec_);
		printf("Info [Motor]: Create Motor Control API \n");
	}

	MotorController::~MotorController()
	{	
		_mStopCalling = true;
		if (isThreadCreated())
			Shutdown();	
		printf("Info [Motor]: Motor Control API destroyed\n");		
	}

	void MotorController::Launch()
	{
		QueryPerformanceFrequency(&_nFreq); 

		// Find COMs
		_init_success = Init();
		if (!_init_success)
		{
			printf("Error [Motor]: Fail to initialize, Thread cannot start.\n");
			return;
		}

		// Allocate
		_motor_input = new float[_config._DOF_];
		memset(_motor_input, 0, _config._DOF_*sizeof(float));

		if (!_mThreadCreated)
		{
			_mThreadLife = true;
			_mThreadHandle = CreateThread(NULL, 0, CreateStaticThread, (void*) this, 0, &_mThreadID );
			_mThreadCreated = true;
			_mStopCalling = false;
			SetPriorityClass(_mThreadHandle, REALTIME_PRIORITY_CLASS);
			SetThreadPriority(_mThreadHandle, THREAD_PRIORITY_TIME_CRITICAL);
			SetThreadAffinityMask(_mThreadHandle, 1 << 0);
			printf("Info [Motor]: Thread opened\n");
		}
	}

	bool MotorController::Shutdown()
	{
		if (_mThreadLife && _mThreadCreated)
		{
			if (_motor_action_go)
				_motor_action_go = false;

			Sleep(500);

			_mThreadLife = false; 

			Sleep(500);

			if ( NULL != _mThreadHandle )
			{
				DWORD ret = WaitForSingleObject( _mThreadHandle, INFINITE);
				CloseHandle( _mThreadHandle );
				_mThreadCreated = false;
			}

			Sleep(500);

			if (NULL != _motor_input)
				delete [] _motor_input;

			printf("Info [Motor]: Thread has been closed\n");
		}
		else printf("Warning [Motor]: Thread already dead\n");

		return true;
	}

	DWORD WINAPI MotorController::CreateStaticThread(void* Param_)
	{
		MotorController *pthis = (MotorController *) Param_;
		return pthis->Run();
	}

	DWORD WINAPI MotorController::Run()
	{
		QueryPerformanceCounter(&_StartTime);

		while (_mThreadLife)
		{
			QueryPerformanceCounter(&_CurrentTime);
			_ElapsedMicroseconds.QuadPart = _CurrentTime.QuadPart - _StartTime.QuadPart;
			_ElapsedMicroseconds.QuadPart *= 1000000; // Micro-seconds
			_ElapsedMicroseconds.QuadPart /= _nFreq.QuadPart;

			//printf("Info [Motor]: _ElapsedMicroseconds = %d\n",_ElapsedMicroseconds.QuadPart);
			QueryPerformanceCounter(&_StartTime);

			if (!_mStopCalling)
			{
				// Do something
				if(_motor_action_go)
					WriteCommand(_motor_input);
			}

			usleep(_config._sleep_time_micro_sec_);
		}

		return 0;
	}

	int MotorController::isThreadCreated() const {return _mThreadCreated;}

	inline void MotorController::usleep(__int64 usec_) //for WINAPI
	{ 
		HANDLE timer; 
		LARGE_INTEGER ft; 

		ft.QuadPart = -(10*usec_); // Convert to 100 nanosecond interval, negative value indicates relative time

		timer = CreateWaitableTimer(NULL, TRUE, NULL); 
		SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
		WaitForSingleObject(timer, INFINITE); 
		CloseHandle(timer); 
	}

	void MotorController::SetMotorInput(float *d) 
	{
		if (!_init_success || !_mThreadLife)
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
}/* namespace motor */