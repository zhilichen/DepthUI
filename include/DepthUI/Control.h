#pragma once
#include "Base.h"
#include "ViewLayout.h"
#include "EventBinder.h"
#include "Shader.h"
#include "Hierarchy.h"

namespace DepthUI{

class DEPTHUI_API Control : public Hierarchy
{
public:
	Control(std::string name, Control * parent);
	Control(std::string name);
	~Control(){};
public:
	// common properties
	uint ID_Internal_;
	uint ID_;
	std::string name_;
	std::string type_;

	// tree structure
	std::vector<Control*> children_;
	Control * parent_;

	// event response
	EventBinder event_binder_;

	virtual void BindCallBack(int eventType, EventBinder::CallBackFunctionType callback);

	virtual void ProcessEvent(Event event);

	// default event response
	virtual void onMouseDown();
	virtual void onMouseUp();
	virtual void onMouseClick();
	virtual void onMouseHover();
	virtual void onMouseEnter();
	virtual void onMouseLeave();
	virtual void onWindowResize();
	virtual void onDataChange();

	virtual void Resize(ViewLayout * view_parent = NULL);

	// view layout
	ViewLayout * viewlayout_;
	void SetLayout(const ViewLayout::SingleDimensionLayout & layout_x, const ViewLayout::SingleDimensionLayout & layout_y);
	//Transformation transform_addition_;

	// rendering
	bool is_render_ready_;
	float z_order_;
	DefaultShader shader_default_;
	virtual void InitializeRenderObjects();
	virtual void Draw(bool draw_id = false);
	void DrawChildren(bool draw_id = false);
};

}
