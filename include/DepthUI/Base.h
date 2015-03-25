#pragma once
#ifdef DEPTHUI_EXPORTS
#define DEPTHUI_API __declspec(dllexport)
#else
#define DEPTHUI_API __declspec(dllimport)
#endif

#include "gl/glew.h"
#include "gl/freeglut.h"
#include "gl/glui.h"
#include <algorithm>
#include "Eigen/Core"
#include "Eigen/Geometry"

namespace DepthUI{

// defines
typedef unsigned int uint;
typedef Eigen::Vector2f Vec2;
typedef Eigen::Vector2i Vec2i;
typedef Eigen::Vector3f Vec3;
typedef Eigen::Vector4f Vec4;
typedef Eigen::Vector3i Vec3i;
typedef Eigen::Vector4i Vec4i;
typedef Eigen::Matrix<uint, 3, 1, 0, 3, 1>  Vec3u;
typedef Eigen::Matrix<uint, 4, 1, 0, 4, 1>  Vec4u;
typedef Eigen::Matrix4f Mat4;
typedef Eigen::Matrix3f Mat3;
typedef Eigen::Transform<float, 3, Eigen::Affine> Transformation;

// enums
enum EventType{
	EVENT_MOUSE_CLICK = 0,
	EVENT_MOUSE_DOWN,
	EVENT_MOUSE_UP,
	EVENT_WINDOW_RESIZE,
	EVENT_KEY_PRESS,
	EVENT_KEY_DOWN,
	EVENT_KEY_UP,
	NUM_EVENT_TYPES = EVENT_KEY_UP + 1
};

enum MouseType{
	MOUSE_LEFT_BUTTON,
	MOUSE_MIDDLE_BUTTON,
	MOUSE_RIGHT_BUTTON
};

enum MouseState{
	MOUSE_DOWN,
	MOUSE_UP
};

enum KeyState{
	KEY_DOWN,
	KEY_UP
};

enum AlignmentType{
	ALIGNMENT_NONE,
	ALIGNMENT_LEFT,
	ALIGNMENT_TOP = ALIGNMENT_LEFT,
	ALIGNMENT_CENTER,
	ALIGNMENT_RIGHT,
	ALIGNMENT_BOTTOM = ALIGNMENT_RIGHT
};

enum LayoutOption{
	SIZE_ABSOLUTE,
	OFFSET_ABSOLUTE = SIZE_ABSOLUTE,
	SIZE_RELATIVE,
	OFFSET_RELATIVE = SIZE_RELATIVE
};

#define DEPTHGL(x) x;{GLenum errorCode=glGetError();if (errorCode!=GL_NO_ERROR){printf("GL error: %d\n", errorCode);__debugbreak();}}

// forward definition
class System;

class Control;

class InputHandler;

class ViewLayout;

class Logger;
class Hierarchy;




}