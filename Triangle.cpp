#include "Triangle.h"

void Triangle::Initalize()
{
	Center = (Vertices[0] + Vertices[1] + Vertices[2]) * (1.0f / 3.0f);

	Normal = -((Vertices[1] - Vertices[0]) * (Vertices[2] - Vertices[0])).GetNormalized();
};

void Triangle::Transform(const sm::Matrix& transform)
{
	Vertices[0] = transform * Vertices[0];
	Vertices[1] = transform * Vertices[1];
	Vertices[2] = transform * Vertices[2];
}
