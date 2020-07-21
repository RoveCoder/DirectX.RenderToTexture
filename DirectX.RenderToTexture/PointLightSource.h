#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderData.h"

class PointLightSource
{
public:
	PointLightSource(Renderer* renderer, Camera* camera);

	bool Load();
	void Render();

	constexpr PointLight GetPointLight() { return m_PointLight; }

private:
	Renderer* m_Renderer = nullptr;
	Camera* m_Camera = nullptr;

	MeshData m_MeshData;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;

	ID3D11ShaderResourceView* m_DiffuseTexture = nullptr;

	PointLight m_PointLight;
};