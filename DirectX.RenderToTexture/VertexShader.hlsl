#include "Header.hlsli"

PixelInput main(VertexInput input)
{
	PixelInput output;

	// Transform to homogeneous clip space.
	output.PositionH = mul(float4(input.Position, 1.0f), World);
	output.PositionH = mul(output.PositionH, View);
	output.PositionH = mul(output.PositionH, Projection);

	// Transform to world space.
	output.Position = mul(float4(input.Position, 1.0f), World).xyz;

	// Transform UVs
	output.Texture = mul(float4(input.Texture, 1.0f, 1.0f), TextureTransform).xy;

	// Transform normals by inverse world
	output.Normal = mul(input.Normal, (float3x3)InverseWorld).xyz;

	return output;
}