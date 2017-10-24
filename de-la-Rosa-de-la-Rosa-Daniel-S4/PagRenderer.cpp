#include "PagRenderer.h"
#include <iostream>
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		//BORRAR
		prepareOpenGL();
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

void PagRenderer::prepareOpenGL()
{
	std::vector<glm::vec2> generatriz;
	glm::vec2 p1(0, 0);
	glm::vec2 p2(1, 1);
	glm::vec2 p3(2, 2);
	glm::vec2 p4(3, 3);
	//algo
	generatriz.push_back(p1);
	generatriz.push_back(p2);
	generatriz.push_back(p3);
	generatriz.push_back(p4);

	PagRevolutionObject ro(generatriz, 2, 10);
	ro.getPositionsAndNormals(PAG_BOTTOM_FAN);
	ro.getPositionsAndNormals(PAG_BODY);

}

