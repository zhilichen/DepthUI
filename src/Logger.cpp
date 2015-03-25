#include "DepthUI\Logger.h"
#include <iostream>

namespace DepthUI{
Logger::Logger(const std::string & filename)
{
	logger_filename_ = filename;
	is_initialized = false;
}


Logger::~Logger()
{
	if (fout_.is_open())
	{
		fout_.close();
	}
}

void Logger::WriteLog(const std::string & message)
{
	if (!is_initialized)
	{
		fout_.open(logger_filename_.c_str(), std::ofstream::out | std::ofstream::app);
		if (fout_.is_open())
		{
			is_initialized = true;
		}
		else
		{
			printf("open logger file %s error!\n", logger_filename_.c_str());
		}
	}

	if (is_initialized)
	{
		fout_ << message << std::endl;
		// TODO more info
	}
}

}