#pragma once

#include "Base.h"
#include "Hierarchy.h"
#include "Shader.h"

namespace DepthUI{

class DEPTHUI_API System : public Hierarchy
{
private:
	ViewLayout * view_window_;
	Control * control_root_;
	InputHandler * input_handler_;
	static int count_systems_;
	int id_system_;

	// rendering
	uint tex_Color_;
	uint tex_IDmap_;
	uint fbo_GUI_;
	Quad2DTextureShader shader_quad2D_;

	// states
	bool is_ID_map_resized_;
	bool is_ID_map_dirty_;
public:
	int count_controls_in_systems_;
public:
	int glut_windowid_;
public:
	System();
	bool Initialize(int glut_window = 0);
	void SetViewport(int x, int y, int w, int h);
	void Render();
	uint GetIDMouseOn(int x, int y);
	~System();
private:
	void RenderControls();
	void DrawToScreen();
public:
	Control * ControlRoot();
	InputHandler * InputHandle();
};

}