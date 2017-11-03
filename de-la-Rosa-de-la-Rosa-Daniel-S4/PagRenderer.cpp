#include "PagRenderer.h"



PagRenderer* PagRenderer::instance = nullptr;

PagRenderer::PagRenderer()
{
	posCamera = glm::vec3(0.0f, 0.0f, 10.0f);
	lookAtCamera = glm::vec3(0.0f, 0.0f, 0.0f);
	upCamera = glm::vec3(0.0f, 1.0f, 0.0f);
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
	//glm::vec2 p1(0, 0);
	/*glm::vec2 p2(1, 0);
	glm::vec2 p3(1, 2);
	glm::vec2 p4(2, 3);
	glm::vec2 p5(0, 4);*/

	glm::vec2 p2(0, 0);
	glm::vec2 p3(1, 0);
	glm::vec2 p4(1, 2);
	glm::vec2 p5(2, 3);

	//generatriz.push_back(p1);
	generatriz.push_back(p2);
	generatriz.push_back(p3);
	generatriz.push_back(p4);
	generatriz.push_back(p5);

	PagRevolutionObject revolutionObj(generatriz, 2, 6);

	std::cout << "PosCamara: " << posCamera.x << ", " << posCamera.y << ", " << posCamera.z << std::endl;

	glm::mat4 perspective = glm::perspective(glm::radians(60.0f), 4.0f/3.0f, 0.1f, 100.0f);
	glm::mat4 vision = glm::lookAt(posCamera, lookAtCamera, upCamera);

	this->pointShader.use();
	this->pointShader.setUniform("pointSize", 5.0f);
	this->pointShader.setUniform("vColor", glm::vec3(0.0f, 0.0f, 1.0f));
	this->pointShader.setUniform("mModelViewProj", perspective * vision);

	revolutionObj.drawAsPointCloud(PAG_BODY);
	//revolutionObj.drawAsPointCloud(PAG_TOP_FAN);

	//revolutionObj.getPositionsAndNormals(PAG_TOP_FAN);
	//revolutionObj.getIndices4PointCloud(PAG_TOP_FAN);

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
			std::cout << "He PULSADO EL 1" << std::endl;
			posCamera = glm::vec3(0.0f, 0.0f, 10.0f);
			lookAtCamera = glm::vec3(0.0f, 0.0f, 0.0f);
			upCamera = glm::vec3(0.0f, 1.0f, 0.0f);
			this->refreshCallback();
		}
		break;
	case 50: //Tecla del número 2.
		if (action == GLFW_PRESS) {
			std::cout << "He PULSADO EL 2" << std::endl;
			posCamera = glm::vec3(0.0f, 10.0f, 0.0f);
			lookAtCamera = glm::vec3(0.0f, 0.0f, 0.0f);
			upCamera = glm::vec3(0.0f, 0.0f, 1.0f);
			this->refreshCallback();
		}
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

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

	pointShader.createShaderProgram("pointShader");
	
}


