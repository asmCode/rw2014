#include "MeshUtils.h"
#include <Graphics/MeshPart.h>
#include <Graphics/VertexInformation.h>

void MeshUtils::ExtractVertexPositionBuffer(sm::Vec3*& outBuffer, MeshPart* meshPart)
{
	int verticesCount = meshPart->GetVerticesCount();
	const void* rawVertexBuffer = meshPart->GetVertices();

	outBuffer = new sm::Vec3[verticesCount];

	for (int i = 0; i < verticesCount; i++)
		outBuffer[i] = VertexInformation::GetPosition(rawVertexBuffer, i, meshPart->m_vertexType);
}
