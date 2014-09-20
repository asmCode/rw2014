#include "ScenesManager.h"
#include "Scenes/Scene01.h"
#include "Scenes/Scene02.h"
#include "Scenes/Scene03.h"
#include "Scenes/Scene04.h"
#include "Scenes/Scene05.h"
#include "Scenes/Scene06.h"
#include "Scenes/Scene07.h"
#include "Scenes/Scene08.h"
#include "Scenes/Scene09.h"

ScenesManager::ScenesManager() :
	m_isSceneChanged(false),
	m_activeSceneIndex(0)
{
}

void ScenesManager::Initialize()
{
	m_scenes.push_back(new Scene01());
	m_scenes.push_back(new Scene02());
	m_scenes.push_back(new Scene03());
//	m_scenes.push_back(new Scene04());
	m_scenes.push_back(new Scene05());
	m_scenes.push_back(new Scene06());
	m_scenes.push_back(new Scene07());
	m_scenes.push_back(new Scene08());
	m_scenes.push_back(new Scene09());

	m_isSceneChanged = true;
	m_activeSceneIndex = 0;

	for (uint32_t i = 0; i < m_scenes.size(); i++)
		m_scenes[i]->Initialize();
}

void ScenesManager::Update(float time)
{
	m_isSceneChanged = false;

	if (m_activeSceneIndex != m_scenes.size() - 1)
	{
		if (time > m_scenes[m_activeSceneIndex + 1]->GetStartTime())
		{
			m_isSceneChanged = true;
			m_activeSceneIndex++;
		}
	}
}

BaseScene* ScenesManager::GetActiveScene() const
{
	return m_scenes[m_activeSceneIndex];
}

bool ScenesManager::IsSceneChanged() const
{
	return m_isSceneChanged;
}
