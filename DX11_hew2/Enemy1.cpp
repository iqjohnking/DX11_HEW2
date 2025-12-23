#include "Enemy1.h"
#include "Shrinemaiden.h"
using namespace DirectX::SimpleMath;

void Enemy1::Init()
{
	m_Radius = 25.0f; //個別調整

	//初期化処理
	m_Texture2D.Texture2D::Init();
	m_Texture2D.SetTexture("assets/texture/enemy.png");
	//SetPosition(100.0f, 100.0f, 0.0f); // 初期位置は外部で設定する想定
	m_Texture2D.SetRotation(m_Rotation);
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0);

}

void Enemy1::Update()
{
	move();
}

void Enemy1::Draw(Camera* cam)
{
	m_Texture2D.SetPosition(GetPosition());
	m_Texture2D.Draw(cam);
}

void Enemy1::Uninit()
{
	m_Texture2D.Uninit();
}

void Enemy1::move()
{
	if (!m_Miko) return;

	// 現在の位置と目標位置を取得
	Vector3 now_pos = GetPosition();
	Vector3 target_pos = Vector3(0, 0, 0);
	//巫女の座標をゲット
	Vector3 miko_pos = m_Miko->Get_Shrinemaiden_pos();

	// 巫女の方向ベクトルを計算
	m_direction = miko_pos - now_pos;
	float length = m_direction.Length();	// ベクトルの長さを計算
	if (length <=  0.0001f) {
		return;
	}

	// 正規化(1にする)
	m_direction /= length;

	// 速度を調整
	float t = length / m_maxDist;
	t = std::clamp(t, 0.0f, 1.0f);
	t = t * t;

	// 基本速度
	//float baseSpeed = m_TargetSpeed; 

	float spdOffset = 0.0f; // 調整用オフセット値
	spdOffset = m_minSpeed + (m_maxSpeed - m_minSpeed) * t;

	// 移動量を計算
	float mikoSpeed = m_Miko->GetSpeed(); // 巫女現在の速度を取得
	SetMinSpeed(mikoSpeed * 1.005f); // 最小速度を巫女速度の50%に設定

	m_TargetSpeed = mikoSpeed * spdOffset;

	m_TargetSpeed = std::clamp(m_TargetSpeed, m_minSpeed, m_maxSpeed);

	if (m_velocity < m_TargetSpeed) {
		m_velocity += m_acceleration;
		if (m_velocity > m_TargetSpeed) m_velocity = m_TargetSpeed;
	}
	else if (m_velocity > m_TargetSpeed) {
		m_velocity -= m_acceleration;
		if (m_velocity < m_TargetSpeed) m_velocity = m_TargetSpeed;
	}

	// 新しい位置を計算
	target_pos = now_pos + (m_direction * m_velocity);
	SetPosition(target_pos);
	float angleRad = atan2f(m_direction.y, m_direction.x);

	m_Texture2D.SetRotationRad(0.0f, 0.0f, angleRad);
}