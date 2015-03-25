#pragma once
#include "Base.h"
#include "Event.h"
#include <functional>

namespace DepthUI{

class DEPTHUI_API EventBinder
{
public:
	typedef std::function<void(EventArgs)> CallBackFunctionType;
	CallBackFunctionType function_list_[NUM_EVENT_TYPES];
public:
	EventBinder(){};
	~EventBinder(){};
	void BindEventCallBack(int EventType, CallBackFunctionType callback)
	{
		function_list_[EventType] = callback;
	}
	void ExecuteCallBack(const Event & event)
	{
		function_list_[event.event_type_](event.args_);
	}
};

}