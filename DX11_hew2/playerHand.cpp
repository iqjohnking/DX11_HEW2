#include "playerHand.h"
#include "Field.h"
#include "Game.h"

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
	// フィールド情報の取得//外でやるべきかも
	m_Field = Game::GetInstance()->AddObject<Field>();
	SetFieldCenter(m_Field->GetCenter());
	SetRadius(m_Field->GetRadius() + 50.f );
	Vector3 initScale = Vector3(100.0f, 50.0f, 0.0f);
	SetScale(initScale);

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

	// 画像の初期化
	m_Texture2D.Texture2D::Init();
	m_Texture2D.SetTexture("assets/texture/hand.png");
	m_Texture2D.SetPosition(m_Position);
	m_Texture2D.SetRotation(m_Rotation);
	m_Texture2D.SetScale(100.0f, 50.0f, 0.0f);

	SetDrawOrder(2);
}

void playerHand::Update()
{
	Move();

}

void playerHand::Draw(Camera* cam)
{
	m_Texture2D.SetPosition(GetPosition()); // 親クラスの位置を反映
	m_Texture2D.SetScale(GetScale());		// 親クラスの大きさを反映
	m_Texture2D.SetRotation(GetRotation()); // 親クラスの回転を反映
	m_Texture2D.Draw(cam);
}

void playerHand::Uninit()
{
	m_Texture2D.Uninit();
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
	// 角度正規化（-PI ~ PI ）
	if (newAngle > PI)m_FiledAngleRad -= TWO_PI;
	else if (newAngle < -PI)m_FiledAngleRad += TWO_PI;

	// 2) 角度 + 半徑によって、newPosを計算
	// 角度だけど、実はフィールド壁に沿って動かすイメージ、回転することではない
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
		m_FiledAngleRad = newAngle; // 角度更新,次回以降の計算に使用
		pos = newPos;
		SetPosition(pos);
	}

	//　3) もう片方の手の方向を向く
	if (m_AnotherHand)
	{
		Vector3 otherPos = m_AnotherHand->GetPosition();
		Vector3 dir = otherPos - pos;

		float angleToBuddy = atan2f(dir.y, dir.x);   // 弧度
		//float angleDeg = XMConvertToDegrees(angleToOther);
		Vector3 newRot = Vector3(0.0f, 0.0f, angleToBuddy * (180.0f / PI)); // 角度に変換してデグリーに
		SetRotation(newRot);
		//SetRotationRad(0.0f, 0.0f, angleToOther);
	}
}
