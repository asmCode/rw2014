#pragma once

#include <vector>

class GameObject;
class Renderable;
class ICamera;

class BaseScene
{
	friend class SceneLoader;

public:
	BaseScene() {};
	virtual ~BaseScene() {}

	bool Initialize();

	virtual bool Update(float time, float deltaTime);

	virtual void Enter() {};
	virtual void Leave() {};

	const std::vector<GameObject*>& GetGameObjects() const;
	const std::vector<Renderable*>& GetRenderables() const;

	virtual ICamera* GetCamera() const;

protected:
	std::string m_name;

	std::vector<GameObject*> m_gameObjects;
	std::vector<Renderable*> m_renderables;

	virtual void InitializeSubScene() = 0;
};

