#pragma once

#include <stdint.h>

class Quad
{
public:
	Quad();
	~Quad();

	bool Initialize();

	void Draw();

	static void Setup();
	static void Clean();

private:
	uint32_t m_vboId;
};

