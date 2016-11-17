cbuffer externalData : register(b0)
{
	float4x4 worldViewProj;
};

TextureCube cubeMap : register(t0);

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float3 position		: POSITION;
};

struct VertexOut
{
	float4 position		: SV_POSITION;
	float3 worldPos		: POSITION;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}