struct Material
{
	float4 mDiffuse;
	float4 mAmbient;
	float4 mSpecular;
};

struct DirectionalLight
{
	float4 mDiffuse;
	float4 mAmbient;
	float4 mSpecular;
	float4 mDirection;
	float3 mCameraPos;
	float padding;
};

struct PointLight
{
	float4 mDiffuse;
	float4 mAmbient;
	float4 mSpecular;
	float4 mLightPos;
	float3 mCameraPos;
	float padding;
};

cbuffer WorldBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix InverseWorld;

	matrix TextureTransform;
	Material mMaterial;
}

cbuffer DirectionalLightBuffer : register(b1)
{
	DirectionalLight mDirectionalLight;
	PointLight mPointLight;
};

struct VertexInput
{
	float3 Position : POSITION;
	float2 Texture : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct PixelInput
{
	float3 Position : POSITION;
	float4 PositionH : SV_POSITION;
	float2 Texture : TEXCOORD0;
	float3 Normal : NORMAL;
};

SamplerState SamplerAnisotropic : register(s0);

Texture2D TextureDiffuse : register(t0);