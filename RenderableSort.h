#pragma once

class Renderable;

class RenderableSort
{
public:
	bool operator() (const Renderable* renderable1, const Renderable* renderable2);
};
