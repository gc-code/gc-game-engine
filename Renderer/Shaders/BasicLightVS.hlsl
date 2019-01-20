//
// Basic Gouraud lighting shader
//

cbuffer cbPerLevel : register(b0)
{
	matrix View;
	matrix Projection;
};

cbuffer cbPerModel : register(b1)
{
	matrix World;
};

cbuffer cbLightingAttributes : register(b2)
{
	float4 Diffuse;
	float4 Ambient;
	float4 Specular;
	float4 LightDirection;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float4 Normal : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float4 Normal : NORMAL;
};

PS_INPUT main(VS_INPUT input)
{
	float4 diffuseTerm;
	float4 outputNormal;

	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;

	diffuseTerm = Diffuse * Ambient;
	outputNormal = max(dot(input.Normal, LightDirection), 0.0f);


	return output;
}