#include "DepthUI\InputHandler.h"
#include "DepthUI\System.h"

namespace DepthUI{
InputHandler::InputHandler(System * parent_system)
{
	parent_system_ = parent_system;
}


InputHandler::~InputHandler()
{
}

bool InputHandler::InjectMouseDown(int x, int y, int mouseType)
{
	// todo
	// find clicked control
	parent_system_->GetIDMouseOn(x, y);
	
	// send event message to eventhandler with that control's id
	return true;
};

bool InputHandler::InjectMouseUp(int x, int y, int mouseType)
{
	// todo
	// find clicked control

	// send event message to eventhandler with that control's id
	return true;
};

bool InputHandler::InjectMouseMotion(int x, int y, int mouseType)
{
	// send event message to all?
	return true;
}

bool InputHandler::InjectMousePassiveMotion(int x, int y, int mouseType)
{
	return true;
}

bool InputHandler::InjectKeyPress(int keyValue)
{
	// send event message to active control
	return true;
}

}