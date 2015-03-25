#pragma once
#include "Base.h"

namespace DepthUI{

class DEPTHUI_API Renderer
{
public:
	uint viewport_pos_x;
	uint viewport_pos_y;
	uint viewport_size_x_absolute;
	uint viewport_size_y_absolute;
public:
	Renderer();
	~Renderer();
	
};

}