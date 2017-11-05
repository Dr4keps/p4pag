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

	glm::mat4 perspective = glm::perspective(glm::radians(70.0f), 4.0f/3.0f, 0.1f, 100.0f);
	glm::mat4 vision = glm::lookAt(posCamera, lookAtCamera, upCamera);

	if (drawPoints) {
		this->pointShader.use();
		this->pointShader.setUniform("pointSize", 5.0f);
		this->pointShader.setUniform("vColor", glm::vec3(0.0f, 0.0f, 1.0f));
		this->pointShader.setUniform("mModelViewProj", perspective * vision);

		revolutionObj->drawAsPoints(PAG_BODY);
		revolutionObj->drawAsPoints(PAG_BOTTOM_FAN);
		revolutionObj->drawAsPoints(PAG_TOP_FAN);
	}

	if (drawWireFrame) {
		this->wireFrameShader.use();
		this->wireFrameShader.setUniform("mModelViewProj", perspective * vision);
		this->wireFrameShader.setUniform("vColor", glm::vec3(1.0f, 0.0f, 1.0f));

		revolutionObj->drawAsLines(PAG_BODY);
		revolutionObj->drawAsLines(PAG_BOTTOM_FAN);
		revolutionObj->drawAsLines(PAG_TOP_FAN);
	}

	if (drawTriangles) {
		this->triangleShader.use();
		this->triangleShader.setUniform("mModelViewProj", perspective * vision);

		revolutionObj->drawAsTriangles(PAG_BODY);
		revolutionObj->drawAsTriangles(PAG_BOTTOM_FAN);
		revolutionObj->drawAsTriangles(PAG_TOP_FAN);
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
	case 49: //Tecla del número 1.
		if (action == GLFW_PRESS) {
			posCamera = glm::vec3(0.0f, 0.0f, 10.0f);
			lookAtCamera = glm::vec3(0.0f, 0.0f, 0.0f);
			upCamera = glm::vec3(0.0f, 1.0f, 0.0f);
			this->refreshCallback();
		}
		break;
	case 50: //Tecla del número 2.
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

	case 65: //Tecla A.
		if (action == GLFW_PRESS) {
			this->drawPoints = true;
			this->drawWireFrame = true;
			this->drawTriangles = true;
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

// - Prepara la escena para su visualización. Solo se llama una única vez.
void PagRenderer::prepareOpenGL()
{
	std::vector<glm::vec2> generatriz;
	glm::vec2 p1(0, 0);
	glm::vec2 p2(1, 0);
	glm::vec2 p3(1, 1);
	glm::vec2 p4(1, 1.75);
	glm::vec2 p5(1, 1.90);
	glm::vec2 p6(1, 2);
	glm::vec2 p7(1.1, 2);
	glm::vec2 p8(1.5, 2);
	glm::vec2 p9(2, 2);
	glm::vec2 p10(4, 2);
	glm::vec2 p11(0, 4);

	generatriz.push_back(p1);
	generatriz.push_back(p2);
	generatriz.push_back(p3);
	generatriz.push_back(p4);
	generatriz.push_back(p5);
	generatriz.push_back(p6);
	generatriz.push_back(p7);
	generatriz.push_back(p8);
	generatriz.push_back(p9);
	generatriz.push_back(p10);
	generatriz.push_back(p11);

	this->revolutionObj = new PagRevolutionObject(generatriz, 2, 100);

	posCamera = glm::vec3(0.0f, 0.0f, 10.0f);
	lookAtCamera = glm::vec3(0.0f, 0.0f, 0.0f);
	upCamera = glm::vec3(0.0f, 1.0f, 0.0f);

	drawPoints = false;
	drawWireFrame = true;
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


