#include "silkWall.h"

using namespace DirectX::SimpleMath;

void silkWall::Init()
{
	// 親クラス的初始化
	Texture2D::Init();

	// 重複テクスチャを有効に（レンダラー側でもサンプラがWRAPなら有効）
	EnableRepeatTexture(true);

	// テクスチャ設定
	SetTexture("assets/texture/spiderSilkKari.png");
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(500.f, 50.f, 0.0f);

	// ピボットを左端に設定（モデル空間で頂点が -0.5..0.5 の想定）
	SetPivot(-0.5f, 0.0f, 0.0f);
}

void silkWall::Update()
{
	Vector3 oldScale = m_Scale;
	oldScale.x += 1.1f; // 拉長物件的 X 軸
	SetScale(oldScale.x, 50.f, 0.0f);

	// テクスチャの元サイズを自動取得して繰り返し回数を計算
	int textureW = GetTextureWidth();
	int textureH = GetTextureHeight();

	// 0除算回避
	float fw = (textureW > 0) ? static_cast<float>(textureW) : 1.0f;
	float fh = (textureH > 0) ? static_cast<float>(textureH) : 1.0f;

	float repeatU = oldScale.x / fw;
	float repeatV = oldScale.y / fh;

	SetUV(0.0f, 0.0f, repeatU, repeatV);
}