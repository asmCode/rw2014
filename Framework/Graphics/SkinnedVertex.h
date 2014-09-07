#pragma once

#include "../Math/Vec3.h"
#include <stdint.h>

class SkinnedVertex
{
public:
	sm::Vec3 position;
	uint8_t boneIndex[4];
	float weight[4];
};

