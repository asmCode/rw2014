#pragma once

#include <vector>
#include <stdint.h>

class BaseScene;

class ScenesManager
{
public:
	ScenesManager();
	virtual ~ScenesManager() {}

	void Initialize();

	void Update(float time);
	BaseScene* GetActiveScene() const;
	bool IsSceneChanged() const;

private:
	std::vector<BaseScene*> m_scenes;
	int m_activeSceneIndex;

	bool m_isSceneChanged;
};
