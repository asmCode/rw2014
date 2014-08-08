#pragma once

class GameObject;

class GraphicsEngine
{
public:
	GraphicsEngine();
	virtual ~GraphicsEngine();

	void RenderGameObjects(GameObject** gameObjects, int count);

private:
};
