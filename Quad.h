#pragma once

#include <stdint.h>

class Quad
{
public:
	Quad();
	~Quad();

	bool Initialize(int width, int height);

	void Draw();

	static void Setup();
	static void Clean();

private:
	uint32_t m_vboId;
};

