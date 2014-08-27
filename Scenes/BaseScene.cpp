#include "BaseScene.h"

const std::vector<GameObject*>& BaseScene::GetGameObjects() const
{
	return m_gameObjects;
}

const std::vector<Renderable*>& BaseScene::GetRenderables() const
{
	return m_renderables;
}
