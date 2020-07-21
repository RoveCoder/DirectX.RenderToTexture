cbuffer WorldBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

SamplerState SamplerAnisotropic : register(s0);
Texture2D TextureDiffuse : register(t0);

struct VertexInput
{
	float3 Position : POSITION;
	float2 Texture : TEXCOORD0;
};

struct PixelInput
{
	float3 Position : POSITION;
	float4 PositionH : SV_POSITION;
	float2 Texture : TEXCOORD0;
};