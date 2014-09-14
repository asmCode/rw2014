#pragma once

class TrianglesRibbon;

class ITriangleModificator
{
public:
	virtual void ProcessTriangle(float time, TrianglesRibbon* trianglesRibbon, int i) = 0;
};
