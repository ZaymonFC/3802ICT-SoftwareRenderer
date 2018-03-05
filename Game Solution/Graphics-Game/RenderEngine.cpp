#include "stdafx.h"
#include "RenderEngine.h"
#include "libs/glut.h"


RenderEngine::RenderEngine(const int height, const int width)
{
	height_ = height;
	width_ = width;
	InitGlut();
}

RenderEngine::~RenderEngine()
{
	return;
}


void RenderEngine::Render()
{
	return;
}

void RenderEngine::InitGlut()
{
	// Spoof the input parameters
	char applicationName[] = "Graphics Game";
	char * spoof_argv[] = { applicationName, nullptr };
	auto spoof_argc = 1;

	glutInit(&spoof_argc, spoof_argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(RenderEngine::width_, RenderEngine::height_);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(spoof_argv[0]);

	
	auto clearColour = 0.0;


}


