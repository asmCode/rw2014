#pragma once

class TriangledMesh;
class Material;

class Renderable
{
public:
	Renderable(TriangledMesh *mesh, Material* material);
	virtual ~Renderable();

	virtual void Draw();

	void SetMesh(TriangledMesh* mesh);
	TriangledMesh* GetTriangledMesh() const;

	void SetMaterial(Material* material);
	Material* GetMaterial() const;

private:
	TriangledMesh* m_mesh;
	Material* m_material;
};

