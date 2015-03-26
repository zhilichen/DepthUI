#include "DepthUI/DepthUI.h"

int glut_window_;
DepthUI::System * depthui = NULL;

void h_Display(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw sth
	glPushMatrix();

	glColor4f(1.0, 0, 0, 1);
	glutWireCube(0.5);

	glPopMatrix();
	// 
	depthui->Render();

	glutSwapBuffers();
}

void h_Reshape(int width, int height)
{
	depthui->SetViewport(0, 0, width, height);

	glViewport(0, 0, width, height);
}

void h_Idle(void)
{
	glutPostRedisplay();
}


void h_Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			depthui->InputHandle()->InjectMouseDown(x, y, DepthUI::MOUSE_LEFT_BUTTON);
		}
		else if (state == GLUT_UP)
		{
			depthui->InputHandle()->InjectMouseUp(x, y, DepthUI::MOUSE_LEFT_BUTTON);
		}
	}
	
}

void startGLUT(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(1600, 900);
	glutInitWindowPosition(1, 1);

	glutSetOption(GLUT_RENDERING_CONTEXT, GLUT_USE_CURRENT_CONTEXT);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glut_window_ = glutCreateWindow("UI Test");

	//
	// register glut callbacks of main window
	//glutCloseFunc(GLUTCallback::Close);
	glutDisplayFunc(h_Display);
	glutReshapeFunc(h_Reshape);
	glutIdleFunc(h_Idle);
	//glutTimerFunc(10,GLUTCallback::Timer,0);
	//glutTimerFunc(10,GLUTCallback::Timer,1);
	//glutKeyboardFunc(GLUTCallback::Keyboard);
	//glutSpecialFunc(GLUTCallback::SpecialKey);
	//glutSpecialUpFunc(GLUTCallback::SpecialKeyUp);
	glutMouseFunc(h_Mouse);
	//glutMouseWheelFunc(GLUTCallback::Wheel);
	//glutMotionFunc(GLUTCallback::Motion);
	//glutPassiveMotionFunc(GLUTCallback::PassiveMotion);

	//
	// enable extensions
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		throw "Error initializing GLEW";
	}
	if (!GLEW_VERSION_2_1)
	{
		throw "Fatal Error: OpenGL 2.1 is required";
	}

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	fprintf(stdout, "OpenGL version: %s\n", glGetString(GL_VERSION));

	// shader source initializations
	//std::string shaderListFile;
	//config_.get("shader_config_file", shaderListFile);
	//shaderManager_.LoadShaderConfig(shaderListFile);

	depthui = new DepthUI::System();
	depthui->Initialize(glut_window_);

	DepthUI::ViewLayout::SingleDimensionLayoutSettings setting_x, setting_y;

	DepthUI::Frame * frame = new DepthUI::Frame("frame_right", depthui->ControlRoot());

	setting_x.size_style = DepthUI::SIZE_RELATIVE;
	setting_x.size_relative = 0.3;
	setting_x.alignment_style = DepthUI::ALIGNMENT_RIGHT;
	setting_x.offset_style = DepthUI::OFFSET_ABSOLUTE;
	setting_x.offset_absolute = 10;

	setting_y.size_style = DepthUI::SIZE_RELATIVE;
	setting_y.size_relative = 0.9;
	setting_y.alignment_style = DepthUI::ALIGNMENT_TOP;
	setting_y.offset_style = DepthUI::OFFSET_ABSOLUTE;
	setting_y.offset_absolute = 10;

	frame->SetLayout(setting_x, setting_y);

	DepthUI::Button * button = new DepthUI::Button("button_test", frame);
	
	setting_x.size_style = DepthUI::SIZE_ABSOLUTE;
	setting_x.size_absolute = 300;
	setting_x.alignment_style = DepthUI::ALIGNMENT_LEFT;
	setting_x.offset_style = DepthUI::OFFSET_ABSOLUTE;
	setting_x.offset_absolute = 20;

	setting_y.size_style = DepthUI::SIZE_ABSOLUTE;
	setting_y.size_absolute = 200;
	setting_y.alignment_style = DepthUI::ALIGNMENT_TOP;
	setting_y.offset_style = DepthUI::OFFSET_ABSOLUTE;
	setting_y.offset_absolute = 20;

	button->SetLayout(setting_x, setting_y);

	depthui->WriteLog("Test");

	//
	// start glut main loop
	glutMainLoop();

}

int main(int argc, char **argv)
{
	DepthUI::System dui;

	startGLUT(argc, argv);

	return 0;
}