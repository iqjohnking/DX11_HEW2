#include "Enemy4.h"
#include "Shrinemaiden.h"
#include "Field.h"
#include "Game.h"
#include "silkWall.h"

using namespace std;
using namespace DirectX::SimpleMath;

void Enemy4::Init()
{
	SetMinSpeed(0.0f);
	SetTargetSpeed(6.0f);   // 
	SetMaxSpeed(10.0f);     // 使わない
	SetAcceleration(0.25f); // 1フレームあたりの加速度（大きすぎると一瞬でMAX）
	SetVelocity(0.0f);      // 初速ゼロ
	SetDirection(Vector3(1.0f, 0.0f, 0.0f)); // 初期向き（何でもOK）
	SetIsAlive(true);
	m_Radius = 25.0f; //個別調整

	//初期化処理
	m_Texture2D.Texture2D::Init();

	m_Texture2D.SetTexture("assets/texture/enemy_4_ani.png");
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

void Enemy4::Update()
{
	// IsActive が true のときだけ動作 
	// IsActive = false の場合はいないけど

	if (isActive)
	{
		move();
		m_Collider.center = GetPosition();
		m_Collider.radius = GetRadius();
		m_Texture2D.Update();
	}
}

void Enemy4::Draw(Camera* cam)
{
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0);
	m_Texture2D.SetPosition(GetPosition());
	m_Texture2D.Draw(cam);
}

void Enemy4::Uninit()
{
	m_Texture2D.Uninit();
}

void Enemy4::move()
{
	if (!m_Miko) return;

	Vector3 now_pos = GetPosition();
	Vector3 target_pos = Vector3(0, 0, 0);
	Vector3 miko_pos = m_Miko->GetPosition();

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
		bool hitThisFrame = false;

		// move() 内で使うので、まず同期（Update() で後から更新してても、判定はここで行う）
		m_Collider.center = now_pos;
		m_Collider.radius = GetRadius();

		//---- スタン（クールダウン）減算 ----
		if (stunTimer > 0.0f)
		{
			stunTimer -= 1.0f;
			if (stunTimer < 0.0f) stunTimer = 0.0f;
		}

		//------------------------------------------------------------------------------
		// WAIT(2~4秒) → CHARGE開始（方向は開始時に一回だけ決める：追跡しない）
		//------------------------------------------------------------------------------
		if (!isCharging)
		{
			// WAIT中は完全停止
			m_velocity = 0.0f;

			// 待機タイマーは回す（衝刺開始だけ cooldown 明けにする）
			chargeTimer++;

			if (chargeTimer > chargeTiming * 60 && stunTimer <= 0.0f)
			{
				isCharging = true;
				chargeTimer = 0;

				// 次回の待機時間
				chargeTiming = (float)get_rand_range(2, 4);

				chargeTarget = miko_pos;
				chargeTarget.z = 0.0f;
				chargeStart = now_pos;
				chargeStart.z = 0.0f;
				m_direction = chargeTarget - chargeStart;
				m_direction.z = 0.0f;

				float len = m_direction.Length();
				if (len <= 0.0001f)
				{
					isCharging = false; // 方向が取れないならやめる
				}
				else
				{
					m_direction /= len; // 正規化
				}
			}
		}

		//------------------------------------------------------------------------------
		// 絹の壁との衝突判定：当たったら「少し反発」→「待機へ戻す」
		//------------------------------------------------------------------------------
		{
			vector<silkWall*> silkWalls = Game::GetInstance()->GetObjects<silkWall>();
			for (auto* w : silkWalls)
			{
				if (!w) continue;

				Vector3 contactPoint;
				if (Collision::CheckHit(w->GetSegment(), m_Collider, contactPoint))
				{
					Vector3 knockbackDir = now_pos - contactPoint;
					knockbackDir.z = 0.0f; // XY平面

					if (knockbackDir.LengthSquared() > 1e-8f)
						knockbackDir.Normalize();
					else
						knockbackDir = Vector3(1.0f, 0.0f, 0.0f);

					// 固定距離だけ押し戻す
					SetPosition(now_pos + knockbackDir * kKnockbackDist);

					// 待機へ戻す
					isCharging = false;
					chargeTimer = 0;
					chargeTiming = (float)get_rand_range(2, 4);

					m_velocity = 0.0f;
					stunTimer = kHitCooldown; // 連続ヒット防止


					int dmg = w->GetPoiseDmg();
					m_Hitpoint -= dmg;

					hitThisFrame = true;
					break;
				}
			}
		}

		if (hitThisFrame)
			break;

		//------------------------------------------------------------------------------
		// CHARGE中のみ速度調整＆移動
		//------------------------------------------------------------------------------
		if (isCharging)
		{
			// 速度調整
			if (m_velocity < m_TargetSpeed)
			{
				m_velocity += m_acceleration;
				if (m_velocity > m_TargetSpeed) m_velocity = m_TargetSpeed;
			}
			else if (m_velocity > m_TargetSpeed)
			{
				m_velocity -= m_acceleration;
				if (m_velocity < m_TargetSpeed) m_velocity = m_TargetSpeed;
			}

			if (isSpdDown) m_velocity *= 0.9f;

			// 1フレームの移動量（XY）
			Vector3 step = m_direction * m_velocity;
			step.z = 0.0f;

			//------------------------------------------------------------------------------
			// フィールド外に出ないようにする
			// ★重要：ResolveBorder が返した修正後 step を使って「場内へ押し戻す」★
			//------------------------------------------------------------------------------
			bool isRunintoWall = m_Field->ResolveBorder(now_pos, step, m_Collider.radius);
			if (isRunintoWall)
			{
				// まず場内側へ押し戻す（これが無いと永遠に?界判定され続ける）
				SetPosition(now_pos + step);

				// 待機へ戻す
				if (stunTimer <= 0.0f) // 毎フレームリセットしない
				{
					isCharging = false;
					chargeTimer = 0;
					chargeTiming = (float)get_rand_range(2, 4);

					m_velocity = 0.0f;
					stunTimer = kHitCooldown;
				}

				break; // このフレームはここで終了
			}

			// 通常移動
			target_pos = now_pos + step;
			SetPosition(target_pos);

			//------------------------------------------------------------------------------
			// 到達判定（XY：z を落とす）
			//------------------------------------------------------------------------------
			Vector3 toTarget = chargeTarget - target_pos;
			toTarget.z = 0.0f;

			if (toTarget.LengthSquared() <= (kArriveRadius * kArriveRadius))
			{
				isCharging = false;
				chargeTimer = 0;
				chargeTiming = (float)get_rand_range(2, 4);

				m_velocity = 0.0f;
			}
		}

		if (m_Hitpoint <= 0)
		{
			state = EnemyState::DYING;
		}

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
		dyingTimer++;
		if (dyingTimer >= kMayuFrames)
		{
			dyingTimer = kMayuFrames;
			state = EnemyState::DEAD;
		}

		break;
	}
	case EnemyState::DEAD:
	{
		if (auto* stage = dynamic_cast<StageBase*>(Game::GetInstance()->GetScene()))
		{
			stage->AddStagekillCount();
		}
		Game::GetInstance()->DeleteObject(this);
		break;
	}
	default:
		break;
	}
}
