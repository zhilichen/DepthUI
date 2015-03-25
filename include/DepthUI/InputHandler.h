#pragma once
#include "Base.h"

namespace DepthUI{

class DEPTHUI_API InputHandler
{
public:
	InputHandler();
	~InputHandler();
public:
	// input injections
	bool InjectMouseClick(int x, int y)
	{
		// todo
		// find clicked control
		// send event message to eventhandler with that control's id
		return true;
	};

	bool InjectMouseMotion(int x, int y, int mouseType)
	{
		// send event message to all?
		return true;
	}

	bool InjectMousePassiveMotion(int x, int y, int mouseType)
	{
		return true;
	}

	bool InjectKeyPress(int keyValue)
	{
		// send event message to active control
		return true;
	}
};

}
