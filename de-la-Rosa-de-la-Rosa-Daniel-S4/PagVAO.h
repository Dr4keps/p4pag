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

	bool fillVBOPosNorm(std::vector<PagPosNorm> ppn);
	bool fillVBOTangents(std::vector<glm::vec3> tangents);
	bool fillVBOTexCoord(std::vector<glm::vec2> tc);

	bool fillIBO4PointCloud(std::vector<GLuint> indices4Points);
	bool fillIBO4WireFrame(std::vector<GLuint> indices4WF);


	void drawAsPointCloud();

private: 
	GLuint vao;

	GLsizei numberOfIndices4Points;
	GLsizei numberOfIndices4WF;

	GLuint vbo_posnorm;
	GLuint vbo_tangents;
	GLuint vbo_texcoord;

	GLuint ibo_cloudPoint;
	GLuint ibo_wireFrame;
	GLuint ibo_triangles;

	bool createVAO();

	void createVBOPosNorm();
	void createVBOTangents();
	void createVBOTexCoord();

	void createIBO4PointCloud();
};

