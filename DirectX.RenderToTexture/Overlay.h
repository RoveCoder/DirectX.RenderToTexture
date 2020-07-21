#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"

class Overlay
{
public:
	Overlay(Renderer* renderer, Camera* camera);

	void SetRenderTargetView();

	bool Load(int window_width, int window_height);
	void Render();

private:
	Renderer* m_Renderer = nullptr;
	Camera* m_Camera = nullptr;

	// Drawing variables
	MeshData m_MeshData;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	ID3D11Buffer* m_WorldConstantBuffer = nullptr;

	ID3D11ShaderResourceView* m_DiffuseTexture = nullptr;

	// Render-to-texture variables
	ID3D11Texture2D* m_TextureDepthStencil = nullptr;
	ID3D11RenderTargetView* m_TextureRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_TextureDepthStencilView = nullptr;

	D3D11_VIEWPORT m_Viewport;
};