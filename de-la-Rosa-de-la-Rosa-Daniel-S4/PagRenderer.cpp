#include "PagRenderer.h"
#include <iostream>
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "PagRevolutionObject.h"


PagRenderer* PagRenderer::instance = nullptr;

PagRenderer::PagRenderer()
{

}


PagRenderer::~PagRenderer()
{
}

PagRenderer* PagRenderer::getInstance() {

	if (!instance) {
		instance = new PagRenderer();
	}

	return instance;
}


void PagRenderer::refreshCallback() {

	std::cout << "Inicio" << std::endl;

	std::vector<glm::vec2> generatriz;
	glm::vec2 p1(0, 0);
	glm::vec2 p2(1, 0);
	glm::vec2 p3(1, 2);
	glm::vec2 p4(2, 3);

	//glm::vec2 p1(0, 0);
	//glm::vec2 p2(1, 1);
	//glm::vec2 p3(2, 2);
	//glm::vec2 p4(3, 3);

	generatriz.push_back(p1);
	generatriz.push_back(p2);
	generatriz.push_back(p3);
	generatriz.push_back(p4);

	PagRevolutionObject revolutionObj(generatriz, 0, 2);


	//BORRAR
	//glm::vec3 points[] = { glm::vec3(-1.0, -1.0, 0.0),
	//	glm::vec3(1.0, -1.0, 0.0),
	//	glm::vec3(-1.0, 1.0, 0.0),
	//	glm::vec3(1.0, 1.0, 0.0)
	//};

	//GLuint indices[] = { 0, 1, 2, 3 };

	//GLuint vao;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	//GLuint vbo;
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(glm::vec3), ((GLubyte *)NULL + (0)));
	//glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	//GLuint ibo;
	//glGenBuffers(1, &ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
	//	GL_STATIC_DRAW);

	//
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	//glBindVertexArray(vao);
	
	auto perspective = glm::perspective(glm::radians(90.0f), 4.0f/3.0f, 0.1f, 500.0f);
	auto vision = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	auto vp = perspective * vision;

	pointShader.use();
	pointShader.setUniform("pointSize", 5.0f);
	pointShader.setUniform("vColor", glm::vec3(0.0f, 0.0f, 1.0f));
	pointShader.setUniform("mModelViewProj", vp);

	/*glDrawElements(GL_POINTS, sizeof(indices) / sizeof(GLuint),
		GL_UNSIGNED_INT, NULL);*/

	revolutionObj.drawAsPointCloud(PAG_BODY);

	std::cout << "PagRenderer::refreshCallback called" << std::endl;
}


void PagRenderer::framebufferSizeCallback(int width, int height)
{
	//Establecemos el viewport.
	glViewport(0, 0, width, height);
	std::cout << " PagRenderer::frameBufferSizeCallback called" << std::endl;
}


void PagRenderer::keyCallback(int key, int scancode, int action, int mods)
{
	std::cout << "PagRenderer::keyCallback called" << std::endl;
}


void PagRenderer::mouseButtonCallback(int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		std::cout << "Pulsado el boton: " << button << std::endl;
	}
	else if (action == GLFW_RELEASE) {
		std::cout << "Soltado el boton: " << button << std::endl;
	}

	std::cout << "PagRenderer::mouseButtonCallback called" << std::endl;
}

//OK
void PagRenderer::scrollCallback(double xoffset, double yoffset)
{
	std::cout << "Movida la rueda del raton " << xoffset <<
		" Unidades en horizontal y " << yoffset << " unidades en vertical" << std::endl;

}

// - Prepara la escena para su visualización. Solo se llama una única vez.
void PagRenderer::prepareOpenGL()
{
	
	//std::cout << "Tapa abajo: " << std::endl;
	//ro.getIndices4PointCloud(PAG_BOTTOM_FAN);
	//std::cout << "Cuerpo: " << std::endl;
	//ro.getIndices4PointCloud(PAG_BODY);

	pointShader.createShaderProgram("pointShader");
	
}


