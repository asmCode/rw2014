#include "GameObject.h"
#include "Renderable.h"
#include <stddef.h>

GameObject::GameObject() :
	WorldMatrix(sm::Matrix::Identity),
	m_isActive(true)
{
}

GameObject::~GameObject()
{
}

const std::vector<Renderable*>& GameObject::GetRenderables() const
{
	return m_renderables;
}

void GameObject::Awake()
{
}

void GameObject::Update(float time, float seconds)
{
}

bool GameObject::IsActive() const
{
	return m_isActive;
}

const std::string& GameObject::GetName() const
{
	return m_name;
}
