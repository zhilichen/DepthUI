#pragma once
#include "Base.h"
#include "Logger.h"

namespace DepthUI{

class DEPTHUI_API Hierarchy
{
public:
	Logger * logger_;
	System * parent_system_;
public:
	Hierarchy()
	{
		logger_ = NULL;
		parent_system_ = NULL;
	}

	virtual void WriteLog(std::string message)
	{
		if (logger_) logger_->WriteLog(message);
	}

	virtual System * GetParentSystem()
	{
		return parent_system_;
	}

	virtual ~Hierarchy(){};
};

}
