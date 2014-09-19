#include "BaseScene.h"
#include "../GameObject.h"
#include "../Environment.h"
#include "../AnimCameraManager.h"
#include <Graphics/AnimationData.h>
#include <Graphics/Animation.h>
#include <Graphics/Content/Content.h>
#include <stdint.h>

BaseScene::BaseScene() :
	m_camerasManager(NULL),
	m_camerasAnimation(NULL),
	m_activeCamera(NULL)
{
};

const std::vector<GameObject*>& BaseScene::GetGameObjects() const
{
	return m_gameObjects;
}

const std::vector<Renderable*>& BaseScene::GetRenderables() const
{
	return m_renderables;
}

ICamera* BaseScene::GetCamera() const
{
	return m_activeCamera;
}

bool BaseScene::Initialize()
{
	InitializeSubScene();

	LoadCameras();

	for (uint32_t i = 0; i < m_gameObjects.size(); i++)
	{
		const std::vector<Renderable*>& renderables = m_gameObjects[i]->GetRenderables();
		m_renderables.insert(m_renderables.end(), renderables.begin(), renderables.end());
	}

	return true;
}

float BaseScene::GetStartTime() const
{
	return m_camerasManager->GetCamera(0)->GetStartTime();
}

float BaseScene::GetEndTime() const
{
	return m_camerasManager->GetCamera(m_camerasManager->GetCamerasCount() - 1)->GetLastKeyFrameTime();
}

bool BaseScene::Update(float time, float deltaTime)
{
	m_camerasAnimation->Update(time, sm::Matrix::IdentityMatrix(), deltaTime);
	m_activeCamera = m_camerasManager->GetActiveCamera(time);

	for (uint32_t i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->Update(time, deltaTime);

	return true;
}

void BaseScene::LoadCameras()
{
	AnimationData* camerasAnimationData = Content::Instance->Get<AnimationData>(m_name);
	if (camerasAnimationData == NULL)
		return;

	m_camerasAnimation = new Animation(camerasAnimationData);

	m_camerasManager = new AnimCameraManager();
	m_camerasManager->Load(Environment::GetInstance()->GetBasePath() + "cameras\\" + m_name + ".cam", m_camerasAnimation);
}

GameObject* BaseScene::FindGameObject(const std::string& name)
{
	for (uint32_t i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i]->GetName() == name)
			return m_gameObjects[i];
	}

	return NULL;
}
