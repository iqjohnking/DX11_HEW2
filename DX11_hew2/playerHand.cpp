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
	//Vector3 initPosition = Vector3(0.0f, 0.0f, 0.0f);
	//float initRadian = 0.0f;

	SetScale(100.0f, 50.0f, 0.0f);

	SetFieldCenter(Vector3(0.0f, 0.0f, 0.0f));
	SetRadius(500.0f);

	if (m_Side == HandSide::Left)
	{
		Vector3 pos;
		m_FiledAngleRad += PI;
		pos.x = m_FiledCenter.x + cosf(m_FiledAngleRad) * m_Radius;
		pos.y = m_FiledCenter.y + sinf(m_FiledAngleRad) * m_Radius;
		pos.z = 0.0f;
		SetPosition(pos);
	}
	else if (m_Side == HandSide::Right)
	{
		Vector3 pos;
		pos.x = m_FiledCenter.x + cosf(m_FiledAngleRad) * m_Radius;
		pos.y = m_FiledCenter.y + sinf(m_FiledAngleRad) * m_Radius;
		pos.z = 0.0f;
		SetPosition(pos);
	}
	else
	{
		return; //error
	}

	//SetRotationRad(0.0f, 0.0f, initRadian);
	//SetAnotherHand(nullptr);

}

void playerHand::Update()
{
	Move();

}

void playerHand::Move()
{
	Vector3 pos = GetPosition();
	float dAngle = 0.0f;
	if (m_Side == HandSide::Left)
	{
		//if (Input::GetKeyPress(VK_W)||Input::GetButtonPress(XINPUT_UP))
		if (Input::GetKeyPress(VK_W))
		{
			dAngle -= m_AngleSpeed;
		}
		//if (Input::GetKeyPress(VK_S) || Input::GetButtonPress(XINPUT_DOWN))
		if (Input::GetKeyPress(VK_S))
		{
			dAngle += m_AngleSpeed;
		}
	}
	else if (m_Side == HandSide::Right)
	{
		// 右手：↑ / ↓
		if (Input::GetKeyPress(VK_UP) || Input::GetKeyPress(VK_I))
		{
			dAngle += m_AngleSpeed;
		}
		if (Input::GetKeyPress(VK_DOWN) || Input::GetKeyPress(VK_K))
		{
			dAngle -= m_AngleSpeed;
		}
	}
	else
	{
		return; //error
	}

	float newAngle = m_FiledAngleRad + dAngle;
	// 角度正規化（保持在 -PI ~ PI 之間，避免數字發散）
	if (newAngle > PI)m_FiledAngleRad -= TWO_PI;
	else if (newAngle < -PI)m_FiledAngleRad += TWO_PI;

	// 2) 由「角度 + 半徑」計算新位置
	Vector3 newPos;
	newPos.x = m_FiledCenter.x + cosf(newAngle) * m_Radius;
	newPos.y = m_FiledCenter.y + sinf(newAngle) * m_Radius;
	newPos.z = 0.0f;

	bool allowMove = true;
	if (m_Side == HandSide::Left)
	{
		// 左制限（x > center.x）
		if (newPos.x > m_FiledCenter.x - 1)
			allowMove = false;
	}
	else if (m_Side == HandSide::Right)
	{
		// 右制限（x < center.x）
		if (newPos.x < m_FiledCenter.x + 1)
			allowMove = false;
	}

	if (allowMove) {
		m_FiledAngleRad = newAngle;
		pos = newPos;
		SetPosition(pos);
	}

	if (m_AnotherHand)
	{
		Vector3 otherPos = m_AnotherHand->GetPosition();
		Vector3 dir = otherPos - pos;

		float angleToOther = atan2f(dir.y, dir.x);   // 弧度
		//float angleDeg = XMConvertToDegrees(angleToOther);
		SetRotationRad(0.0f, 0.0f, angleToOther);
	}
}
