#pragma once
#include "Base.h"

namespace DepthUI{

class DEPTHUI_API InputHandler
{
private:
	System * parent_system_;
public:
	// input states
public:
	InputHandler(System * parent_system);
	~InputHandler();
public:
	// input injections
	bool InjectMouseDown(int x, int y, int mouseType);
	bool InjectMouseUp(int x, int y, int mouseType);
	bool InjectMouseMotion(int x, int y, int mouseType);
	bool InjectMousePassiveMotion(int x, int y, int mouseType);
	bool InjectKeyPress(int keyValue);
};

}
