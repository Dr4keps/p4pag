#pragma once

#include "PagShaderProgram.h"
#include "PagRevolutionObject.h"

// - La clase PagRenderer encapsula toda la gestión del área de dibujo
// OpenGL, eventos, etc.
// Se implementa como un singleton para que pueda ser llamada desde las
// funciones callback del main.cpp

class PagRenderer
{
public:
	~PagRenderer();

	static PagRenderer* getInstance();

	void refreshCallback();
	void framebufferSizeCallback(int width, int height);
	void keyCallback(int key, int scancode, int action, int mods);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void prepareOpenGL();

private:
	PagRenderer();

	static PagRenderer *instance;

	PagShaderProgram pointShader;

};

