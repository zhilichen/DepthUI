#include "DepthUI\Control.h"
#include "DepthUI\System.h"

namespace DepthUI{

Control::Control(std::string name, Control * parent){
	name_ = name;
	viewlayout_ = new ViewLayout();
	//viewlayout_old_ = new ViewLayout();

	if (parent != NULL)
	{
		parent_ = parent;
		parent_system_ = parent->parent_system_;
		logger_ = parent->logger_;
		ID_Internal_ = parent_system_->count_controls_in_systems_;
		parent_system_->count_controls_in_systems_++;
		parent_->children_.push_back(this);
		z_order_ = parent->z_order_ + 1.0;
	}
	else
	{
		printf("Error! parent cannot be NULL");
	}
	is_render_ready_ = false;
}

Control::Control(std::string name){
	name_ = name;
	viewlayout_ = new ViewLayout();
	//viewlayout_old_ = new ViewLayout();
	
	ID_Internal_ = 0;
	parent_ = NULL;
	parent_system_ = NULL;
	logger_ = NULL;
	z_order_ = -999;
	is_render_ready_ = false;
}

Control * Control::findControlByInternalID(uint ID)
{
	if (ID_Internal_ == ID)
	{
		return this;
	}
	else
	{
		Control * controlInChildren = NULL;
		for (std::vector<Control*>::iterator iter = children_.begin(); iter != children_.end() && controlInChildren==NULL; iter++)
		{
			controlInChildren = (*iter)->findControlByInternalID(ID);
		}
		return controlInChildren;
	}
}

void Control::BindCallBack(int eventType, EventBinder::CallBackFunctionType callback)
{
	event_binder_.BindEventCallBack(eventType, callback);
}

void Control::ProcessEvent(Event event)
{
	int eventType = event.event_type_;
	event_binder_.ExecuteCallBack(event);
}

void Control::onMouseDown()
{
}

void Control::onMouseUp()
{
}

void Control::onMouseClick()
{
}

void Control::onMouseHover()
{
}

void Control::onMouseEnter()
{
}

void Control::onMouseLeave()
{
}

void Control::onWindowResize()
{
}

void Control::onDataChange()
{
}

void Control::Draw(bool draw_id)
{
	DrawChildren(draw_id);
}

void Control::DrawChildren(bool draw_id)
{
	for (std::vector<Control*>::iterator iter = children_.begin(); iter != children_.end(); iter++)
	{
		(*iter)->Draw(draw_id);
	}
}

void Control::SetLayout(const ViewLayout::SingleDimensionLayout & layout_x, const ViewLayout::SingleDimensionLayout & layout_y)
{
	viewlayout_->layout_settings_x = layout_x;
	viewlayout_->layout_settings_y = layout_y;
}

void Control::Resize(ViewLayout * view_parent)
{
	//swap(viewlayout_old_, viewlayout_);

	// recompute size based on parent's size
	if (view_parent != NULL)
		viewlayout_->RefreshGlobalLayout(*view_parent);
	else if (parent_!= NULL)
		viewlayout_->RefreshGlobalLayout(*parent_->viewlayout_);

	for (std::vector<Control*>::iterator iter = children_.begin(); iter != children_.end(); iter++)
	{
		(*iter)->Resize();
	}
}

void Control::InitializeRenderObjects()
{

}

}