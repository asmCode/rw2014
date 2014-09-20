#pragma once

#include "BaseMesh.h"
#include <Math/Matrix.h>
#include <map>
#include <string>
#include <stdint.h>

class TriangleDataColorGlow;
class SkinnedMeshData;
class SkinnedVertex;
class Animation;
class VertexDataBufferTransform;
class Ribbon;

class SkinnedRibbonMesh : public BaseMesh
{
public:
	SkinnedRibbonMesh();
	virtual ~SkinnedRibbonMesh();

	sm::Matrix* BoneTransforms;

	void Initialize(SkinnedMeshData* meshData);
	virtual void Draw();
	void Update(float time, float deltaTime);

	uint32_t GetTrianglesCount() const;

	void SetColor(const sm::Vec4& color);
	void SetTriangleColor(int index, const sm::Vec4& color);
	void SetGlowPower(int index, float glowPower);
	void SetGlowPower(float glowPower);

	void AddAnimation(const std::string& name, Animation* animation);

	int GetBonesCount() const;

	void SetRibbon(Ribbon* ribbon);

	Ribbon* m_ribbon;

protected:
	/*class RibbonData
	{

	};*/

	uint32_t m_vertexBufferId;
	uint32_t m_vertexDataBufferId;
	uint32_t m_indexBufferId;

	VertexDataBufferTransform* m_transformBuffer;

	int m_trianglesCount;
	TriangleDataColorGlow* m_triangles;

	sm::Matrix* m_worldToOrigin;

	SkinnedMeshData* m_meshData;


	std::map<std::string, Animation*> m_animations;

	virtual void Apply();
	void CreateVertexDataBuffer();
	void CreateVertexPositionBuffer(SkinnedMeshData* meshData);
	void CreateIndexBuffer();
};
