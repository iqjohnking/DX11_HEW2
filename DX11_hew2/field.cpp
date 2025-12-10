#include "field.h"

using namespace DirectX::SimpleMath;

void field::Init()
{
	// 親クラス的初始化
	Texture2D::Init();

	// 重複テクスチャを設定
	//SetRepeatTexture(m_RepeatTexture::m_false);

	// テクスチャ設定
	SetTexture("assets/texture/field.png");
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(1080.f, 1080.f, 0.0f);
}

void field::Update()
{
	
}