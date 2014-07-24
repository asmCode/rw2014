#pragma once

#include <Math/Vec3.h>

class Triangle
{
public:
	sm::Vec3* Vertices;
	sm::Vec3 Normal;
	sm::Vec3 Center;

	void Initalize();
};
