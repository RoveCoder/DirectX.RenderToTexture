#include "GeometryGenerator.h"
#include <DirectXMath.h>

namespace
{
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, MeshData* meshData)
	{
		unsigned int baseIndex = (unsigned int)meshData->vertices.size();

		float y = 0.5f * height;
		float dTheta = 2.0f * DirectX::XM_PI / sliceCount;

		// Duplicate cap ring vertices because the texture coordinates and normals differ.
		for (unsigned int i = 0; i <= sliceCount; ++i)
		{
			float x = topRadius * cosf(i * dTheta);
			float z = topRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData->vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, u, v));
		}

		// Cap center vertex.
		meshData->vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

		// Index of center vertex.
		unsigned int centerIndex = (unsigned int)meshData->vertices.size() - 1;

		for (unsigned int i = 0; i < sliceCount; ++i)
		{
			meshData->indices.push_back(centerIndex);
			meshData->indices.push_back(baseIndex + i + 1);
			meshData->indices.push_back(baseIndex + i);
		}
	}

	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, MeshData* meshData)
	{
		// 
		// Build bottom cap.
		//

		unsigned int baseIndex = (unsigned int)meshData->vertices.size();
		float y = -0.5f * height;

		// vertices of ring
		float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
		for (unsigned int i = 0; i <= sliceCount; ++i)
		{
			float x = bottomRadius * cosf(i * dTheta);
			float z = bottomRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData->vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, u, v));
		}

		// Cap center vertex.
		meshData->vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f));

		// Cache the index of center vertex.
		unsigned int centerIndex = (unsigned int)meshData->vertices.size() - 1;

		for (unsigned int i = 0; i < sliceCount; ++i)
		{
			meshData->indices.push_back(centerIndex);
			meshData->indices.push_back(baseIndex + i);
			meshData->indices.push_back(baseIndex + i + 1);
		}
	}
}

void Geometry::CreateBox(float width, float height, float depth, MeshData* mesh)
{
	Vertex vertices[] =
	{
		{ -width, -height, -depth, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },
		{ -width, +height, -depth, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
		{ +width, +height, -depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },
		{ +width, -height, -depth, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f },

		{ -width, -height, +depth, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
		{ +width, -height, +depth, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
		{ +width, +height, +depth, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
		{ -width, +height, +depth, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },

		{ -width, +height, -depth, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -width, +height, +depth, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ +width, +height, +depth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ +width, +height, -depth, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },

		{ -width, -height, -depth, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f },
		{ +width, -height, -depth, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f },
		{ +width, -height, +depth, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },
		{ -width, -height, +depth, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },

		{ -width, -height, +depth, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		{ -width, +height, +depth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ -width, +height, -depth, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ -width, -height, -depth, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f },

		{ +width, -height, -depth, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		{ +width, +height, -depth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ +width, +height, +depth, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ +width, -height, +depth, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f }
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23,
	};

	mesh->vertices.assign(&vertices[0], &vertices[24]);
	mesh->indices.assign(&indices[0], &indices[36]);
}

void Geometry::CreateGrid(float width, float depth, unsigned int m, unsigned int n, MeshData* mesh)
{
	unsigned int vertexCount = m * n;
	unsigned int faceCount = (m - 1) * (n - 1) * 2;

	// Create the vertices.
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	mesh->vertices.resize(vertexCount);
	for (unsigned int i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;
		for (unsigned int j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;

			mesh->vertices[i * n + j].x = x;
			mesh->vertices[i * n + j].y = 0.0f;
			mesh->vertices[i * n + j].z = z;

			mesh->vertices[i * n + j].nx = 0.0f;
			mesh->vertices[i * n + j].ny = 1.0f;
			mesh->vertices[i * n + j].nz = 0.0f;

			mesh->vertices[i * n + j].u = j * du;
			mesh->vertices[i * n + j].v = i * dv;
		}
	}

	// Create the indices.
	mesh->indices.resize(faceCount * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	unsigned int k = 0;
	for (unsigned int i = 0; i < m - 1; ++i)
	{
		for (unsigned int j = 0; j < n - 1; ++j)
		{
			mesh->indices[k] = i * n + j;
			mesh->indices[k + 1] = i * n + j + 1;
			mesh->indices[k + 2] = (i + 1) * n + j;

			mesh->indices[k + 3] = (i + 1) * n + j;
			mesh->indices[k + 4] = i * n + j + 1;
			mesh->indices[k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}
}

void Geometry::CreateCylinder(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, MeshData* meshData)
{
	meshData->vertices.clear();
	meshData->indices.clear();

	//
	// Build Stacks.
	// 

	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	unsigned int ringCount = stackCount + 1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for (unsigned int i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;

		// vertices of ring
		float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
		for (unsigned int j = 0; j <= sliceCount; ++j)
		{
			Vertex vertex;

			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(r * c, y, r * s);

			vertex.x = position.x;
			vertex.y = position.y;
			vertex.z = position.z;

			vertex.u = (float)j / sliceCount;
			vertex.v = 1.0f - (float)i / stackCount;

			DirectX::XMFLOAT3 tangentU = DirectX::XMFLOAT3(-s, 0.0f, c);

			float dr = bottomRadius - topRadius;
			DirectX::XMFLOAT3 bitangent(dr * c, -height, dr * s);

			DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&tangentU);
			DirectX::XMVECTOR B = XMLoadFloat3(&bitangent);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(T, B));

			DirectX::XMFLOAT3 normal;
			XMStoreFloat3(&normal, N);

			vertex.nx = normal.x;
			vertex.ny = normal.y;
			vertex.nz = normal.z;

			meshData->vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	unsigned int ringVertexCount = sliceCount + 1;

	// Compute indices for each stack.
	for (unsigned int i = 0; i < stackCount; ++i)
	{
		for (unsigned int j = 0; j < sliceCount; ++j)
		{
			meshData->indices.push_back(i * ringVertexCount + j);
			meshData->indices.push_back((i + 1) * ringVertexCount + j);
			meshData->indices.push_back((i + 1) * ringVertexCount + j + 1);

			meshData->indices.push_back(i * ringVertexCount + j);
			meshData->indices.push_back((i + 1) * ringVertexCount + j + 1);
			meshData->indices.push_back(i * ringVertexCount + j + 1);
		}
	}

	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
}

void Geometry::CreateFullscreenQuad(MeshData* meshData)
{
	meshData->vertices.resize(4);
	meshData->indices.resize(6);

	// Position coordinates specified in NDC space.
	meshData->vertices[0] = Vertex(
		-1.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f);

	meshData->vertices[1] = Vertex(
		-1.0f, +1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f);

	meshData->vertices[2] = Vertex(
		+1.0f, +1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f);

	meshData->vertices[3] = Vertex(
		+1.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 1.0f);

	meshData->indices[0] = 0;
	meshData->indices[1] = 1;
	meshData->indices[2] = 2;

	meshData->indices[3] = 0;
	meshData->indices[4] = 2;
	meshData->indices[5] = 3;
}

void Geometry::CreateSphere(float radius, unsigned int sliceCount, unsigned int stackCount, MeshData* meshData)
{
	meshData->vertices.clear();
	meshData->indices.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

	meshData->vertices.push_back(topVertex);

	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (unsigned int i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (unsigned int j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			Vertex v;

			// spherical to cartesian
			v.x = radius * sinf(phi) * cosf(theta);
			v.y = radius * cosf(phi);
			v.z = radius * sinf(phi) * sinf(theta);

			// Partial derivative of P with respect to theta
			/*v.TangentU.x = -radius * sinf(phi) * sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = +radius * sinf(phi) * cosf(theta);*/

			/*DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&v.TangentU);
			DirectX::XMStoreFloat3(&v.TangentU, DirectX::XMVector3Normalize(T));*/

			// Normals
			DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(v.x, v.y, v.z);
			DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&pos);

			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(p));

			v.nx = normal.x;
			v.ny = normal.y;
			v.nz = normal.z;

			// Texture UV
			v.u = theta / DirectX::XM_2PI;
			v.v = phi / DirectX::XM_PI;

			meshData->vertices.push_back(v);
		}
	}

	meshData->vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (unsigned int i = 1; i <= sliceCount; ++i)
	{
		meshData->indices.push_back(0);
		meshData->indices.push_back(i + 1);
		meshData->indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	unsigned int baseIndex = 1;
	unsigned int ringVertexCount = sliceCount + 1;
	for (unsigned int i = 0; i < stackCount - 2; ++i)
	{
		for (unsigned int j = 0; j < sliceCount; ++j)
		{
			meshData->indices.push_back(baseIndex + i * ringVertexCount + j);
			meshData->indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData->indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			meshData->indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			meshData->indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData->indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	unsigned int southPoleIndex = (unsigned int)meshData->vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (unsigned int i = 0; i < sliceCount; ++i)
	{
		meshData->indices.push_back(southPoleIndex);
		meshData->indices.push_back(baseIndex + i);
		meshData->indices.push_back(baseIndex + i + 1);
	}
}
