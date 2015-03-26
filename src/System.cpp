#include "DepthUI\System.h"
#include "DepthUI\Control.h"
#include "DepthUI\InputHandler.h"
#include "DepthUI\Logger.h"

namespace DepthUI{

int System::count_systems_ = 0;

System::System()
{
	glut_windowid_ = 0;
	id_system_ = count_systems_;
	count_systems_++;
	parent_system_ = NULL;
	logger_ = NULL;
	count_controls_in_systems_ = 0;
	is_ID_map_dirty_ = true;
	is_ID_map_resized_ = true;
	tex_IDmap_ = 0;
	fbo_GUI_ = 0;
	view_window_ = NULL;
}


bool System::Initialize(int glut_window)
{
	glut_windowid_ = glut_window;

	view_window_ = new ViewLayout();
	view_window_->is_root = true;

	control_root_ = new Control("root");
	this->count_controls_in_systems_ = 1;

	ViewLayout::SingleDimensionLayoutSettings setting;
	setting.offset_style = OFFSET_ABSOLUTE;
	setting.offset_absolute = 0;
	setting.size_style = SIZE_RELATIVE;
	setting.size_relative = 1.0;
	control_root_->SetLayout(setting, setting);

	logger_ = new Logger("log.txt");

	control_root_->parent_system_ = this;
	control_root_->logger_ = logger_;

	return true;
}

System::~System()
{
}

void System::SetViewport(int x, int y, int w, int h)
{
	view_window_->offset_global_x = x;
	view_window_->offset_global_y = y;
	view_window_->size_global_x = w;
	view_window_->size_global_y = h;
	if (view_window_->window_width != w)
	{
		view_window_->window_width = w;
		is_ID_map_dirty_ = true;
		is_ID_map_resized_ = true;
	}
	if (view_window_->window_height != h)
	{
		view_window_->window_height = h;
		is_ID_map_dirty_ = true;
		is_ID_map_resized_ = true;
	}

	control_root_->Resize(view_window_);
}

void System::Render()
{
	if (is_ID_map_resized_)
	{
		// rebuild stencil fbo
		// set up fbo
		if (fbo_GUI_ != 0) glDeleteFramebuffers(1, &fbo_GUI_);
		if (tex_IDmap_ != 0) glDeleteTextures(1, &tex_IDmap_);
		if (tex_Color_ != 0) glDeleteTextures(1, &tex_Color_);

		DEPTHGL(glGenTextures(1, &tex_Color_));
		DEPTHGL(glBindTexture(GL_TEXTURE_2D, tex_Color_));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		DEPTHGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, view_window_->window_width, view_window_->window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));

		//DEPTHGL(glGenTextures(1, &tex_IDmap_));
		//DEPTHGL(glBindTexture(GL_TEXTURE_2D, tex_IDmap_));
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//DEPTHGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, view_window_->window_width, view_window_->window_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));

		glBindTexture(GL_TEXTURE_2D, 0);

		DEPTHGL(glGenFramebuffers(1, &fbo_GUI_));
		DEPTHGL(glBindFramebuffer(GL_FRAMEBUFFER, fbo_GUI_));

		GLuint rbo_depth_stencil;
		DEPTHGL(glGenRenderbuffers(1, &rbo_depth_stencil));
		DEPTHGL(glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_stencil));
		DEPTHGL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL,
			view_window_->window_width, view_window_->window_height));
		DEPTHGL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		DEPTHGL(glDrawBuffers(1, DrawBuffers));

		DEPTHGL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_stencil));
		DEPTHGL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_stencil));

		DEPTHGL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_Color_, 0));
		//DEPTHGL(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_IDmap_, 0));
		//DEPTHGL(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex_IDmap_, 0));

		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			WriteLog("Framebuffer creation success.");
			break;
		default:
			WriteLog("Framebuffer creation fail.");
		}

		DEPTHGL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	RenderControls();

	is_ID_map_dirty_ = false;
	is_ID_map_resized_ = false;

	DrawToScreen();
}

void System::RenderControls()
{
	DEPTHGL(glBindFramebuffer(GL_FRAMEBUFFER, fbo_GUI_));

	if (is_ID_map_dirty_)
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	control_root_->Draw(is_ID_map_dirty_);

	if (is_ID_map_dirty_)
	{
		glDisable(GL_STENCIL_TEST);

		//GLuint * mypixels = new GLuint[view_window_->window_width * view_window_->window_height];
		//DEPTHGL(glReadPixels(0, 0, view_window_->window_width, view_window_->window_height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, mypixels));

		//std::ofstream fout("stencil.txt");
		//for (int i = 0; i < view_window_->window_height; i++)
		//{
		//	for (int j = 0; j < view_window_->window_width; j++)
		//	{
		//		fout << (((mypixels[(i*view_window_->window_width + j)+0])) & 0xFF) << " ";
		//	}
		//	fout << std::endl;
		//}
		//fout.close();

		//delete[] mypixels;
	}

	DEPTHGL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void System::DrawToScreen()
{

}

void System::GetIDMouseOn(int x, int y)
{

}

Control * System::ControlRoot()
{
	return control_root_;
}
InputHandler * System::InputHandler()
{
	return input_handler_;
}

}