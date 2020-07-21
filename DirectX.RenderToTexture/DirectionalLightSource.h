#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderData.h"

class DirectionalLightSource
{
public:
	DirectionalLightSource(Renderer* renderer, Camera* camera);

	bool Load();
	void Render();

	constexpr DirectionalLight GetDirectionalLight() { return m_DirectionalLight; }

private:
	Renderer* m_Renderer = nullptr;
	Camera* m_Camera = nullptr;

	MeshData m_MeshData;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;

	ID3D11ShaderResourceView* m_DiffuseTexture = nullptr;

	DirectionalLight m_DirectionalLight;
};