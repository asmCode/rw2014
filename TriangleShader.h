#pragma once

#include <Math/Vec3.h>

class TriangleShader
{
public:
	virtual ~TriangleShader() {}

	virtual void ProcessTriangle(
		float time,
		float deltaTime,
		sm::Vec3 vertices[3]) = 0;
};
