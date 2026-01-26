#include "TitleBG.h"

using namespace DirectX::SimpleMath;

void TitleBG::Init()
{
	// 親クラスの初期化処理を呼び出す
	Texture2D::Init();
	// テクスチャを指定
	SetTexture("assets/texture/haikei.png");
	// 位置を指定
	SetPosition(0.0f, 0.0f, 0.0f);
	// 角度を指定
	SetRotation(0.0f, 0.0f, 0.0f);
	// 大きさを指定
	SetScale(1920, 1080, 0.0f);

	SetDrawOrder(0);
}
void TitleBG::Update()
{
	//Vector3 oldPos = m_Position;
	//oldPos.x += 0.1f;
	//m_Position = oldPos;
}