#include "organism.h"
#include "graph.h"
#include <iostream>

std::vector<vec2> organism::blocks;

organism::organism(float x, float y, float rot)
{
	position[0] = x;
	position[1] = y;
	rotation = rot;
	dead = false;
}

#define degtorad(deg) ((deg)*3.14/180)
#define SENSOR_LENGTH 25
#define sqr(a) ((a)*(a))

void organism::update()
{
	graph::drawCircle(position[0], position[1], GLOBAL_RADIUS, RGB(255, 0, 250));
	graph::drawLine(position[0], position[1], position[0] + cos(rotation)*GLOBAL_RADIUS, position[1] + sin(rotation)*GLOBAL_RADIUS, RGB(100, 24, 255));
	if (dead) { return; }
	for (int i = 0; i < blocks.size(); i++)
	{
		if ((sqr(position[0] - blocks[i].x) + sqr(position[1] - blocks[i].y)) <= sqr(GLOBAL_RADIUS + GLOBAL_RADIUS))
		{
			dead = true;
		}
	}
	counter++;
	float sensors[5];
	for (int i = 0; i < 5; i++)
	{
		float s = sin(rotation + degtorad(-90 + i * 45));
		float c = cos(rotation + degtorad(-90 + i * 45));
		float sensorx = position[0] + c*SENSOR_LENGTH;
		float sensory = position[1] + s*SENSOR_LENGTH;
		graph::drawLine(position[0] + c*GLOBAL_RADIUS, position[1] + s*GLOBAL_RADIUS, sensorx, sensory, RGB(0, 0, 255));
		graph::drawCircle(sensorx, sensory, GLOBAL_RADIUS, RGB(0, 255, 0));
		float mindis = 1e10;
		float dis;
		for (int k = 0; k < blocks.size(); k++)
		{
			dis = sqr(blocks[k].x - sensorx) + sqr(blocks[k].y - sensory);
			if (dis <= sqr(GLOBAL_RADIUS + GLOBAL_RADIUS)) { if (dis < mindis) { mindis = dis; } }
		}
		sensors[i] = (mindis <= sqr(GLOBAL_RADIUS + GLOBAL_RADIUS)) ? ((GLOBAL_RADIUS * 2) - mindis) : 0; // TODO
	}
	nn->feedForward(sensors);
	rotation += (nn->os[1] - nn->os[0])*5.f;
	const float speed = 4.7f;
	position[0] += cos(rotation) * speed;
	position[1] += sin(rotation) * speed;
}

int organism::getFitness()
{
	return counter*counter;
}
