#include "PagVAO.h"

#include <iostream>

PagVAO::PagVAO()
{
	vao = 0;

	vbo_body = 0;
	vbo_bottom_fan = 0;
	vbo_top_fan = 0;

	ibo_body = 0;
	ibo_bottom_fan = 0;
	ibo_top_fan = 0;

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


void PagVAO::createVBO(PagRevObjParts part) {

	if (vao >= 0) {
		glGenBuffers(1, &vbo_body);
	}

}