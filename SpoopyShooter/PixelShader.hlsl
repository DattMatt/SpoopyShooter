// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
Texture2D diffuseTexture  : register(t0);
SamplerState basicSampler : register(s0);

struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal       : NORMAL;	
	float3 worldPos     : POSITION;
	float2 uv			: TEXCOORD;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight
{
	float4 DiffuseColor;
	float3 Position;
};

struct SpotLight
{
	float4 DiffuseColor;
	float3 Direction;
	float2 SpotPower;
};

cbuffer data : register(b0)
{
	DirectionalLight dirLight;
	DirectionalLight dirLight2;

	PointLight pLight;

	float3 CameraPosition;
};

float3 CalculateDirLight(float3 norm, DirectionalLight light)
{
	float3 dirToLight = -normalize(light.Direction);
	float lightAmount = saturate(dot(norm, dirToLight));

	return ((lightAmount * light.DiffuseColor) + light.AmbientColor);
}

float3 CalculatePointLight(float3 norm, float3 wPos, PointLight light) {
	float3 lightDir = normalize(light.Position - wPos);
	float lightAmount = saturate(dot(norm, lightDir));

	float3 toCamera = normalize(CameraPosition - wPos);
	float3 refl = reflect(-lightDir, norm);
	float spec = pow(max(dot(refl, toCamera), 0), 200);
	
	return (lightAmount * light.DiffuseColor) + spec;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{	
	input.normal = normalize(input.normal);	
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	float4 directionalLight = float4(CalculateDirLight(input.normal, dirLight), 1);
	float4 directionalLight2 = float4(CalculateDirLight(input.normal, dirLight2), 1);

	float4 pointLight = float4(CalculatePointLight(input.normal, input.worldPos, pLight), 1);
	
	return (directionalLight + directionalLight2 + pointLight) * surfaceColor;
}