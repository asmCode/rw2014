#pragma once

class BaseMesh
{
public:
	BaseMesh() {};
	virtual ~BaseMesh() {}

	virtual void Draw() = 0;
};
