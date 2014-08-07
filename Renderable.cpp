#include "Renderable.h"
#include "TriangledMesh.h"
#include <Graphics/Material.h>

Renderable::Renderable(TriangledMesh *mesh, Material* material) :
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

void Renderable::SetMesh(TriangledMesh* mesh)
{
	m_mesh = mesh;
}

TriangledMesh* Renderable::GetTriangledMesh() const
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
