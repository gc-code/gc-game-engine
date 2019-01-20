//
//	Default level editor shader
//

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbLevelEditor
{
	float4 SelectedHighlight;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_Target
{
	return txDiffuse.Sample(samLinear, input.Tex) * SelectedHighlight;
}