#include "BaseScene.h"

const std::vector<GameObject*>& BaseScene::GetGameObjects() const
{
	return m_gameObjects;
}