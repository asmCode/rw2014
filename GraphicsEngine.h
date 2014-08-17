#pragma once

#include <vector>

class GameObject;

class GraphicsEngine
{
public:
	GraphicsEngine();
	virtual ~GraphicsEngine();

	void RenderGameObjects(const std::vector<GameObject*>& gameObjects);

private:
};
