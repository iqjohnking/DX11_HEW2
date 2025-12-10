#include "playerHand.h"

using namespace DirectX::SimpleMath;

playerHand::playerHand(int side)
{
	if (side == 0)
	{
		SetSide(HandSide::Left);  // 左
	}
	else if (side == 1)
	{
		SetSide(HandSide::Right);  // 左
	}
	else
	{
		return;
	}
}

void playerHand::Init()
{
	// 初期化処理
	Texture2D::Init();

	// 手的
	SetTexture("assets/texture/hand.png");
	Vector3 initPosition = Vector3(0.0f, 0.0f, 0.0f);
	float initRadian = 0.0f;
	if (m_Side == HandSide::Left)
	{
		initPosition.x = -450.0f;  // 左
	}
	else if(m_Side == HandSide::Right)
	{
		initPosition.x = 450.0f;   // 右
		initRadian = PI;   // 右
	}
	else
	{
		return; //error
	}

	SetPosition(initPosition);
	SetRotationRad(0.0f, 0.0f, initRadian);
	SetScale(50.0f, 150.0f, 0.0f);

}

void playerHand::Update()
{
	Vector3 pos = GetPosition();
	float dy = 0.0f;
	if (m_Side == HandSide::Left)
	{
		if (Input::GetKeyPress(VK_W)||Input::GetButtonPress(XINPUT_UP))
		{
			dy += m_Speed;
		}
		if (Input::GetKeyPress(VK_S) || Input::GetButtonPress(XINPUT_DOWN))
		{
			dy -= m_Speed;
		}
	}
	else if (m_Side == HandSide::Right)
	{
		// 右手：↑ / ↓
		if (Input::GetKeyPress(VK_UP)|| Input::GetKeyPress(VK_I))
		{
			dy += m_Speed;
		}
		if (Input::GetKeyPress(VK_DOWN)|| Input::GetKeyPress(VK_K))
		{
			dy -= m_Speed;
		}
	}
	else
	{
		return; //error
	}

	pos.y += dy;

	SetPosition(pos);
}
