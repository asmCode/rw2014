#pragma once

#include "TriangleShader.h"

class BumpAlongNormal : public TriangleShader
{
public:
	void ProcessTriangle(
		float time,
		float deltaTime,
		sm::Vec3 vertices[3],
		sm::Vec3 normals[3]) const;
};
