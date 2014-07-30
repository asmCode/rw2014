#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>

class Triangle
{
public:
	sm::Vec3* Vertices;
	//sm::Vec3 BaseVertices[3];
	sm::Vec3 Normal;
	sm::Vec3 Center;

	void Initalize();
	void Transform(const sm::Matrix& transform);
};
