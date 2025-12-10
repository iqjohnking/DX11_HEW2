#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

class MeshRenderer {
protected:
	VertexBuffer<VERTEX_3D>	m_VertexBuffer;		// 頂点バッファ
	IndexBuffer				m_IndexBuffer;		// インデックスバッファ
	int						m_IndexNum = 0;		// インデックス数
public:
	virtual void Init(Mesh& mesh) 
	{
		m_VertexBuffer.Create(mesh.GetVertices());
		m_IndexBuffer.Create(mesh.GetIndices());
		m_IndexNum = static_cast<int>(mesh.GetIndices().size());
	}

	// 描画前処理
	virtual void BeforeDraw()
	{
		ID3D11DeviceContext* devicecontext = Renderer::GetDeviceContext();

		// トポロジーをセット（旧プリミティブタイプ）
		devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_VertexBuffer.SetGPU();			// 頂点バッファをセット
		m_IndexBuffer.SetGPU();				// インデックスバッファをセット
	}

	// サブセット描画
	virtual void DrawSubset(unsigned int indexnum,unsigned int baseindex,unsigned int basevertexindex ) 
	{
		Renderer::GetDeviceContext()->DrawIndexed(
			indexnum,								// 描画するインデックス数（面数×３）
			baseindex,								// 最初のインデックスバッファの位置
			basevertexindex);						// 頂点バッファの最初から使う
	}

	// 描画
	virtual void Draw() 
	{
		BeforeDraw();								// 描画前処理

		Renderer::GetDeviceContext()->DrawIndexed(
			m_IndexNum,								// 描画するインデックス数（面数×３）
			0,										// 最初のインデックスバッファの位置
			0);										// 頂点バッファの最初から使う
	}
};
