#include "MotorControl/MotorControl.h"
#include "MotorControl/SerialPort/serial_port.h"

#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>

namespace motor
{
	namespace faulhaber
	{
		class DSPdriver
		{
		public:
			DSPdriver() {};
			virtual ~DSPdriver() {};

		protected:
			SerialPort _sport;

		public:
			int Initial(const std::string &COMport, int m_baudrate)
			{
				bool check = false;

				check = _sport.open( COMport.c_str(), NORMAL_RS232);	
				check = _sport._set_baudrate(m_baudrate);	

				if(!check)
				{
					printf("Error [Motor]: %s initial fail\n",COMport.c_str());
					return 0;
				}
				else
				{
					printf("Info [Motor]: %s Initial success\n",COMport.c_str());
					return 1;
				}
			}

			void WriteOneMotorCommand(float Comm, int index)
			{
				char str[50];
				std::string write_command;

				sprintf(str,"la%d\rm\r",(int)(Comm*COUNT_RATIO[index]));
				write_command = str;

				if( !_sport._write((uchar* )write_command.c_str(),write_command.length()) )
				{
					printf("Error [Motor]: Cannot write command to motor %d suddenly\n",index);
					system("pause");
				}
			}
		}; // class DSPdriver

		/////////////////////////////////////////////////////

		MotorControl::MotorControl(int DOF_) 
			: _DOF(DOF_),
			_init_success(false),
			_gThreadHandle(NULL),
			_gThreadOpened(false),
			_motor_action_go(false)
		{	
			// Find COMs
			_init_success = COMPortInitial(expected_baud_rate);

			// Zeroing
			_motor_input = new float[_DOF];
			memset(_motor_input, 0, _DOF*sizeof(float));
			QueryPerformanceFrequency(&_nFreq); 
		}

		MotorControl::~MotorControl(void)
		{
			if (_motor_action_go)
				_motor_action_go = false;
			if (_gThreadLife)
				_gThreadLife = false;

			Sleep(1000);

			if (NULL != _DSP)
				delete [] _DSP;

			if (NULL != _motor_input)
				delete [] _motor_input;
		}

		bool MotorControl::SetThreadStart(void)
		{
			if (!_init_success)
			{
				printf("Error [Motor]: Fail to initialize, Thread cannot start.\n");
				return false;
			}

			OnStart();

			return true;
		}

		void MotorControl::SetThreadClose(void) 
		{ 
			if (_gThreadLife && _gThreadOpened) _gThreadLife = false; 
			else printf("Warning [Motor]: Thread already dead\n");
		}

		void MotorControl::SetMotorInput(float *d_) 
		{
			if (!_init_success || !_gThreadLife)
			{
				printf("Error [Motor]: Fail to write motor commands\n");
				return;
			}
			memcpy(_motor_input, d_, _DOF*sizeof(float));
		}

		void MotorControl::SetMotorAction(bool action_) 
		{ 
			_motor_action_go = action_;
			if (_motor_action_go) printf("Info [Motor]: Turn action on\n");
			else printf("Info [Motor]: Turn action off\n");
		}

		int MotorControl::GetThreadOpened(void) const {return _gThreadOpened;}

		bool MotorControl::COMPortInitial(int baudrate_)
		{
			if (!_init_success)
			{
				// Load port list from the text file
				std::vector<std::string> com_port_list(_DOF);
				std::fstream file("MotorControl/COMPort_list.txt");
				int i = 0;
				std::string temp;
				if (file.is_open())
				{
					while(getline(file, temp)) 
					{
						com_port_list[i] = temp;
						//printf("Port %d: %s\n",i+1,com_port_list[i].c_str());
						i++;
					}
				}
				else printf("Error [Motor]: Unable to load COMPort_list.txt!!\n");
				file.close();

				if (i != _DOF)
				{
					// Free vector object
					com_port_list.clear();
					std::vector<std::string>().swap(com_port_list);

					printf("Error [Motor]: The COMPort list is wrong!!\n");
					return false;
				}

				// Serial port initialization 
				_DSP = new DSPdriver[_DOF];
				int k = 0;
				for (int j = 0 ; j < _DOF ; j++)
					k += _DSP[j].Initial(com_port_list[j].c_str(), baudrate_);

				// Free vector object
				com_port_list.clear();
				std::vector<std::string>().swap(com_port_list);

				if (k != _DOF)
				{
					printf("Error [Motor]: Unable to find all COMPorts!!\n");
					return false;
				}

				printf("Info [Motor]: All COMPorts are connected!!\n");
				return true;
			}
			else
				return true;
		}

		void MotorControl::OnStart(void)
		{
			if (!_gThreadOpened)
			{
				_gThreadLife = true;
				_gThreadHandle = CreateThread(NULL, 0, StaticThreadStart, (void*) this, 0, &_gThreadID );
				_gThreadOpened = true;
				SetPriorityClass(_gThreadHandle, REALTIME_PRIORITY_CLASS);
				SetThreadPriority(_gThreadHandle, THREAD_PRIORITY_TIME_CRITICAL);
				SetThreadAffinityMask(_gThreadHandle, 1 << 0);
				printf("Info [Motor]: Thread opened\n");
			}
		}

		DWORD WINAPI MotorControl::StaticThreadStart(void* Param_)
		{
			MotorControl *pthis = (MotorControl *) Param_;
			return pthis->OnRun();
		}

		DWORD WINAPI MotorControl::OnRun(void)
		{
			QueryPerformanceCounter(&_StartTime);

			while (1)
			{
				if(!_gThreadLife)
					break;

				QueryPerformanceCounter(&_CurrentTime);
				_ElapsedMicroseconds.QuadPart = _CurrentTime.QuadPart - _StartTime.QuadPart;
				_ElapsedMicroseconds.QuadPart *= 1000000; // Micro-seconds
				_ElapsedMicroseconds.QuadPart /= _nFreq.QuadPart;
				//std::cout<<ElapsedMicroseconds.QuadPart<<std::endl;

				QueryPerformanceCounter(&_StartTime);

				if(_motor_action_go)
				{
					for (int i = 0 ; i < _DOF ; ++i)
						_DSP[i].WriteOneMotorCommand(_motor_input[i], i);
				}

				usleep(sleep_time_micro_sec);
			}

			// Wait for thread stop
			OnClose();

			return 0;
		}

		void MotorControl::OnClose(void)
		{	
			if ( NULL != _gThreadHandle && _gThreadOpened )
			{
				WaitForSingleObject( _gThreadHandle, INFINITE );
				CloseHandle( _gThreadHandle );
				_gThreadOpened = false;
				_motor_action_go = false;
				printf("Info [Motor]: Thread has been closed\n");
			}
		}

		void inline MotorControl::usleep(__int64 usec_) //for WINAPI
		{ 
			HANDLE timer; 
			LARGE_INTEGER ft; 

			ft.QuadPart = -(10*usec_); // Convert to 100 nanosecond interval, negative value indicates relative time

			timer = CreateWaitableTimer(NULL, TRUE, NULL); 
			SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
			WaitForSingleObject(timer, INFINITE); 
			CloseHandle(timer); 
		}

	}/* namespace faulhaber */
}/* namespace motor */