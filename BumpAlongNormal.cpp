#include "BumpAlongNormal.h"

void BumpAlongNormal::ProcessTriangle(
	float time,
	float deltaTime,
	sm::Vec3 vertices[3],
	sm::Vec3 normals[3]) const
{
	sm::Vec3 norm = (normals[0] + normals[1] + normals[2]).GetNormalized() * deltaTime * 0.2f;
	vertices[0] += norm;
	vertices[1] += norm;
	vertices[2] += norm;
}
