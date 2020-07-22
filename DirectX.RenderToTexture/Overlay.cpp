#include "Overlay.h"
#include "GeometryGenerator.h"
#include "DDSTextureLoader.h"
#include <DirectXMath.h>
#include <DirectXColors.h>

_declspec(align(16)) struct WorldConstantBuffer
{
    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProjection;
};

Overlay::Overlay(Renderer* renderer, Camera* camera) : m_Renderer(renderer), m_Camera(camera)
{
}

void Overlay::SetRenderTargetView()
{
    m_Renderer->GetDeviceContext()->ClearRenderTargetView(m_TextureRenderTargetView, reinterpret_cast<const float*>(&DirectX::Colors::Navy));
    m_Renderer->GetDeviceContext()->ClearDepthStencilView(m_TextureDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    m_Renderer->GetDeviceContext()->OMSetRenderTargets(1, &m_TextureRenderTargetView, m_TextureDepthStencilView);
    m_Renderer->GetDeviceContext()->RSSetViewports(1, &m_Viewport);
}

bool Overlay::Load(int window_width, int window_height)
{
    // Render targets
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width = window_width;
    texDesc.Height = window_height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.SampleDesc.Count = 4;
    texDesc.SampleDesc.Quality = m_Renderer->GetMsaaQuality() - 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    ID3D11Texture2D* texture = nullptr;
    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateTexture2D(&texDesc, 0, &texture));

    // Create the render target view.
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format = texDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateRenderTargetView(texture, &renderTargetViewDesc, &m_TextureRenderTargetView));

    // Depth stencil view
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = window_width;
    descDepth.Height = window_height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 4;
    descDepth.SampleDesc.Quality = m_Renderer->GetMsaaQuality() - 1;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateTexture2D(&descDepth, nullptr, &m_TextureDepthStencil));
    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateDepthStencilView(m_TextureDepthStencil, nullptr, &m_TextureDepthStencilView));

    // Create the shader resource view.
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    shaderResourceViewDesc.Format = texDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateShaderResourceView(texture, &shaderResourceViewDesc, &m_DiffuseTexture));

    // Create viewport
    m_Viewport.Width = (float)window_width;
    m_Viewport.Height = (float)window_height;
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;

    // Drawing code
    Geometry::CreateFullscreenQuad(&m_MeshData);

    // Create vertex buffer
    D3D11_BUFFER_DESC vbd = {};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = (UINT)(sizeof(Vertex) * m_MeshData.vertices.size());
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA vInitData = {};
    vInitData.pSysMem = &m_MeshData.vertices[0];

    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateBuffer(&vbd, &vInitData, &m_VertexBuffer));

    // Create index buffer
    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = (UINT)(sizeof(UINT) * m_MeshData.indices.size());
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA iInitData = {};
    iInitData.pSysMem = &m_MeshData.indices[0];

    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateBuffer(&ibd, &iInitData, &m_IndexBuffer));

    // Constant buffer
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WorldConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    DX::ThrowIfFailed(m_Renderer->GetDevice()->CreateBuffer(&bd, nullptr, &m_WorldConstantBuffer));

    return true;
}

void Overlay::Render()
{
    // Bind the vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_Renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    // Bind the index buffer
    m_Renderer->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set topology
    m_Renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set world buffer
    DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
    world *= DirectX::XMMatrixScaling(0.25f, 0.25f, 1.0f);
    world *= DirectX::XMMatrixTranslation(0.70f, 0.70f, 0.0f);

    WorldConstantBuffer cb;
    cb.mWorld = DirectX::XMMatrixTranspose(world);
    cb.mView = DirectX::XMMatrixTranspose(m_Camera->GetView());
    cb.mProjection = DirectX::XMMatrixTranspose(m_Camera->GetProjection());

    m_Renderer->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_WorldConstantBuffer);
    m_Renderer->GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_WorldConstantBuffer);
    m_Renderer->GetDeviceContext()->UpdateSubresource(m_WorldConstantBuffer, 0, nullptr, &cb, 0, 0);

    m_Renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &m_DiffuseTexture);

    // Render geometry
    m_Renderer->GetDeviceContext()->DrawIndexed((UINT)m_MeshData.indices.size(), 0, 0);
}  
