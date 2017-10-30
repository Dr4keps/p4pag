#include "PagShaderProgram.h"

PagShaderProgram::PagShaderProgram()
{
	handler = 0;
	linked = false;
	logString = "";
}


PagShaderProgram::~PagShaderProgram()
{
	glDeleteProgram(handler);
	linked = false;
	logString = "";
}


