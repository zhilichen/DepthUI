#pragma once
#include "Base.h"

namespace DepthUI{

class DEPTHUI_API EventArgs
{
public:
	EventArgs(){};
	~EventArgs(){};
};

class DEPTHUI_API MouseEventArgs
	:public EventArgs
{
public:
	MouseEventArgs():EventArgs(){};
	~MouseEventArgs(){};
};

class DEPTHUI_API KeyEventArgs
	:public EventArgs
{
public:
	KeyEventArgs() :EventArgs(){};
	~KeyEventArgs(){};
};

class DEPTHUI_API ResizeEventArgs
	:public EventArgs
{
public:
	ResizeEventArgs() :EventArgs(){};
	~ResizeEventArgs(){};
};

class DEPTHUI_API Event
{
public:
	int event_type_;
	void * sender_;
	void * receiver_;
	EventArgs args_;
public:
	Event(int eventType, EventArgs args, void * sender = NULL, void * receiver = NULL)
	{
		event_type_ = eventType;
		args_ = args;
		sender_ = sender;
		receiver_ = receiver_;
	};
	~Event(){};
};

}
