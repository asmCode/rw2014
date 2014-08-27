#pragma once

#include <vector>

class GameObject;
class Renderable;

class BaseScene
{
public:
	BaseScene() {};
	virtual ~BaseScene() {}

	virtual bool Initialize() = 0;

	virtual bool Update(float time, float deltaTime) = 0;

	virtual void Enter() {};
	virtual void Leave() {};

	const std::vector<GameObject*>& GetGameObjects() const;
	const std::vector<Renderable*>& GetRenderables() const;

protected:
	std::vector<GameObject*> m_gameObjects;
	std::vector<Renderable*> m_renderables;
};

