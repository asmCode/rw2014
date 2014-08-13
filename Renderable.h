#pragma once

class BaseMesh;
class Material;

class Renderable
{
public:
	Renderable(BaseMesh *mesh, Material* material);
	virtual ~Renderable();

	virtual void Draw();

	void SetMesh(BaseMesh* mesh);
	BaseMesh* GetMesh() const;

	void SetMaterial(Material* material);
	Material* GetMaterial() const;

private:
	BaseMesh* m_mesh;
	Material* m_material;
};

