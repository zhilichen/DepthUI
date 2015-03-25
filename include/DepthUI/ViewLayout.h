#pragma once
#include "Base.h"

namespace DepthUI{

class DEPTHUI_API ViewLayout
{
public:
	typedef struct SingleDimensionLayout
	{
		int offset_absolute;
		float offset_relative;
		uint size_absolute;
		float size_relative;
		int alignment_style;
		int size_style;
		int offset_style;

		SingleDimensionLayout()
		{
			alignment_style = ALIGNMENT_NONE;
			size_style = SIZE_ABSOLUTE;
			offset_style = OFFSET_ABSOLUTE;
		}
	} SingleDimensionLayoutSettings;
public:
	SingleDimensionLayoutSettings layout_settings_x;
	SingleDimensionLayoutSettings layout_settings_y;
	int offset_global_x;
	int offset_global_y;
	int size_global_x;
	int size_global_y;
	int window_width;
	int window_height;
	Transformation transform_projection_;
	Transformation transform_modelview_;
	bool is_root;
public:
	ViewLayout()
	{
		is_root = false;
	}
	ViewLayout(const SingleDimensionLayout & layout_x, const SingleDimensionLayout & layout_y, ViewLayout * view_parent = NULL)
	{
		is_root = false;
		layout_settings_x = layout_x;
		layout_settings_y = layout_y;

		if (view_parent == NULL)
		{
			offset_global_x = 0;
			offset_global_y = 0;
			size_global_x = layout_settings_x.size_absolute;
			size_global_y = layout_settings_y.size_absolute;
		}
		else
		{
			RefreshGlobalLayout(*view_parent);
		}
	}
	~ViewLayout()
	{

	}
	void SetDimensionLayout(int dimCode, const SingleDimensionLayout & layout)
	{
		if (dimCode == 0) layout_settings_x = layout;
		else layout_settings_y = layout;
	}
	void RefreshGlobalLayout(const ViewLayout & view_parent)
	{
		window_width = view_parent.window_width;
		window_height = view_parent.window_height;

		computeGlobalLayoutOneDimension(
			view_parent.offset_global_x,
			view_parent.size_global_x,
			layout_settings_x,
			offset_global_x,
			size_global_x);

		computeGlobalLayoutOneDimension(
			view_parent.offset_global_y,
			view_parent.size_global_y,
			layout_settings_y,
			offset_global_y,
			size_global_y);

		// recompute projection
		// re-generate modelview matrix
		transform_modelview_.setIdentity();
		transform_modelview_.matrix()(2, 2) = -1;
		transform_modelview_.translate(Vec3(offset_global_x, window_height - offset_global_y, 0.0));
		transform_modelview_.scale(Vec3(size_global_x, size_global_y, 1.0));

		if (view_parent.is_root)
		{
			// compute ortho projection
			double left = 0;
			double right = size_global_x;
			double bottom = 0;
			double top = size_global_y;
			double zNear = 0;
			double zFar = -1000;

			Mat4 matrix_new;
			matrix_new.setZero();
			matrix_new(0, 0) = 2.0f / (right - left);
			matrix_new(1, 1) = 2.0f / (top - bottom);
			matrix_new(2, 2) = -2.0f / (zFar - zNear);
			matrix_new(0, 3) = -(right + left) / (right - left);
			matrix_new(1, 3) = -(top + bottom) / (top - bottom);
			matrix_new(2, 3) = -(zFar + zNear) / (zFar - zNear);
			matrix_new(3, 3) = 1.0f;

			transform_projection_.matrix() = matrix_new;
		}
		else
		{
			transform_projection_ = view_parent.transform_projection_;
		}
	}
private:
	void computeGlobalLayoutOneDimension(int offset_parent, int size_parent, const SingleDimensionLayoutSettings& layout_settings,
		int & offset_global, int & size_global)
	{
		if (layout_settings.size_style==SIZE_RELATIVE) 
			size_global = size_parent * std::min(layout_settings.size_relative, 1.0f);
		else
			size_global = std::min(size_parent, (int)layout_settings.size_absolute);

		int offset_amount = 0;
		if (layout_settings.offset_style == OFFSET_RELATIVE)
			offset_amount = size_parent * std::min(layout_settings.offset_relative, 1.0f);
		else 
			offset_amount = std::min(size_parent, (int)layout_settings.offset_absolute);

		if (layout_settings.alignment_style == ALIGNMENT_NONE)
		{
			offset_global = offset_parent + offset_amount;
		}
		else if (layout_settings.alignment_style == ALIGNMENT_LEFT)
		{
			offset_global = offset_parent + offset_amount;
		}
		else if (layout_settings.alignment_style == ALIGNMENT_CENTER)
		{
			offset_global = offset_parent + size_parent / 2 - size_global / 2 + offset_amount;
		}
		else if (layout_settings.alignment_style == ALIGNMENT_RIGHT)
		{
			offset_global = offset_parent + size_parent - size_global - offset_amount;
		}
	}
};

}