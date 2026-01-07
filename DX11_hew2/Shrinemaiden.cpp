#include "Shrinemaiden.h"
#include "Enemy_base.h"
#include "Field.h"
#include "Game.h"
#include "silkWall.h"

using namespace std;
using namespace DirectX::SimpleMath;

void Shrinemaiden::Init()
{
	SetMinSpeed(0.0f);
	SetTargetSpeed(1.0f);   // 
	SetMaxSpeed(2.5f);      // 最高速度
	SetAcceleration(0.08f); // 1フレームあたりの加速度（大きすぎると一瞬でMAX）
	SetSerchDistance(200.f);
	SetVelocity(0.0f);      // 初速ゼロ
	SetDirection(Vector3(1.0f, 0.0f, 0.0f)); // 初期向き（何でもOK）
	SetIsAlive(true);
	m_Radius = 25.0f; //個別調整

	//初期化処理
	m_Texture2D.Texture2D::Init();

	//m_Texture2D.SetTexture("assets/texture/miko.png");
	m_Texture2D.SetTexture("assets/texture/mikoanime.png");
	m_Texture2D.SetPosition(m_Position);
	m_Texture2D.SetRotation(m_Rotation);
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0.0f);
	//m_Texture2D.SetFlipX(1); // 左右反転

	m_Collider.center = GetPosition();
	m_Collider.radius = m_Radius;
	SetDrawOrder(6);

	//animation 設定
	m_Texture2D.SetSpriteSheet(2, 2);
	m_Texture2D.AddAnimClip("idle", 0, 1, 10);
	m_Texture2D.AddAnimClip("yowa", 2, 3, 10);

	m_Texture2D.PlayAnim("idle");
}

void Shrinemaiden::Update()
{
	move();

	m_Texture2D.Update();//アニメーション更新
	if (m_velocity < 1.0f)
	{
		m_Texture2D.PlayAnim("yowa");
	}
	else {
		m_Texture2D.PlayAnim("idle");
		//m_Texture2D.StopAnimation();
	}


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
	Vector3 now_pos = GetPosition();        // 現在位置
	Vector3 escapeDir = Vector3::Zero;      // 逃避方向

	// ===============================
	// 1) 逃避方向ベクトル計算
	// ===============================
	vector<Enemy_base*> enemies = Game::GetInstance()->GetObjects<Enemy_base>();
	if (m_wallSlideTimer <= 0.0f)
	{
		for (auto& e : enemies)
		{
			if (!e || !e->GetIsAlive()) continue;

			if ((e->GetPosition() - now_pos).LengthSquared() > m_serchDistance * m_serchDistance) continue;

			Vector3 fromEnemy = now_pos - e->GetPosition();
			float distSq = fromEnemy.LengthSquared();
			if (distSq < 1e-8f) continue;

			escapeDir += fromEnemy / distSq;
		}

		if (escapeDir.LengthSquared() > 1e-4f)
		{
			escapeDir.Normalize();
			SetDirection(escapeDir);

			float v = m_velocity + m_acceleration;
			if (v > GetTargetSpeed())v = GetTargetSpeed();
			SetVelocity(v);
		}
		else
		{
			// 敵がいない場合はステージ中心へ戻る
			const float centerRadius = 10.0f;
			const float centerRadiusSq = centerRadius * centerRadius;

			Vector3 toCenter = Vector3::Zero - now_pos;

			if (toCenter.LengthSquared() > centerRadiusSq)
			{
				toCenter.Normalize();
				SetDirection(toCenter);

				float v = m_velocity + m_acceleration;
				if (v > m_maxSpeed) v = m_maxSpeed;
				SetVelocity(v);
			}
			else
			{
				SetVelocity(0.0f);
			}
		}
	}
	else
	{
		m_wallSlideTimer -= 1.0f;
		if (m_wallSlideTimer < 0.0f) m_wallSlideTimer = 0.0f;
	}

	// ===============================
	// 2) 速度・目標位置計算
	// ===============================
	Vector3 vel = m_direction * m_velocity;
	Vector3 pos = now_pos + vel;   //

	// ===============================
	// 3) 壁すり抜け防止（切線方向維持）
	// ===============================
	if (m_Field)
	{
		Vector3 adjVel = vel;

		hitBorder = m_Field->ResolveBorder(pos, adjVel, m_Radius);

		if (hitBorder)
		{
			// 壁に当たった瞬間のみ切線方向を決定
			if (m_wallSlideTimer <= 0.0f)
			{
				Vector3 dir = (adjVel.LengthSquared() > 1e-8f) ? adjVel : vel;

				if (dir.LengthSquared() > 1e-8f)
				{
					dir.Normalize();

					Vector3 leftTangent(-dir.y, dir.x, 0.0f);
					Vector3 rightTangent(dir.y, -dir.x, 0.0f);

					Vector3 chosen = rightTangent;
					if (escapeDir.LengthSquared() > 1e-8f)
					{
						Vector3 esc = escapeDir;
						esc.Normalize();
						chosen = (esc.Dot(leftTangent) > esc.Dot(rightTangent))
							? leftTangent
							: rightTangent;
					}

					m_wallSlideDir = chosen;
					m_wallSlideTimer = 16.0f;
				}
			}

			// stunTimer > 0 時は切線方向固定
			if (m_wallSlideDir.LengthSquared() > 1e-8f)
			{
				float speed = vel.Length();
				adjVel = m_wallSlideDir * speed;
				SetDirection(m_wallSlideDir);

				pos = now_pos + adjVel;
				m_Field->ResolveBorder(pos, adjVel, m_Radius);

				vel = adjVel;
			}
			else
			{
				vel = adjVel;
			}
		}
		else
		{
			// 壁から離れたら切線解除
			m_wallSlideDir = Vector3::Zero;
		}
	}
	vector<silkWall*> silkWalls = Game::GetInstance()->GetObjects<silkWall>();
	for (auto w : silkWalls)
	{
		Vector3 contactPoint;
		if (Collision::CheckHit(w->GetSegment(), m_Collider, contactPoint))
		{
			// 衝突したらバックさせてスタン
			m_velocity = 0.5f;
			m_wallSlideTimer = 1.f; // スタンタイマーをセット
			//Vector3 now_pos = GetPosition();
			Vector3 knockbackDir = now_pos - contactPoint;
			m_direction = knockbackDir;
			//SetPosition(GetPosition() + knockbackDir * 2.0f); // 少し後退
			break;
		}
	}

	SetPosition(pos);
}
