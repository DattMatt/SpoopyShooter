#pragma once
#include "SimpleShader.h"

class Material
{
public:
	Material(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* _srv, ID3D11SamplerState* _sampState);
	Material(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* _srv, ID3D11ShaderResourceView* _nmsrv, ID3D11SamplerState* _sampState);
	~Material();

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView* GetNMSRV();
	ID3D11SamplerState* GetSamplerState();

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* nmsrv;
	ID3D11SamplerState* sampState;
};

