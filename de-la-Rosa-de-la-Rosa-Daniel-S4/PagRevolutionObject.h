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

	std::vector<PagPosNorm> pos_norm_body;
	std::vector<PagPosNorm> pos_norm_bottom_fan;
	std::vector<PagPosNorm> pos_norm_top_fan;

	std::vector<glm::vec3> tangents_body;
	std::vector<glm::vec3> tangents_bottom_fan;
	std::vector<glm::vec3> tangents_top_fan;



};

