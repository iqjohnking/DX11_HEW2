#pragma once
#include	<wrl/client.h>
#include	<string>
#include	<d3d11.h>

using Microsoft::WRL::ComPtr;

//-----------------------------------------------------------------------------
//Shaderクラス
//-----------------------------------------------------------------------------
class Shader{
public:
	void Create(std::string vs, std::string ps);
	void SetGPU();
private:
	ComPtr<ID3D11VertexShader> m_pVertexShader;		// 頂点シェーダー
	ComPtr<ID3D11PixelShader>  m_pPixelShader;		// ピクセルシェーダー
	ComPtr<ID3D11InputLayout>  m_pVertexLayout;		// 頂点レイアウト
};

