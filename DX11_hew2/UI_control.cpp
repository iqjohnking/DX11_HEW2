#include "UI_control.h"
//#include "Game.h"

using namespace std;
using namespace DirectX::SimpleMath;
void UI_control::Init()
{
	m_control2D.Texture2D::Init();
	m_control2D.SetTexture("assets/texture/buttonhint003.png");
	float x = (1920 / 2) - 190 - 50;
	float y = -(1080 / 2) + 190 + 50;

	m_control2D.SetPosition(x, y, 0);
	m_control2D.SetRotation(Vector3::Zero);
	m_control2D.SetScale(380.0f, 380.f, 0.0f);

	m_control2D.SetSpriteSheet(1, 1);
	SetDrawOrder(10);
}

void UI_control::Update()
{
}

void UI_control::Draw(Camera* cam)
{
	m_control2D.Draw(cam);
}

void UI_control::Uninit()
{
	m_control2D.Uninit();
}