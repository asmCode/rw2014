#pragma once

#include <stdint.h>

class Material;
class TriangleShader;

class TriangledMesh
{
public:
	TriangledMesh();
	virtual ~TriangledMesh();

	void Initialize(float* vertices, float* normals, float* texcoords, int count);

	void SetMaterial(Material* material);
	Material* GetMaterial() const;

	void SetTriangleShader(TriangleShader* triangleShader);

	void Apply();
	void Update(float time, float deltaTime);
	void Draw();

private:
	uint32_t m_vertexBufferId;
	uint32_t m_normalBufferId;
	uint32_t m_texcoordBufferId;
	uint32_t m_indexBufferId;

	int m_vertexCount;
	float* m_vertexBuffer;
	float* m_normalBuffer;
	float* m_texcoordBuffer;
	uint16_t* m_indexBuffer;

	Material* m_material;
	TriangleShader* m_triangleShader;
};
