#include "PagRenderer.h"



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

	std::vector<glm::vec2> generatriz;
	glm::vec2 p1(0, 0);
	glm::vec2 p13(0.5, 0);
	glm::vec2 p2(1, 0);
	glm::vec2 p21(1, 0.5);
	glm::vec2 p22(1, 1);
	glm::vec2 p23(1, 1.75);
	glm::vec2 p24(1, 1.90);
	glm::vec2 p3(1, 2);
	glm::vec2 p34(1.1, 2);
	glm::vec2 p35(1.5, 2);
	glm::vec2 p36(2, 2);
	glm::vec2 p4(4, 2);
	glm::vec2 p5(0, 4);

	//glm::vec2 p1(0, 0);
	//glm::vec2 p2(1, 1);
	//glm::vec2 p3(2, 2);
	//glm::vec2 p4(3, 3);
	//glm::vec2 p5(4, 4);

	generatriz.push_back(p1);
	generatriz.push_back(p13);
	generatriz.push_back(p2);
	generatriz.push_back(p21);
	generatriz.push_back(p22);
	generatriz.push_back(p23);
	generatriz.push_back(p24);
	generatriz.push_back(p3);
	generatriz.push_back(p34);
	generatriz.push_back(p35);
	generatriz.push_back(p36);
	generatriz.push_back(p4);
	generatriz.push_back(p5);

	PagRevolutionObject revolutionObj(generatriz, 1, 10);

	glm::mat4 perspective = glm::perspective(glm::radians(90.0f), 4.0f/3.0f, 0.1f, 100.0f);
	glm::mat4 vision = glm::lookAt(posCamera, lookAtCamera, upCamera);

	if (drawPoints) {
		this->pointShader.use();
		this->pointShader.setUniform("pointSize", 5.0f);
		this->pointShader.setUniform("vColor", glm::vec3(0.0f, 0.0f, 1.0f));
		this->pointShader.setUniform("mModelViewProj", perspective * vision);

		revolutionObj.drawAsPoints(PAG_BODY);
		revolutionObj.drawAsPoints(PAG_BOTTOM_FAN);
		revolutionObj.drawAsPoints(PAG_TOP_FAN);
	}

	if (drawWireFrame) {
		this->wireFrameShader.use();
		this->wireFrameShader.setUniform("mModelViewProj", perspective * vision);
		this->wireFrameShader.setUniform("vColor", glm::vec3(1.0f, 0.0f, 1.0f));

		revolutionObj.drawAsLines(PAG_BODY);
		revolutionObj.drawAsLines(PAG_BOTTOM_FAN);
		revolutionObj.drawAsLines(PAG_TOP_FAN);
	}

	if (drawTriangles) {
		this->triangleShader.use();
		this->triangleShader.setUniform("mModelViewProj", perspective * vision);

		revolutionObj.drawAsTriangles(PAG_BODY);
		revolutionObj.drawAsTriangles(PAG_BOTTOM_FAN);
		revolutionObj.drawAsTriangles(PAG_TOP_FAN);
	}


}


void PagRenderer::framebufferSizeCallback(int width, int height)
{
	//Establecemos el viewport.
	glViewport(0, 0, width, height);
	std::cout << " PagRenderer::frameBufferSizeCallback called" << std::endl;
}


void PagRenderer::keyCallback(int key, int scancode, int action, int mods)
{
	switch (key) {
	case 49: //Tecla del n�mero 1.
		if (action == GLFW_PRESS) {
			posCamera = glm::vec3(0.0f, 0.0f, 10.0f);
			lookAtCamera = glm::vec3(0.0f, 0.0f, 0.0f);
			upCamera = glm::vec3(0.0f, 1.0f, 0.0f);
			this->refreshCallback();
		}
		break;
	case 50: //Tecla del n�mero 2.
		if (action == GLFW_PRESS) {
			posCamera = glm::vec3(0.0f, 10.0f, 0.0f);
			lookAtCamera = glm::vec3(0.0f, 0.0f, 0.0f);
			upCamera = glm::vec3(0.0f, 0.0f, 1.0f);
			this->refreshCallback();
		}
		break;

	case 87: //Tecla W.
		if (action == GLFW_PRESS) {
			this->drawWireFrame = !this->drawWireFrame;
		}
		this->refreshCallback();
		break;

	case 80: //Tecla P.
		if (action == GLFW_PRESS) {
			this->drawPoints = !this->drawPoints;
		}
		this->refreshCallback();
		break;

	case 84: //Tecla T.
		if (action == GLFW_PRESS) {
			this->drawTriangles = !this->drawTriangles;
		}
		this->refreshCallback();
		break;
	}

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


void PagRenderer::scrollCallback(double xoffset, double yoffset)
{
	std::cout << "Movida la rueda del raton " << xoffset <<
		" Unidades en horizontal y " << yoffset << " unidades en vertical" << std::endl;

}

// - Prepara la escena para su visualizaci�n. Solo se llama una �nica vez.
void PagRenderer::prepareOpenGL()
{
	posCamera = glm::vec3(0.0f, 0.0f, 10.0f);
	lookAtCamera = glm::vec3(0.0f, 0.0f, 0.0f);
	upCamera = glm::vec3(0.0f, 1.0f, 0.0f);

	drawPoints = false;
	drawWireFrame = false;
	drawTriangles = true;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

	pointShader.createShaderProgram("pointShader");
	wireFrameShader.createShaderProgram("wireFrameShader");
	triangleShader.createShaderProgram("triangleShader");
	
}


