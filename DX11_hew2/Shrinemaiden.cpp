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
	SetTargetSpeed(2.0f);   // 好みで
	SetMaxSpeed(5.0f);      // 巫女の最高速度
	SetAcceleration(0.1f);  // 1フレームあたりの加速度（大きすぎると一瞬でMAX）
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
	Vector3 now_pos = GetPosition();
	Vector3 escapeDir = Vector3::Zero;

	// ===============================
	// 1) 逃?方向計算
	//    ※ stunTimer > 0 時「不更新方向」
	// ===============================
	vector<Enemy_base*> enemies = Game::GetInstance()->GetObjects<Enemy_base>();
	if (stunTimer <= 0.0f)
	{
		for (auto e : enemies)
		{
			if (!e || !e->GetIsAlive()) continue;
			Vector3 fromEnemy = now_pos - e->GetPosition();
			float distSq = fromEnemy.LengthSquared();
			if (distSq < 1e-8f) continue;
			escapeDir += fromEnemy / distSq;
		}

		if (escapeDir.LengthSquared() > 1e-4f)
		{
			escapeDir.Normalize();
			SetDirection(escapeDir);

			float v = GetVelocity() + m_acceleration;
			if (v > m_maxSpeed) v = m_maxSpeed;
			SetVelocity(v);
		}
		else
		{
			float v = GetVelocity() - m_deceleration;
			if (v < m_stop_speed) v = 0.0f;
			SetVelocity(v);
		}
	}
	else
	{
		// 切線方向維持時間中：只倒數，不重新算方向
		stunTimer -= 1.0f;
		if (stunTimer < 0.0f) stunTimer = 0.0f;
	}

	// ===============================
	// 2) 位置更新 + フィールド境界
	// ===============================
	Vector3 vel = m_direction * m_velocity;
	Vector3 proposedPos = now_pos + vel;

	if (m_Field)
	{
		Vector3 pos = proposedPos;
		Vector3 adjVel = vel;
		bool hitBorder = m_Field->ResolveBorder(pos, adjVel, m_Radius);

		if (hitBorder)
		{
			// 壁に当たった瞬間のみ切線方向を決定
			if (stunTimer <= 0.0f)
			{
				Vector3 dir = (adjVel.LengthSquared() > 1e-8f) ? adjVel : vel;
				if (dir.LengthSquared() > 1e-8f)
				{
					dir.Normalize();
					Vector3 leftTangent = Vector3(-dir.y, dir.x, 0.0f);
					Vector3 rightTangent = Vector3(dir.y, -dir.x, 0.0f);

					Vector3 chosen = rightTangent;
					if (escapeDir.LengthSquared() > 1e-8f)
					{
						Vector3 esc = escapeDir;
						esc.Normalize();
						chosen = (esc.Dot(leftTangent) > esc.Dot(rightTangent))
							? leftTangent
							: rightTangent;
					}

					m_wallSlideDir = chosen;   // ★ 切線方向鎖定
					stunTimer = 30.0f;          // ★ 維持時間（建議 6～12）
				}
			}

			// stunTimer > 0 時，一律使用鎖定的切線方向
			if (m_wallSlideDir.LengthSquared() > 1e-8f)
			{
				float speed = vel.Length();
				adjVel = m_wallSlideDir * speed;
				SetDirection(m_wallSlideDir);

				pos = now_pos + adjVel;
				m_Field->ResolveBorder(pos, adjVel, m_Radius);

				proposedPos = pos;
				vel = adjVel;
			}
			else
			{
				// 念のための保険
				proposedPos = pos;
				vel = adjVel;
			}
		}
		else
		{
			// 壁から離れたら切線ロック解除
			m_wallSlideDir = Vector3::Zero;
			proposedPos = now_pos + vel;
		}
	}

	SetPosition(proposedPos);
}
