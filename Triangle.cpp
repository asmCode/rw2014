#include "Triangle.h"

void Triangle::Initalize()
{
	Center = (Vertices[0] + Vertices[1] + Vertices[2]) * (1.0f / 3.0f);
};
