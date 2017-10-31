#pragma once
#include <Windows.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class PagVAO
{
public:
	PagVAO();
	~PagVAO();

private: 
	GLuint vao;

	GLuint vbo_bottom_fan;
	GLuint vbo_body;
	GLuint vbo_top_fan;

	GLuint ibo_bottom_fan;
	GLuint ibo_body;
	GLuint ibo_top_fan;
};

