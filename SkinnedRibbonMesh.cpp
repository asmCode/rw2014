#pragma once

#include "SkinnedRibbonMesh.h"
#include "TriangleDataColorGlow.h"
#include "GameObjects/Ribbon.h"
#include "TrianglesRibbon.h"
#include <Graphics/SkinnedMeshData.h>
#include <Graphics/SkinnedVertex.h>
#include <Graphics/Animation.h>
#include "MeshUtils.h"
#include "DemoUtils.h"
#include "VertexDataBufferTransform.h"
#include <Math/Vec2.h>
#include <GL/glew.h>

SkinnedRibbonMesh::SkinnedRibbonMesh() :
	m_vertexBufferId(0),
	m_vertexDataBufferId(0),
	m_indexBufferId(0),
	m_trianglesCount(0),
	m_triangles(NULL),
	m_meshData(NULL),
	BoneTransforms(NULL),
	m_transformBuffer(NULL),
	m_worldToOrigin(NULL)
{
}

SkinnedRibbonMesh::~SkinnedRibbonMesh()
{
	if (m_triangles != NULL)
		delete m_triangles;

	if (m_vertexBufferId != 0)
		glDeleteBuffers(1, &m_vertexBufferId);
	if (m_vertexDataBufferId != 0)
		glDeleteBuffers(1, &m_vertexDataBufferId);
	if (m_indexBufferId != 0)
		glDeleteBuffers(1, &m_indexBufferId);
}

void SkinnedRibbonMesh::Initialize(SkinnedMeshData* meshData)
{
	m_meshData = meshData;

	int maxInVertices = 0;
	int minInVertices = 99999999;

	int bones[50];
	memset(bones, 0, sizeof(int)* 50);

	for (int i = 0; i < meshData->verticesCount; i++)
	{
		bones[meshData->vertices[i].boneIndex[0]]++;

		if (meshData->vertices[i].boneIndex[0] > maxInVertices)
			maxInVertices = meshData->vertices[i].boneIndex[0];

		if (meshData->vertices[i].boneIndex[0] < minInVertices)
			minInVertices = meshData->vertices[i].boneIndex[0];
	}

	int maxInBones = 0;
	int minInBones = 9999;
	for (int i = 0; i < meshData->bonesCount; i++)
	{
		if (meshData->bonesIds[i] > maxInBones)
			maxInBones = meshData->bonesIds[i];

		if (meshData->bonesIds[i] < minInBones)
			minInBones = meshData->bonesIds[i];
	}

	BoneTransforms = new sm::Matrix[meshData->bonesCount];

	m_trianglesCount = meshData->verticesCount / 3;

	m_transformBuffer = new VertexDataBufferTransform();
	m_transformBuffer->Initialize(m_trianglesCount);

	m_worldToOrigin = new sm::Matrix[m_trianglesCount];

	//meshData->vertices[0].position

	for (int i = 0; i < m_trianglesCount; i++)
	{
		sm::Vec3 v[3];
		v[0] = meshData->vertices[i * 3 + 0].position;
		v[1] = meshData->vertices[i * 3 + 1].position;
		v[2] = meshData->vertices[i * 3 + 2].position;

		sm::Matrix transform = DemoUtils::GetTriangleTransform(v);
		//m_transformBuffer->SetTransform(i, sm::Matrix::ScaleMatrix(0.01f, 0.01f, 0.01f) * transform.GetInversed());
		m_worldToOrigin[i] = sm::Matrix::ScaleMatrix(0.01f, 0.01f, 0.01f) * transform.GetInversed();
	}

	CreateVertexPositionBuffer(meshData);
	CreateIndexBuffer();
	CreateVertexDataBuffer();
}

void SkinnedRibbonMesh::CreateVertexPositionBuffer(SkinnedMeshData* meshData)
{
	glGenBuffers(1, &m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkinnedVertex)* m_trianglesCount * 3, meshData->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkinnedRibbonMesh::CreateIndexBuffer()
{
	uint16_t* indexBuffer = new uint16_t[m_trianglesCount * 3];

	for (int i = 0; i < m_trianglesCount * 3; i++)
		indexBuffer[i] = i;

	glGenBuffers(1, &m_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t)* 3 * m_trianglesCount, indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexBuffer;
}

void SkinnedRibbonMesh::CreateVertexDataBuffer()
{
	// TODO: bedzie trzeba przeniesc inicjalizacji klas pochodnych

	m_triangles = new TriangleDataColorGlow[m_trianglesCount * 3];

	for (uint32_t i = 0; i < m_trianglesCount * 3; i++)
	{
		m_triangles[i].Color.Set(0.2f, 0.8f, 0.0f, 0.8f);
		m_triangles[i].GlowPower = 1.2f;
	}

	glGenBuffers(1, &m_vertexDataBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColorGlow)* m_trianglesCount * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkinnedRibbonMesh::Apply()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColorGlow)* m_trianglesCount * 3, m_triangles, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(TriangleDataColorGlow), 0);
	glVertexAttribPointer(4, 1, GL_FLOAT, false, sizeof(TriangleDataColorGlow), reinterpret_cast<void*>(sizeof(sm::Vec4)));

	m_transformBuffer->Apply(5);
}

void SkinnedRibbonMesh::Draw()
{
	Apply();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(SkinnedVertex), 0);
	glVertexAttribIPointer(1, 4, GL_UNSIGNED_BYTE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, boneIndex));
	glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, weight));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glDrawElements(GL_TRIANGLES, 3 * m_trianglesCount, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SkinnedRibbonMesh::Update(float time, float deltaTime)
{
	if (m_ribbon == NULL)
		return;

	TrianglesRibbon* trianglesRibbon = m_ribbon->GetSource();

	for (int i = 0; i < m_trianglesCount; i++)
	{
		TrianglesRibbon::TriangleData* triangleData = trianglesRibbon->GetTriangleData(i);

		sm::Vec3 position = triangleData->Curve->Evaluate(time);
		float scale = triangleData->ScaleCurve->Evaluate(time);

		//float normTimeOnCurve = (time - m_trianglesData[i]->Curve->GetStartTime()) / (m_trianglesData[i]->Curve->GetEndTime() - m_trianglesData[i]->Curve->GetStartTime());
		//m_trianglesData[i]->Angle = (cos(normTimeOnCurve * 3.1415f * 2) - 1) * -0.5f;
		//m_trianglesData[i]->Angle *= m_trianglesData[i]->AngleSpeed;

		//m_trianglesData[i]->Angle += m_trianglesData[i]->AngleSpeed * deltaTime;

		sm::Matrix ribbonTransform =
			sm::Matrix::TranslateMatrix(position) *
			//m_triangledMesh->GetBaseRotation(i) *
			//sm::Matrix::RotateAxisMatrix(m_trianglesData[i]->Angle * (1.0f - scale), m_trianglesData[i]->Axis) *
			sm::Matrix::ScaleMatrix(scale, scale, scale);

		m_transformBuffer->SetTransform(
			i,
			ribbonTransform *
			m_worldToOrigin[i]);
	}
}

//TriangleDataColor* SkinnedRibbonMesh::GetTrianglesData() const
//{
//	return m_triangles;
//}

uint32_t SkinnedRibbonMesh::GetTrianglesCount() const
{
	return m_trianglesCount;
}

void SkinnedRibbonMesh::SetColor(const sm::Vec4& color)
{
	for (uint32_t i = 0; i < m_trianglesCount; i++)
		SetTriangleColor(i, color);
}

void SkinnedRibbonMesh::SetTriangleColor(int index, const sm::Vec4& color)
{
	TriangleDataColorGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->Color = color;
	(trianglesPointer + 1)->Color = color;
	(trianglesPointer + 2)->Color = color;
}

void SkinnedRibbonMesh::SetGlowPower(int index, float glowPower)
{
	TriangleDataColorGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->GlowPower = glowPower;
	(trianglesPointer + 1)->GlowPower = glowPower;
	(trianglesPointer + 2)->GlowPower = glowPower;
}

void SkinnedRibbonMesh::SetGlowPower(float glowPower)
{
	for (uint32_t i = 0; i < m_trianglesCount; i++)
		SetGlowPower(i, glowPower);
}

void SkinnedRibbonMesh::AddAnimation(const std::string& name, Animation* animation)
{
	for (int i = 0; i < m_meshData->bonesCount; i++)
	{
		Animation* anim = animation->GetAnimationById(m_meshData->bonesIds[i]);
		assert(anim != NULL);

		anim->AttachTransformTarget(&BoneTransforms[i], NULL);

		m_animations[name] = anim;
	}
}

int SkinnedRibbonMesh::GetBonesCount() const
{
	return m_meshData->bonesCount;
}

void SkinnedRibbonMesh::SetRibbon(Ribbon* ribbon)
{
	m_ribbon = ribbon;
}
