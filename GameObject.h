#pragma once

#include <Math/Matrix.h>
#include <vector>

class Renderable;

class GameObject
{
public:
	sm::Matrix WorldMatrix;

	GameObject();
	virtual ~GameObject();

	const std::vector<Renderable*>& GetRenderables() const;

	virtual void Awake();
	virtual void Update(float time, float seconds);

	bool IsActive() const;

	const std::string& GetName() const;

protected:
	std::vector<Renderable*> m_renderables;

	bool m_isActive;
	std::string m_name;
};

