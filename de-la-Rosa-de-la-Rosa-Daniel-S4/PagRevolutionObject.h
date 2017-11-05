#pragma once
#include <glm.hpp>
#include <vector>
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
	
	void drawAsPoints(PagRevObjParts part);
	void drawAsLines(PagRevObjParts part);
	void drawAsTriangles(PagRevObjParts part);

private:
	void createTopology4PointCloud();
	void createTopology4TriangleMesh();
	void createTopology4WireFrame();

	PagSubdivisionProfile sp;
	unsigned int subdivisions;
	unsigned int slices;

	std::vector<PagPosNorm> pos_norm_body;
	std::vector<PagPosNorm> pos_norm_bottomFan;
	std::vector<PagPosNorm> pos_norm_topFan;

	std::vector<glm::vec3> tangents_body;
	std::vector<glm::vec3> tangents_bottomFan;
	std::vector<glm::vec3> tangents_topFan;

	std::vector<glm::vec2> texcoord_body;
	std::vector<glm::vec2> texcoord_bottomFan;
	std::vector<glm::vec2> texcoord_topFan;

	std::vector<GLuint> i4PointCloud_bottomFan;
	std::vector<GLuint> i4PointCloud_body;
	std::vector<GLuint> i4PointCloud_topFan;

	std::vector<GLuint> i4TriangleMesh_body;
	std::vector<GLuint> i4TriangleMesh_bottomFan;
	std::vector<GLuint> i4TriangleMesh_topFan;

	std::vector<GLuint> i4WireFrame_body;
	std::vector<GLuint> i4WireFrame_bottomFan;
	std::vector<GLuint> i4WireFrame_topFan;

	PagVAO vaoBody;
	PagVAO vaoBottomFan;
	PagVAO vaoTopFan;

};

