#pragma once
#include <glm.hpp>
#include <vector>
#include <iostream>
#include "PagSubdivisionProfile.h"
#include "PagEnumerations.h"
#include "PagStructures.h"


class PagRevolutionObject
{
public:
	PagRevolutionObject(std::vector<glm::vec2> points, unsigned int subdivisions,
		unsigned int slices);
	~PagRevolutionObject();

	bool isValid();
	bool has(PagRevObjParts part);
	struct PagPosNorm* getPositionsAndNormals(PagRevObjParts part);
	glm::vec3* getTangents(PagRevObjParts part);

private:
	PagSubdivisionProfile sp;
	unsigned int subdivisions;
	unsigned int slices;
	unsigned int index_bodyBegin;
	unsigned int index_bodyEnd;

	std::vector<PagPosNorm> pos_norm;
	std::vector<glm::vec3> tangents;
};

