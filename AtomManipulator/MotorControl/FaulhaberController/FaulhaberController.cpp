#include "MotorControl/FaulhaberController/FaulhaberController.h"
#include "MotorControl/SerialPort/serial_port.h"

#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>

namespace motor
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

				printf("%s ",write_command.c_str());

				if( !_sport._write((uchar* )write_command.c_str(),write_command.length()) )
				{
					printf("Error [Motor]: Cannot write command to motor %d suddenly\n",index);
					system("pause");
				}
			}
		}; // class DSPdriver

		/////////////////////////////////////////////////////

		FaulhaberController::FaulhaberController(const CL_Config &config)
			: MotorController(config),
			_config(config)
		{	
		}

		FaulhaberController::~FaulhaberController()
		{
			if (NULL != _DSP)
				delete [] _DSP;
			printf("Faulhaber controller destroyed\n");
		}

		bool FaulhaberController::Init()
		{
			// Load port list from the text file
			std::vector<std::string> com_port_list(_config._DOF_);
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

			if (i != _config._DOF_)
			{
				// Free vector object
				com_port_list.clear();
				std::vector<std::string>().swap(com_port_list);

				printf("Error [Motor]: The COMPort list is wrong!!\n");
				return false;
			}

			// Serial port initialization 
			_DSP = new DSPdriver[_config._DOF_];
			int k = 0;
			for (int j = 0 ; j < _config._DOF_ ; j++)
				k += _DSP[j].Initial(com_port_list[j].c_str(), _config._expected_baud_rate_);

			// Free vector object
			com_port_list.clear();
			std::vector<std::string>().swap(com_port_list);

			if (k != _config._DOF_)
			{
				printf("Error [Motor]: Unable to find all COMPorts!!\n");
				return false;
			}

			printf("Info [Motor]: All COMPorts are connected!!\n");
			return true;
		}

		void FaulhaberController::WriteCommand(float *motor_input)
		{
			for (int i = 0 ; i < _config._DOF_ ; ++i)
				_DSP[i].WriteOneMotorCommand(motor_input[i], i);
			printf("\n");
		}
}/* namespace motor */