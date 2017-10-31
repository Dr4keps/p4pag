#include "PagVAO.h"

#include <iostream>

PagVAO::PagVAO()
{
	vao = 0;

	vbo_posnorm = 0;
	vbo_tangents = 0;
	vbo_textcoord = 0;

	ibo_cloudPoint = 0;
	ibo_triangles = 0;
	ibo_wireFrame = 0;

	createVAO();

}


PagVAO::~PagVAO()
{
}

void PagVAO::createVAO() {
	
	if (vao <= 0) {
		glGenVertexArrays(1, &vao);
		if (vao == 0) {
			std::cout << "Cannot create VAO: \n" << std::endl;
		}
	}
}


bool PagVAO::createVBOPosNorm() {

	if (vao >= 0) {
		// - Se activa el vao para este vbo.
		glBindVertexArray(vao);
		
		// - Se genera el VBO y se activa
		glGenBuffers(1, &vbo_posnorm);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_posnorm);

		// - Se describe cómo es la geometría que hay en ese array 

		// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el  
		//   layout (location=0) en el shader, en concreto la posición.
		glEnableVertexAttribArray(0);
		// - Aquí se describen las características del puntero que permite a la GPU acceder a las 
		//   posiciones
		glVertexAttribPointer(1, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(PagPosNorm),
			((GLubyte *)NULL + (sizeof(glm::vec3))));

		return true;
	}
	else {
		std::cout << "Cannot create VBO for this VAO \n" << std::endl;
		return false;
	}

}


bool PagVAO::fillVBOPosNorm(std::vector<PagPosNorm> ppn) {

	if (vao > 0 && vbo_posnorm > 0) {
		// - Se activa el VAO que contiene al VBO que se quiere rellenar con su array de geometría
		glBindVertexArray(vao);

		// - Se activa el VBO que se quiere rellenar
		glBindBuffer(GL_ARRAY_BUFFER, vbo_posnorm);

		glBufferData(GL_ARRAY_BUFFER, ppn.size() * sizeof(ppn), ppn.data(), GL_STATIC_DRAW);

		return true;
	}
	else {
		std::cout << "Cannot fill positions and normals VBO" << std::endl;
	}
	
}


bool PagVAO::createIBOCloudPoint() {
	if (vao >= 0) {
		// - Se activa el vao para este vbo.
		glBindVertexArray(vao);

		// - Se genera el VBO y se activa
		glGenBuffers(1, &ibo_cloudPoint);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_posnorm);

		return true;
	}
	else {
		std::cout << "Cannot create IBO for this VAO \n" << std::endl;
		return false;
	}
}