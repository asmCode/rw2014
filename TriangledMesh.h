#pragma once

#include "BaseMesh.h"
#include <stdint.h>

class Material;
class TriangleDataColor;
class MeshPart;

class TriangledMesh : public BaseMesh
{
public:
	TriangledMesh();
	virtual ~TriangledMesh();

	void Initialize(int trianglesCount);

	void SetMaterial(Material* material);
	Material* GetMaterial() const;

	void Draw();

	TriangleDataColor* GetTrianglesData() const;
	uint32_t GetTrianglesCount() const;

private:
	uint32_t m_vertexBufferId; // to statyczne i jedyne na aplikacje
	uint32_t m_vertexDataBufferId;
	uint32_t m_indexBufferId; // to statyczne i jedyne na aplikacje

	float* m_vertexBuffer; // to statyczne i jedyne na aplikacje
	uint16_t* m_indexBuffer; // to statyczne i jedyne na aplikacje

	int m_trianglesCount;
	TriangleDataColor* m_triangles;

	Material* m_material;

	void Apply();
};
