#include "BaseScene.h"
#include "../GameObject.h"
#include <stdint.h>

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
	return NULL;
}

bool BaseScene::Initialize()
{
	InitializeSubScene();

	for (uint32_t i = 0; i < m_gameObjects.size(); i++)
	{
		const std::vector<Renderable*>& renderables = m_gameObjects[i]->GetRenderables();
		m_renderables.insert(m_renderables.end(), renderables.begin(), renderables.end());
	}

	return true;
}

bool BaseScene::Update(float time, float deltaTime)
{
	for (uint32_t i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->Update(time, deltaTime);

	return true;
}
