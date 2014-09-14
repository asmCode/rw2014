#pragma once

#include "ITriangleModificator.h"

class BlinkAtStartAndEnd : public ITriangleModificator
{
public:
	static BlinkAtStartAndEnd* GetInstance();

	void ProcessTriangle(float time, TrianglesRibbon* trianglesRibbon, int i);

private:
	static BlinkAtStartAndEnd* m_instance;

	BlinkAtStartAndEnd();
};
