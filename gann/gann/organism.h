#pragma once
#define GLOBAL_RADIUS 10
#include "ann.h"
#include <vector>
#include "vec2.h"
class organism
{
private:
	float position[2], rotation;
	int counter;
public:
	static std::vector<vec2> blocks;
	bool dead;
	ann* nn;
	organism(float x, float y, float rot);
	void update();
	int getFitness();
};