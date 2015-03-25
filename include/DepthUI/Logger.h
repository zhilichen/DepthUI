#pragma once
#include "Base.h"
#include <fstream>

namespace DepthUI{

class DEPTHUI_API Logger
{
private:
	bool is_initialized;
	std::string logger_filename_;
	std::ofstream fout_;
public:
	Logger(const std::string & filename);
	void WriteLog(const std::string & message);
	~Logger();
};

}
