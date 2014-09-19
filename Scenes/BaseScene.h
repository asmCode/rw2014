#pragma once

#include <vector>

class GameObject;
class Renderable;
class ICamera;
class Animation;
class AnimCameraManager;

class BaseScene
{
	friend class SceneLoader;

public:
	BaseScene();
	virtual ~BaseScene() {}

	bool Initialize();

	virtual bool Update(float time, float deltaTime);

	virtual void Enter() {};
	virtual void Leave() {};

	const std::vector<GameObject*>& GetGameObjects() const;
	const std::vector<Renderable*>& GetRenderables() const;

	virtual ICamera* GetCamera() const;

	virtual float GetStartTime() const;
	virtual float GetEndTime() const;

protected:
	std::string m_name;

	std::vector<GameObject*> m_gameObjects;
	std::vector<Renderable*> m_renderables;

	AnimCameraManager* m_camerasManager;
	Animation* m_camerasAnimation;
	ICamera* m_activeCamera;

	virtual void InitializeSubScene() = 0;

	void LoadCameras();

	GameObject* FindGameObject(const std::string& name);
};

