#include "Enemy3.h"
#include "Shrinemaiden.h"
//#include "EnemyMayu.h"
#include "Field.h"
#include "Game.h"
#include "silkWall.h"

using namespace std;
using namespace DirectX::SimpleMath;

void Enemy3::Init()
{
	SetMinSpeed(0.0f);
	SetTargetSpeed(1.3f);   // 
	SetMaxSpeed(2.5f);      // 
	SetAcceleration(0.05f); // 1フレームあたりの加速度（大きすぎると一瞬でMAX）
	SetVelocity(0.0f);      // 初速ゼロ
	SetDirection(Vector3(1.0f, 0.0f, 0.0f)); // 初期向き（何でもOK）
	SetIsAlive(true);
	m_Radius = 25.0f; //個別調整
	mayuingTimer = 0;

	//初期化処理
	m_Texture2D.Texture2D::Init();

	m_Texture2D.SetTexture("assets/texture/enemy_1_ani.png");
	//SetPosition(100.0f, 100.0f, 0.0f); // 初期位置は外部で設定する想定
	m_Texture2D.SetRotation(m_Rotation);
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0);

	m_Collider.center = GetPosition();
	m_Collider.radius = m_Radius;

	SetDrawOrder(4);

	m_Texture2D.SetSpriteSheet(4, 3);
	m_Texture2D.AddAnimClip("idle", 0, 3, 10);
	m_Texture2D.AddAnimClip("atk", 4, 7, 10);
	m_Texture2D.AddAnimClip("spawn", 8, 11, 10);
	m_Texture2D.AddAnimClip("dying", 8, 8, 20);
	m_Texture2D.PlayAnim("spawn");
	state = EnemyState::SPAWNING;
}

void Enemy3::Update()
{
	// IsActive が true のときだけ動作 
	// IsActive = false の場合はいないけど

	if (!isActive)
	{
		return;
	}

	switch (state) {
	case EnemyState::SPAWNING:
	{
		spawnTimer++;
		if (spawnTimer > 30) {
			state = EnemyState::ALIVE;
			m_Texture2D.PlayAnim("idle");
		}
		break;
	}

	case EnemyState::ALIVE:
	{
		move(); //enumはこれ中に入ってることに注意
		break;
	}
	case EnemyState::ISMAYUING:
	{
		mayuingTimer++; // 0 -> kMayuFrames

		float t = (float)mayuingTimer / (float)kMayuFrames;
		if (t > 1.0f) t = 1.0f;

		Vector3 pos = m_StartMayuPos + (m_TargetMayuPos - m_StartMayuPos) * t;
		SetPosition(pos);

		if (mayuingTimer >= kMayuFrames)
		{
			SetPosition(m_TargetMayuPos);
			mayuingTimer = kMayuFrames;
			state = EnemyState::DEAD;
		}
		break;
	}
	case EnemyState::DYING:
	{

		break;
	}
	case EnemyState::DEAD:
	{
		Game::GetInstance()->DeleteObject(this);
		break;
	}
	default:
		break;
	}



	m_Collider.center = GetPosition();
	m_Collider.radius = GetRadius();
	m_Texture2D.Update();
}

void Enemy3::Draw(Camera* cam)
{
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0);
	m_Texture2D.SetPosition(GetPosition());
	m_Texture2D.Draw(cam);
}

void Enemy3::Uninit()
{
	m_Texture2D.Uninit();
}

void Enemy3::move()
{
	if (!m_Miko) return;

	Vector3 now_pos = GetPosition();
	Vector3 target_pos = Vector3(0, 0, 0);

	//============================================================
	// 0) Mayu を探す：state==1 の中で一番近いものを追う
	//    ・近づいたら停止
	//    ・停止中 180f 計時したら Mayu を ISDESTROING にする
	//============================================================
	EnemyMayu* nearestMayu = nullptr;
	float bestD2 = FLT_MAX;

	auto mayus = Game::GetInstance()->GetObjects<EnemyMayu>();
	for (auto* m : mayus)
	{
		if (!m) continue;
		if (m->GetState() != 1) continue;

		const float d2 = (m->GetPosition() - now_pos).LengthSquared();
		if (d2 < bestD2)
		{
			bestD2 = d2;
			nearestMayu = m;
		}
	}

	// ターゲット更新（変わったらタイマーリセット）
	if (nearestMayu != m_TargetMayu)
	{
		m_TargetMayu = nearestMayu;
		m_MayuDestroyTimer = 0;

		m_Texture2D.PlayAnim("idle");

	}

	// Mayu がいるなら「到達判定」
	if (m_TargetMayu)
	{
		const float stopDist = m_TargetMayu->GetRadius() * 2.0f;
		const float stopDist2 = stopDist * stopDist;

		const float d2 = (m_TargetMayu->GetPosition() - now_pos).LengthSquared();
		if (d2 <= stopDist2)
		{
			// 停止
			m_velocity = 0.0f;

			// 180f 計時

			m_Texture2D.PlayAnim("atk");
			++m_MayuDestroyTimer;
			if (m_MayuDestroyTimer >= kMayuDestroyFrames)
			{
				// Mayu を破壊状態へ（中身は EnemyMayu 側であとから実装でOK）
				// どっちか使える方だけ残してね：
				m_TargetMayu->SetIsExploding(true);
				// m_TargetMayu->state = MayuState::ISDESTROING;

				m_TargetMayu = nullptr;
				m_MayuDestroyTimer = 0;

				m_Texture2D.PlayAnim("idle");
			}
			return; // このフレームは移動しない
		}
	}
	else
	{
		m_MayuDestroyTimer = 0;
	}

	//============================================================
	// 1) 追跡ターゲット（Mayu があれば Mayu、なければ巫女）
	//============================================================
	Vector3 chaseTarget = (m_TargetMayu) ? m_TargetMayu->GetPosition()
		: m_Miko->GetPosition();

	//============================================================
	// 2) 移動・分離・衝突（元のロジック）
	//============================================================
	if (stunTimer <= 0)
	{
		// (1) ターゲットへ向かう
		m_direction = chaseTarget - now_pos;
		float len = m_direction.Length();
		if (len <= 0.0001f) return;
		m_direction /= len;

		// 速度調整
		if (m_velocity < m_TargetSpeed)
		{
			m_velocity += m_acceleration;
			m_velocity = min(m_velocity, m_TargetSpeed);
		}
		else if (m_velocity > m_TargetSpeed)
		{
			m_velocity -= m_acceleration;
			m_velocity = max(m_velocity, m_TargetSpeed);
		}

		// 速度低下中なら減速
		if (isSpdDown) m_velocity *= 0.9f;

		// (2) 敵同士の分離
		auto enemies = Game::GetInstance()->GetObjects<EnemyBase>();
		Vector3 separation = Vector3::Zero;

		float selfR = m_Collider.radius;
		float margin = 6.0f;

		for (auto* eb : enemies)
		{
			if (!eb || eb == this) continue;
			if (!eb->GetIsAlive()) continue;

			Vector3 otherPos = eb->GetPosition();
			float otherR = eb->GetRadius();
			float sumR = selfR + otherR + margin;

			Vector3 toSelf = now_pos - otherPos;
			float d2 = toSelf.LengthSquared();
			if (d2 <= 1e-8f)
			{
				separation += Vector3(0.01f, 0.0f, 0.0f);
				continue;
			}

			float d = sqrtf(d2);
			if (d < sumR)
			{
				Vector3 push = toSelf / d;
				float strength = (sumR - d) / sumR; // 0..1
				separation += push * strength;
			}
		}

		// 分離ブレンド
		if (separation.LengthSquared() > 1e-6f)
		{
			separation.Normalize();
			float separationWeight = 0.8f;
			Vector3 desired = m_direction * (1.0f - separationWeight) + separation * separationWeight;
			if (desired.LengthSquared() > 1e-8f)
			{
				desired.Normalize();
				SetDirection(desired);
				m_direction = desired;
			}
		}
	}
	else
	{
		// スタン中
		stunTimer -= 1;
		if (stunTimer < 0) stunTimer = 0;
		if (m_velocity < 0) m_velocity = 0;
	}

	// (3) フィールド外に出ないようにする
	Vector3 vel = GetDirectionXVelocity();
	bool isRunintoWall = m_Field->ResolveBorder(now_pos, vel, m_Collider.radius);
	if (isRunintoWall)
	{
		m_direction = vel;
		if (m_direction.LengthSquared() > 1e-8f) m_direction.Normalize();
		stunTimer = 60.f;
	}

	// (4) 絹の壁との衝突判定
	vector<silkWall*> silkWalls = Game::GetInstance()->GetObjects<silkWall>();
	for (auto w : silkWalls)
	{
		Vector3 contactPoint;
		if (Collision::CheckHit(w->GetSegment(), m_Collider, contactPoint))
		{
			m_velocity = 0.5f;
			stunTimer = 1.f;
			Vector3 knockbackDir = now_pos - contactPoint;
			m_direction = knockbackDir;
			break;
		}
	}

	if (m_Hitpoint <= 0)
	{
		state = EnemyState::DYING;
	}

	// (5) 新しい位置
	target_pos = now_pos + (m_direction * m_velocity);
	SetPosition(target_pos);
}
