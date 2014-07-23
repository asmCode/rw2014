#pragma once

#include <Math/Vec3.h>

class TriangleShader
{
public:
	TriangleShader();
	virtual ~TriangleShader() {}

	virtual void ProcessTriangle(
		float time,
		float deltaTime,
		const sm::Vec3*& vertices) = 0;
};
