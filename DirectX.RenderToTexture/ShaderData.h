#pragma once

#include <DirectXMath.h>

__declspec(align(16)) struct Material
{
	DirectX::XMFLOAT4 mDiffuse;
	DirectX::XMFLOAT4 mAmbient;
	DirectX::XMFLOAT4 mSpecular;
};

_declspec(align(16)) struct ConstantBuffer
{
    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProjection;
    DirectX::XMMATRIX mWorldInverse;

	DirectX::XMMATRIX mTextureTransform = DirectX::XMMatrixTranslation(1.0f, 1.0f, 1.0f);
	Material mMaterial;
};

_declspec(align(16)) struct DirectionalLight
{
	DirectX::XMFLOAT4 mDiffuse;
	DirectX::XMFLOAT4 mAmbient;
	DirectX::XMFLOAT4 mSpecular;
	DirectX::XMFLOAT4 mDirection;

	DirectX::XMFLOAT3 mCameraPos;
	float padding;
};

_declspec(align(16)) struct PointLight
{
	DirectX::XMFLOAT4 mDiffuse;
	DirectX::XMFLOAT4 mAmbient;
	DirectX::XMFLOAT4 mSpecular;
	DirectX::XMFLOAT4 mLightPos;

	DirectX::XMFLOAT3 mCameraPos;
	float padding;
};

_declspec(align(16)) struct LightBuffer
{
	DirectionalLight mDirectionalLight;
	PointLight mPointLight;
};