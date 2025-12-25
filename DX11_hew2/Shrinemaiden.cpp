#include "Shrinemaiden.h"
#include "Enemy_base.h"
#include "Field.h"
#include "Game.h"

using namespace std;
using namespace DirectX::SimpleMath;

void Shrinemaiden::Init()
{
	// ここを追加
	SetMinSpeed(0.0f);
	SetTargetSpeed(3.0f);   // 好みで
	SetMaxSpeed(5.0f);      // 巫女の最高速度
	SetAcceleration(0.2f);  // 1フレームあたりの加速度（大きすぎると一瞬でMAX）
	SetVelocity(0.0f);      // 初速ゼロ
	SetDirection(Vector3(1.0f, 0.0f, 0.0f)); // 初期向き（何でもOK）
	SetIsAlive(true);

	//初期化処理
	m_Texture2D.Texture2D::Init();

	m_Texture2D.SetTexture("assets/texture/miko.png");
	m_Texture2D.SetPosition(m_Position);
	m_Texture2D.SetRotation(0.0f, 0.0f, 0.0f);
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0.0f);

	m_Collider.center = GetPosition();
	m_Collider.radius = m_Radius;
}

void Shrinemaiden::Update()
{
	move();
	m_Collider.center = GetPosition();
}

void Shrinemaiden::Draw(Camera* cam)
{
	m_Texture2D.SetPosition(GetPosition());
	m_Texture2D.Draw(cam);
}

void Shrinemaiden::Uninit()
{
	m_Texture2D.Uninit();
}

void Shrinemaiden::move()
{
	Vector3 myPos = GetPosition();
	Vector3 escapeDir = Vector3::Zero;


	// 1) 全ての敵を取得
	vector<Enemy_base*> enemies = Game::GetInstance()->GetObjects<Enemy_base>();
	for (auto e : enemies) {
		if (!e || !e->GetIsAlive()) continue; // 無効な敵は無視

		Vector3 enemyPos = e->GetPosition();
		Vector3 fromEnemy = myPos - enemyPos;   // 敵 -> 巫女（敵から遠ざかる向き）

		float distSq = fromEnemy.LengthSquared();
		if (distSq <= 0.0001f) continue;        // 同じ位置/極端に近い場合は無視

		// 探索範囲外の敵は無視（必要なら）
		//if (distSq > m_serchDistance * m_serchDistance) continue;

		fromEnemy.Normalize(); // 方向ベクトルに変換//1になる

		// 近い敵ほど強く逃げる（1 / 距離^2）
		float weight = 1.0f / distSq;
		escapeDir += fromEnemy * weight; // 逃走方向に加算
	}
	// 2) 逃走方向の決定
	if (escapeDir.LengthSquared() > 0.0001f)
	{
		escapeDir.Normalize();

		// Character の移動パラメータを使って逃げる
		SetDirection(escapeDir);

		// 目標速度まで加速（値は好みで調整）
		float v = GetVelocity();
		//float dt = 1.0f / 60.0f;　// 仮の固定フレームレート
		v += m_acceleration;
		if (v > m_maxSpeed) v = m_maxSpeed;
		SetVelocity(v);
	}
	else 
	{
		// 周りに敵がいない → 減速して停止
		float v = GetVelocity();
		v -= m_deceleration;
		if (v < 0.0f) v = 0.0f;
		if (v < m_stop_speed) v = 0.0f;
		SetVelocity(v);
	}

	 // 3) 位置更新
	Vector3 vel = GetDirection() * GetVelocity();
	m_dirXvel = vel;
	m_Position += vel;
	SetPosition(m_Position);

	if (m_Field)
	{
		Vector3 pos = m_Position;
		Vector3 v = vel;
		if (m_Field->ResolveBorder(pos, v, m_Radius))
		{
			m_Position = pos;
			SetPosition(m_Position);

			if (v.LengthSquared() > 0.0001f)
			{
				SetDirection(v);
				SetVelocity(v.Length());
			}
		}
	}
}