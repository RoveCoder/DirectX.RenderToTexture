#include "OverlayHeader.hlsli"

PixelInput main(VertexInput input)
{
	PixelInput output;

	output.PositionH = mul(float4(input.Position, 1.0f), World);
	/*output.PositionH = mul(output.PositionH, View);
	output.PositionH = mul(output.PositionH, Projection);*/

	output.Position = mul(float4(input.Position, 1.0f), World).xyz;

	output.Texture = input.Texture;

	return output;
}