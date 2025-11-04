#include	"Shader.h"
#include	"renderer.h"

//=======================================
//Shader作成
//=======================================
void Shader::Create(std::string vs, std::string ps)
{
	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int numElements = ARRAYSIZE(layout);

	ID3D11Device* device = Renderer::GetDevice();

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	HRESULT hr = Renderer::CreateVertexShader(
		&m_pVertexShader,
		&m_pVertexLayout,
		layout,
		numElements,
		vs.c_str()
		);
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return;
	}

	// ピクセルシェーダーを生成

	hr = Renderer::CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
		&m_pPixelShader,
		ps.c_str()
		);
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return;
	}

	return;
}

//=======================================
//GPUにデータを送る
//=======================================
void Shader::SetGPU()
{
	ID3D11DeviceContext* devicecontext = Renderer::GetDeviceContext();

	devicecontext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);		// 頂点シェーダーをセット
	devicecontext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);		// ピクセルシェーダーをセット
	devicecontext->IASetInputLayout(m_pVertexLayout.Get());				// 頂点レイアウトセット
}

