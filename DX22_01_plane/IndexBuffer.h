#pragma once

#include	<vector>
#include	<wrl/client.h>
#include	"renderer.h"

using Microsoft::WRL::ComPtr;

//-----------------------------------------------------------------------------
//IndexBufferクラス
//-----------------------------------------------------------------------------
class IndexBuffer {

	ComPtr<ID3D11Buffer> m_IndexBuffer;

public:
	void Create(const std::vector<unsigned int>& indices)
	{
		// デバイス取得
		ID3D11Device* device = nullptr;
		device = Renderer::GetDevice();
		assert(device); //deviceが存在することを確認

		// インデックスバッファ作成
		bool sts = Renderer::CreateIndexBuffer(
			(unsigned int)(indices.size()),				// インデックス数
			(void*)indices.data(),						// インデックスデータ先頭アドレス
			&m_IndexBuffer);							// インデックスバッファ

		assert(sts == true); //結果を確認
	}

	void SetGPU()
	{
		// デバイスコンテキスト取得
		ID3D11DeviceContext* devicecontext = nullptr;
		devicecontext = Renderer::GetDeviceContext();

		// インデックスバッファをセット
		devicecontext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
};
