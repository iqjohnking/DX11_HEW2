#include "Texture2D.h"

using namespace std;
using namespace DirectX::SimpleMath;

//=======================================
// 初期化処理
//=======================================
void Texture2D::Init()
{
	// 頂点データ
	m_Vertices.resize(4);

	m_Vertices[0].position = Vector3(-0.5f, 0.5f, 0);
	m_Vertices[1].position = Vector3( 0.5f, 0.5f, 0);
	m_Vertices[2].position = Vector3(-0.5f, -0.5f, 0);
	m_Vertices[3].position = Vector3( 0.5f, -0.5f, 0);

	m_Vertices[0].color = Color(1, 1, 1, 1);
	m_Vertices[1].color = Color(1, 1, 1, 1);
	m_Vertices[2].color = Color(1, 1, 1, 1);
	m_Vertices[3].color = Color(1, 1, 1, 1);

	m_Vertices[0].uv = Vector2(0, 0);
	m_Vertices[1].uv = Vector2(1, 0);
	m_Vertices[2].uv = Vector2(0, 1);
	m_Vertices[3].uv = Vector2(1, 1);

	// 頂点バッファ生成
	m_VertexBuffer.Create(m_Vertices);

	// インデックスバッファ生成
	m_Indices.resize(4);

	m_Indices[0] = 0;
	m_Indices[1] = 1;
	m_Indices[2] = 2;
	m_Indices[3] = 3;

	// インデックスバッファ生成
	m_IndexBuffer.Create(m_Indices);

	// シェーダオブジェクト生成
	m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");

	// マテリアル情報取得
	m_Material = std::make_unique<Material>();
	MATERIAL mtrl;
	mtrl.Diffuse = Color(1, 1, 1, 1);
	mtrl.TextureEnable = true; // テクスチャを使うか否かのフラグ
	m_Material->Create(mtrl);
}

//=======================================
// 更新処理
//=======================================
void Texture2D::Update()
{

}

//=======================================
// 描画処理
//=======================================
void Texture2D::Draw(Camera* cam)
{
	//カメラを選択する
	cam->SetCamera(1);

	// SRT情報作成
	Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	Matrix t = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	Matrix s = Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);

	// ピボット(Pivot)対応：モデルローカル座標の前後平行移動を挟む
	Matrix toPivot   = Matrix::CreateTranslation(-m_Pivot.x, -m_Pivot.y, -m_Pivot.z);
	Matrix fromPivot = Matrix::CreateTranslation(m_Pivot.x, m_Pivot.y, m_Pivot.z);

	// 新しいワールド行列：ピボットで移動 → スケール → 回転 → 元に戻す → 最後にオブジェクト位置を適用
	Matrix worldmtx = toPivot * s * r * fromPivot * t;
	//Matrix worldmtx = toPivot * s * r * t;
	Renderer::SetWorldMatrix(&worldmtx); // GPUにセット

	// 描画処理
	ID3D11DeviceContext* devicecontext = Renderer::GetDeviceContext();

	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();

	m_Texture.SetGPU();
	m_Material->SetGPU();

	// UVの設定を指定
	float u, v, uw, vh;


	switch (repeatState)
	{
	case m_RepeatTexture::m_false:
		u = m_NumU - 1;
		v = m_NumV - 1;
		uw = 1 / m_SplitX;
		vh = 1 / m_SplitY;
		break;

	case m_RepeatTexture::m_true:
		u = 0;
		v = 0;
		uw = m_Scale.x / m_Texture.GetWidth();
		vh = m_Scale.y / m_Texture.GetHeight();
		break;

	case m_RepeatTexture::xOnly:
		u = 0;
		v = 0;
		uw = m_Scale.x / m_Texture.GetWidth();
		vh = 1 / m_SplitY;
		break;

	case m_RepeatTexture::yOnly:
		u = 0;
		v = 0;
		uw = 1 / m_SplitX;
		vh = m_Scale.y / m_Texture.GetHeight();
		break;
	}

	Renderer::SetUV(u, v, uw, vh);

	devicecontext->DrawIndexed(
		(UINT)m_Indices.size(),
		0,
		0);
}

//=======================================
// 終了処理
//=======================================
void Texture2D::Uninit()
{

}

// テクスチャを指定
void Texture2D::SetTexture(const char* imgname)
{
	// テクスチャロード
	bool sts = m_Texture.Load(imgname);
	assert(sts == true);
}

// 位置を指定
void Texture2D::SetPosition(const float& x, const float& y, const float& z)
{
	Vector3 p = { x, y, z };
	SetPosition(p);
}
void Texture2D::SetPosition(const Vector3& pos)
{
	m_Position = pos;
}

// 角度を指定
void Texture2D::SetRotation(const float& x, const float& y, const float& z)
{
	Vector3 r = { x, y, z };
	SetRotation(r);
}
void Texture2D::SetRotation(const float& z)
{
	Vector3 r = { 0.0f, 0.0f, z };
	SetRotation(r);
}
void Texture2D::SetRotation(const Vector3& rot)
{
	m_Rotation = rot * PI /180; // degree→radianに変換
}

void Texture2D::SetRotationRad(const float& x, const float& y, const float& z)
{
	Vector3 r = { x, y, z };
	SetRotationRad(r);
}
void Texture2D::SetRotationRad(const Vector3& rot)
{
	m_Rotation = rot; // 已經是 rad，直接存
}

// 大きさを指定
void Texture2D::SetScale(const float& x, const float& y, const float& z)
{
	Vector3 s = { x, y, z };
	SetScale(s);
}
void Texture2D::SetScale(const Vector3& scl)
{
	m_Scale = scl;
}

// UV座標を指定
void Texture2D::SetUV(const float& nu, const float& nv, const float& sx, const float& sy)
{
	m_NumU = nu;  
	m_NumV = nv;
	m_SplitX = sx; // X方向の分割数
	m_SplitY = sy; // Y方向の分割数
}
