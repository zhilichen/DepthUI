#pragma once
#include "..\Base.h"
#include "..\Control.h"

namespace DepthUI{

class DEPTHUI_API Frame :
	public Control
{
public:
	Frame(std::string name, Control * parent);
	~Frame();
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