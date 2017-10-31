#pragma once
#include <Windows.h>
#include <vector>
#include <glm.hpp>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "PagStructures.h"

class PagVAO
{
public:
	PagVAO();
	~PagVAO();

	bool createVBOPosNorm();
	bool createVBOTangents();
	bool createVBOTexCoord();

	bool fillVBOPosNorm(std::vector<PagPosNorm> ppn);
	bool fillVBOTangents();
	bool fillVBOTexCoord();

	bool createIBOCloudPoint();

private: 
	GLuint vao;

	GLuint vbo_posnorm;
	GLuint vbo_tangents;
	GLuint vbo_texcoord;

	GLuint ibo_cloudPoint;
	GLuint ibo_wireFrame;
	GLuint ibo_triangles;

	void createVAO();
};

