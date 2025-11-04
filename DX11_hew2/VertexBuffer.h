#pragma once
#include	<vector>
#include	<wrl/client.h>
#include	"renderer.h"

using Microsoft::WRL::ComPtr;

//-----------------------------------------------------------------------------
//VertexBufferクラス
//-----------------------------------------------------------------------------
template <typename T> class VertexBuffer{

	ComPtr<ID3D11Buffer> m_VertexBuffer;

public:
	void Create(const std::vector<T>& vertices)
	{
		// デバイス取得
		ID3D11Device* device = nullptr;
		device = Renderer::GetDevice();
		assert(device); //deviceは存在することを確認

		// 頂点バッファ作成
		bool sts = Renderer::CreateVertexBufferWrite(
			sizeof(T),						// 1頂点当たりバイト数
			(unsigned int)vertices.size(),	// 頂点数
			(void*)vertices.data(),			// 頂点データ格納メモリ先頭アドレス
			&m_VertexBuffer);				// 頂点バッファ

		assert(sts == true); //結果を確認
	}

	// GPUにセット
	void SetGPU()
	{
		// デバイスコンテキスト取得
		ID3D11DeviceContext* devicecontext = nullptr;
		devicecontext = Renderer::GetDeviceContext();

		// 頂点バッファをセットする
		unsigned int stride = sizeof(T);
		unsigned  offset = 0;
		devicecontext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	}

	// 頂点バッファを書き換える
	void Modify(const std::vector<T>& vertices)
	{
		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		HRESULT hr = Renderer::GetDeviceContext()->Map(
			m_VertexBuffer.Get(), 
			0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (SUCCEEDED(hr)) {
			memcpy(msr.pData, vertices.data(), vertices.size() * sizeof(T));
			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer.Get(), 0);
		}
	}
};
