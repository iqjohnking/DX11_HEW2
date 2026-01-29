#include "UI_mikoHp.h"

using namespace std;
using namespace DirectX::SimpleMath;

void UI_mikoHp::Init()
{
	// 1920 : 1080 ÇäÓèÄÇ…ÇµÇΩà íuí≤êÆ
	m_Position = Vector3(0.f, 50.f, 0.f);
	m_Texture2D.Texture2D::Init();
	//m_Texture2D.SetRepeatTexture(m_RepeatTexture::xOnly);
	m_Texture2D.SetTexture("assets/texture/spiderSilknerau.png");
	m_Texture2D.SetPosition(Vector3::Zero);
	m_Texture2D.SetRotation(Vector3::Zero);
	m_Texture2D.SetScale(0.0f, 15.f, 0.0f);

	SetDrawOrder(2);
}

void UI_mikoHp::Update()
{
}

void UI_mikoHp::Draw(Camera* cam)
{
}

void UI_mikoHp::Uninit()
{
}
