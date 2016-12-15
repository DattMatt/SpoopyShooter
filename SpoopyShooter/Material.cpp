#include "Material.h"

Material::Material(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* _srv, ID3D11SamplerState* _sampState)
{
	vertexShader = vs;
	pixelShader = ps;
	srv = _srv;
	nmsrv = nullptr;
	sampState = _sampState;
	norm = false;
}

Material::Material(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* _srv, ID3D11ShaderResourceView* _nmsrv, ID3D11SamplerState* _sampState) {
	vertexShader = vs;
	pixelShader = ps;
	srv = _srv;
	nmsrv = _nmsrv;
	sampState = _sampState;
	norm = true;
}


Material::~Material()
{
}

SimpleVertexShader* Material::GetVertexShader() { return vertexShader; }
SimplePixelShader* Material::GetPixelShader() { return pixelShader; }
ID3D11ShaderResourceView* Material::GetSRV() { return srv; }
ID3D11ShaderResourceView* Material::GetNMSRV() { return nmsrv; }
bool Material::GetIfNorm() { return norm; }
ID3D11SamplerState* Material::GetSamplerState() { return sampState; }