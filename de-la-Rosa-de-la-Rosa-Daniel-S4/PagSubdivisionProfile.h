#pragma once
#include <glm.hpp>
#include <gtc/epsilon.hpp>
#include <vector>
#include <iostream>

class PagSubdivisionProfile
{
public:
	PagSubdivisionProfile(std::vector<glm::vec2> points);
	~PagSubdivisionProfile();

	PagSubdivisionProfile subdivide(unsigned int times);

	bool isValid();
	bool hasTopFan();
	bool hasBottomFan();
	bool hasBody();
	std::vector<glm::vec2> getPoints();

private:
	std::vector<glm::vec2> validPoints;
	bool valid;
	bool topFan;
	bool bottomFan;
	bool body;

	bool equalZeroE(float a);
	void initialize();

};

