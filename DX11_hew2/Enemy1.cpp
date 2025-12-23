#include "Enemy1.h"
#include "Shrinemaiden.h"
using namespace DirectX::SimpleMath;

void Enemy1::Init()
{
	alive_flg_enemy = true;

	m_Radius = 25.0f; //個別調整

	//初期化処理
	Texture2D::Init();

	SetTexture("assets/texture/enemy.png");
	//SetPosition(100.0f, 100.0f, 0.0f); // 初期位置は外部で設定する想定
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(m_Radius * 2, m_Radius * 2, 0);

}

void Enemy1::Update()
{
	move();
}


void Enemy1::move()
{
	//巫女の座標をゲット
	Vector3 now_pos = GetPosition();
	Vector3 target_pos = Vector3(0, 0, 0);
	Vector3 miko_pos = m_Miko->Get_Shrinemaiden_pos();

	// 巫女の方向ベクトルを計算
	Vector3 direction = miko_pos - now_pos;
	// ベクトルの長さを計算
	float length = direction.Length();

	if (length > 0.0f) {
		// 正規化(1にする)
		direction /= length; 

		// 速度を調整
		float t = length / m_maxDist;
		t = std::clamp(t, 0.0f, 1.0f);
		t = t * t;

		m_speed = m_minSpeed + (m_maxSpeed - m_minSpeed) * t;

		// 移動量を計算
		float mikoSpeed = m_Miko->get_run_speed();
		mikoSpeed *= m_speed;
		direction *= mikoSpeed;

		// 新しい位置を計算
		//target_pos = now_pos + direction;
		//SetPosition(target_pos);
		//float angleRad = atan2f(direction.y, direction.x);
		//SetRotationRad(0.0f, 0.0f, angleRad);
	}
}

//void Enemy1::Add_Enemy()
//{
//	//今のところなし
//}