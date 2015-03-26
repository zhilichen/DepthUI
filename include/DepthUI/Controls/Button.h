#pragma once
#include "../Base.h"
#include "../Control.h"
namespace DepthUI{

class DEPTHUI_API Button
	: public Control
{
public:
	Button(std::string name, Control * parent);
	~Button();
public:
	// event response
	virtual void onMouseDown(){};
	virtual void onMouseUp(){};
	virtual void onMouseClick(){};
	virtual void onMouseHover(){};
	virtual void onMouseEnter(){};
	virtual void onMouseLeave(){};
	virtual void onWindowResize(){};
private:
	uint vao_box_;
	uint vbo_position_;
	uint vbo_color_;
	uint vbo_element_;
public:
	// behaviors
	virtual void InitializeRenderObjects();
	virtual void Draw(bool draw_id);
};

}