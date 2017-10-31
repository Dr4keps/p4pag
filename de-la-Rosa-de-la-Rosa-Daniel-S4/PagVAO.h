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

	bool createIBO4PointCloud();

	bool fillIBO4PointCloud(std::vector<GLuint> indices4Points);

	void drawAsPointCloud();

private: 
	GLuint vao;
	GLsizei numberOfIndices4Points;

	GLuint vbo_posnorm;
	GLuint vbo_tangents;
	GLuint vbo_texcoord;

	GLuint ibo_cloudPoint;
	GLuint ibo_wireFrame;
	GLuint ibo_triangles;

	void createVAO();
};

