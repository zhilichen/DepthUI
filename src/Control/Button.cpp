#include "DepthUI\Controls\Button.h"

namespace DepthUI{
Button::Button(std::string name, Control * parent) : Control(name, parent)
{

}


Button::~Button()
{
}

void Button::InitializeRenderObjects()
{
	shader_default_.Enable();

	DEPTHGL(glGenVertexArrays(1, &vao_box_));
	DEPTHGL(glBindVertexArray(vao_box_));

	float data_pos_box[4 * 3] = {	0.0, 0.0, z_order_,
		0.0, -1.0, z_order_,
		1.0, -1.0, z_order_,
		1.0, 0.0, z_order_ };
	glGenBuffers(1, &vbo_position_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), data_pos_box, GL_STATIC_DRAW);

	const int loc_vertices_box = shader_default_.GetAttributeLocation("VPosition");
	DEPTHGL(glEnableVertexAttribArray(loc_vertices_box));
	DEPTHGL(glVertexAttribPointer(loc_vertices_box, 3, GL_FLOAT, GL_FALSE, 0, 0));

	float data_color_box[4 * 4] = { 0.8, 0.8, 0.8, 1.0,
		0.8, 0.8, 0.8, 1.0,
		0.8, 0.8, 0.8, 1.0,
		0.8, 0.8, 0.8, 1.0};
	glGenBuffers(1, &vbo_color_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color_);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), data_color_box, GL_STATIC_DRAW);

	const int loc_color_box = shader_default_.GetAttributeLocation("VColor");
	DEPTHGL(glEnableVertexAttribArray(loc_color_box));
	DEPTHGL(glVertexAttribPointer(loc_color_box, 4, GL_FLOAT, GL_FALSE, 0, 0));

	uint data_quad[1 * 4] = { 0, 1, 2, 3};
	glGenBuffers(1, &vbo_element_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1 * 4 * sizeof(uint), data_quad, GL_STATIC_DRAW);

	DEPTHGL(glBindVertexArray(0));

	DEPTHGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	DEPTHGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	shader_default_.Disable();
}

void Button::Draw(bool draw_id)
{
	if (!is_render_ready_)
	{
		InitializeRenderObjects();
		is_render_ready_ = true;
	}
	if (is_render_ready_)
	{
		if (draw_id) glStencilFunc(GL_ALWAYS, ID_Internal_, 0xFF);

		shader_default_.Enable();

		shader_default_.SetTransformationMatrices(viewlayout_->transform_projection_.matrix(), viewlayout_->transform_modelview_.matrix());

		DEPTHGL(glBindVertexArray(vao_box_));
		DEPTHGL(glDrawElements(GL_QUADS, 4 * 1, GL_UNSIGNED_INT, (void*)0));
		DEPTHGL(glBindVertexArray(0));

		shader_default_.Disable();
	}

	Control::Draw(draw_id);
}

}