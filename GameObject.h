#pragma once

#include <Math/Matrix.h>

class Renderable;

class GameObject
{
public:
	sm::Matrix WorldMatrix;

	GameObject();
	virtual ~GameObject();

	void SetRenderable(Renderable* renderable);
	Renderable* GetRenderable() const;

	virtual void Awake();
	virtual void Update(float time, float seconds);

	bool IsActive() const;

protected:
	Renderable* m_renderable;
	bool m_isActive;
};

