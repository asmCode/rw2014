#include "Renderable.h"
#include "BaseMesh.h"
#include <Graphics/Material.h>

Renderable::Renderable(BaseMesh *mesh, Material* material) :
	m_mesh(mesh),
	m_material(material)
{
}

Renderable::~Renderable()
{
}

void Renderable::Draw()
{
	assert(m_mesh != NULL);

	m_mesh->Draw();
}

void Renderable::SetMesh(BaseMesh* mesh)
{
	m_mesh = mesh;
}

BaseMesh* Renderable::GetMesh() const
{
	return m_mesh;
}

void Renderable::SetMaterial(Material* material)
{
	m_material = material;
}

Material* Renderable::GetMaterial() const
{
	return m_material;
}
