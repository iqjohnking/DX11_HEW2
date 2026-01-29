#include "UI_mikoHp.h"
#include "Game.h"

using namespace std;
using namespace DirectX::SimpleMath;

void UI_mikoHp::Init()
{
	// 1920 : 1080 ‚ðŠî€‚É‚µ‚½ˆÊ’u’²®
	m_Position = Vector3(0.f, 50.f, 0.f);
	m_Texture2D.Texture2D::Init();
	//m_Texture2D.SetRepeatTexture(m_RepeatTexture::xOnly);
	m_Texture2D.SetTexture("assets/texture/mikoanime.png");
	m_Texture2D.SetPosition(Vector3::Zero);
	m_Texture2D.SetRotation(Vector3::Zero);
	m_Texture2D.SetScale(100.0f, 100.f, 0.0f);

	m_Texture2D.SetFlipX(true);
	m_Texture2D.SetSpriteSheet(4, 3);

	m_Texture2D.AddAnimClip("idle", 0, 3, 30);
	m_Texture2D.PlayAnim("idle");

	SetDrawOrder(10);
}

void UI_mikoHp::Update()
{
	m_Texture2D.Update();
}

void UI_mikoHp::Draw(Camera* cam)
{
	if (m_miko == nullptr)
	{
		return;
	}

	int hp = m_miko->GetHitpoint();
	hp = std::clamp(hp, 0, 3);

	const float iconSize = 100.0f;
	const float margin = 30.0f;
	const float spacing = 10.0f;

	const float leftX = -(1920 / 2) + margin + iconSize;
	const float topY = 1080/2 - margin - iconSize;

	for (int i = 0; i < hp; ++i)
	{
		const float x = leftX + i * (iconSize + spacing);
		const float y = topY;

		m_Texture2D.SetPosition(DirectX::SimpleMath::Vector3(x, y, 0.0f));
		m_Texture2D.Draw(cam);
	}
}

void UI_mikoHp::Uninit()
{
	m_Texture2D.Uninit();
}
