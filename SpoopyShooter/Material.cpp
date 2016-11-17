#include "Material.h"

Material::Material(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* _srv, ID3D11SamplerState* _sampState)
{
	vertexShader = vs;
	pixelShader = ps;
	srv = _srv;
	sampState = _sampState;
}

Material::Material(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* _srv, ID3D11ShaderResourceView* _nmsrv, ID3D11SamplerState* _sampState) {
	vertexShader = vs;
	pixelShader = ps;
	srv = _srv;
	nmsrv = _nmsrv;
	sampState = _sampState;
}

Material::~Material()
{
}

SimpleVertexShader* Material::GetVertexShader() { return vertexShader; }
SimplePixelShader* Material::GetPixelShader() { return pixelShader; }
ID3D11ShaderResourceView* Material::GetSRV() { return srv; }
ID3D11ShaderResourceView* Material::GetNMSRV() { return nmsrv; }
ID3D11SamplerState* Material::GetSamplerState() { return sampState; }