#pragma once
#include "Object.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Material.h"
//#include "Collision.h"

static constexpr float	TWO_PI = 6.283185307f;
static constexpr float		PI = 3.1415926535;

enum class m_RepeatTexture
{
	m_false,
	m_true,
	xOnly,
	yOnly,
};

//-----------------------------------------------------------------------------
// Texture2Dクラス
//-----------------------------------------------------------------------------
class Texture2D : public Object
{
private:
	// 頂点データ
	std::vector<VERTEX_3D> m_Vertices;

	// インデックスデータ
	std::vector<unsigned int> m_Indices;

	// 描画の為の情報（メッシュに関わる情報）
	IndexBuffer m_IndexBuffer; // インデックスバッファ
	VertexBuffer<VERTEX_3D> m_VertexBuffer; // 頂点バッファ

	// 描画の為の情報（見た目に関わる部分）
	Texture m_Texture; // テクスチャ
	std::unique_ptr<Material> m_Material; //マテリアル

	// UV座標の情報
	float m_NumU = 1;
	float m_NumV = 1;
	float m_SplitX = 1;
	float m_SplitY = 1;

	// 繰り返し無効化
	m_RepeatTexture repeatState = m_RepeatTexture::m_false;

	// ピボット（モデルローカル座標、既定は原点）
	DirectX::SimpleMath::Vector3 m_Pivot = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

public:
	void Init();				
	void Update();				
	void Draw(Camera* cam);		
	void Uninit();				

	// テクスチャを指定
	void SetTexture(const char* imgname);

	// 位置を指定
	void SetPosition(const float& x, const float& y, const float& z);
	void SetPosition(const DirectX::SimpleMath::Vector3& pos);

	// 角度を指定
	void SetRotation(const float& x, const float& y, const float& z);
	void SetRotation(const float& z);
	void SetRotation(const DirectX::SimpleMath::Vector3& rot);

	void SetRotationRad(const float& x, const float& y, const float& z);
	void SetRotationRad(const DirectX::SimpleMath::Vector3& rot);

	// 大きさを指定
	void SetScale(const float& x, const float& y, const float& z);
	void SetScale(const DirectX::SimpleMath::Vector3& scl);
	DirectX::SimpleMath::Vector3 GetScale() const { return m_Scale; }

	// UV座標を指定
	void SetUV(const float& nu, const float& nv, const float& sx, const float& sy);

	// ピボット設定（モデルローカル座標）
	void SetPivot(const DirectX::SimpleMath::Vector3& pivot) { m_Pivot = pivot; }
	void SetPivot(const float& x, const float& y, const float& z) { m_Pivot = DirectX::SimpleMath::Vector3(x,y,z); }

	// テクスチャサイズ取得ラッパ（既存Textureのアクセサを使う）
	int GetTextureWidth() const { return m_Texture.GetWidth(); }
	int GetTextureHeight() const { return m_Texture.GetHeight(); }

	//繰り返し設定
	void SetRepeatTexture(m_RepeatTexture state) { repeatState = state; }
};

