#include "GameObject.h"
#include "Renderable.h"
#include <stddef.h>

GameObject::GameObject() :
	WorldMatrix(sm::Matrix::IdentityMatrix()),
	m_renderable(NULL),
	m_isActive(true)
{
}

GameObject::~GameObject()
{
}

void GameObject::SetRenderable(Renderable* renderable)
{
	m_renderable = renderable;
}

Renderable* GameObject::GetRenderable() const
{
	return m_renderable;
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

