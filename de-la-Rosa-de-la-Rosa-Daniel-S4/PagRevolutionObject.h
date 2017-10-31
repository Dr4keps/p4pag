#pragma once
#include <glm.hpp>
#include <vector>
#include <iostream>
#include "PagSubdivisionProfile.h"
#include "PagEnumerations.h"
#include "PagStructures.h"
#include "PagVAO.h"



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
	glm::vec2* getTextureCoords(PagRevObjParts part);
	GLuint* getIndices4PointCloud(PagRevObjParts part);
	
	void drawAsPointCloud(PagRevObjParts part);

private:
	void createTopology4PointCloud();
	void createTopology4TriangleMesh();

	PagSubdivisionProfile sp;
	unsigned int subdivisions;
	unsigned int slices;

	std::vector<PagPosNorm> pos_norm_body;
	std::vector<PagPosNorm> pos_norm_bottom_fan;
	std::vector<PagPosNorm> pos_norm_top_fan;

	std::vector<glm::vec3> tangents_body;
	std::vector<glm::vec3> tangents_bottom_fan;
	std::vector<glm::vec3> tangents_top_fan;

	std::vector<glm::vec2> texcoord_body;
	std::vector<glm::vec2> texcoord_bottom_fan;
	std::vector<glm::vec2> texcoord_top_fan;

	std::vector<GLuint> i4PointCloud_bottomFan;
	std::vector<GLuint> i4PointCloud_body;
	std::vector<GLuint> i4PointCloud_topFan;

	std::vector<GLuint> i4TriangleMesh_body;
	std::vector<GLuint> i4TriangleMesh_bottom_fan;
	std::vector<GLuint> i4TriangleMesh_top_fan;

	PagVAO vaoBody;

};

