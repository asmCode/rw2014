#include "BumpAlongNormal.h"

void BumpAlongNormal::ProcessTriangle(
	float time,
	float deltaTime,
	sm::Vec3 vertices[3])
{
	sm::Vec3 norm = (vertices[0] + vertices[1] + vertices[2]).GetNormalized();
	vertices[0] += norm * deltaTime;
	vertices[1] += norm * deltaTime;
	vertices[2] += norm * deltaTime;
}
