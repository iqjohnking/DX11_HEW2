#include "Enemy1.h"
#include "Shrinemaiden.h"
#include "Field.h"
#include "Game.h"
#include "silkWall.h"

using namespace std;
using namespace DirectX::SimpleMath;

void Enemy1::Init()
{
	SetMinSpeed(0.0f);
	SetTargetSpeed(1.3f);   // 
	SetMaxSpeed(2.5f);      // 
	SetAcceleration(0.05f); // 1フレームあたりの加速度（大きすぎると一瞬でMAX）
	SetVelocity(0.0f);      // 初速ゼロ
	SetDirection(Vector3(1.0f, 0.0f, 0.0f)); // 初期向き（何でもOK）
	SetIsAlive(true);
	m_Radius = 25.0f; //個別調整

	//初期化処理
	m_Texture2D.Texture2D::Init();

	m_Texture2D.SetTexture("assets/texture/enemy.png");
	//SetPosition(100.0f, 100.0f, 0.0f); // 初期位置は外部で設定する想定
	m_Texture2D.SetRotation(m_Rotation); 
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0);

	m_Collider.center = GetPosition();
	m_Collider.radius = m_Radius;

	SetDrawOrder(4);
}

void Enemy1::Update()
{
	// IsActive が true のときだけ動作 
	// IsActive = false の場合はいないけど
	if (isActive) 
	{
		move();	
		
		m_Collider.center = GetPosition();
	}
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

	Vector3 now_pos = GetPosition();
	Vector3 target_pos = Vector3(0, 0, 0);
	Vector3 miko_pos = m_Miko->GetPosition();

	if (stunTimer <= 0) {
		// 1) 巫女へ向かう基本方向
		m_direction = miko_pos - now_pos;
		float len = m_direction.Length();
		if (len <= 0.0001f) return;
		m_direction /= len;

		// 速度調整
		if (m_velocity < m_TargetSpeed) {
			m_velocity += m_acceleration;
			if (m_velocity > m_TargetSpeed) {
				m_velocity = m_TargetSpeed;
			}
		}
		else if (m_velocity > m_TargetSpeed) {
			m_velocity -= m_acceleration;
			if (m_velocity < m_TargetSpeed) {
				m_velocity = m_TargetSpeed;
			}
		}
		// 速度低下中なら減速
		if (isSpdDown) {
			m_velocity *= 0.9f;
		}


		// 2) 敵同士の分離ステアリングを加算
		{
			// 他の敵一覧を取得
			auto enemies = Game::GetInstance()->GetObjects<Enemy_base>();
			Vector3 separation = Vector3::Zero;

			// 分離影響半径（重なり検出 + マージン）
			float selfR = m_Collider.radius;
			float margin = 6.0f; // 少し余裕（必要に応じ調整）
			for (auto* eb : enemies) {
				if (!eb || eb == this) continue;
				if (!eb->GetIsAlive()) continue;

				Vector3 otherPos = eb->GetPosition();
				float otherR = eb->GetRadius(); // 他の敵半径。共通なら eb から取得する実装に変更可
				float sumR = selfR + otherR + margin;

				Vector3 toSelf = now_pos - otherPos;
				float d2 = toSelf.LengthSquared();
				if (d2 <= 1e-8f) {
					// 同座標近傍はランダム微小押し出し
					separation += Vector3(0.01f, 0.0f, 0.0f);
					continue;
				}
				float d = sqrtf(d2);
				if (d < sumR) {
					// 重なり/過接近。距離に反比例で押し出す
					Vector3 push = toSelf / d; // 正規化
					float strength = (sumR - d) / sumR; // 0..1
					separation += push * strength;
				}
			}

			// 分離をブレンド（重みを調整）
			if (separation.LengthSquared() > 1e-6f) {
				separation.Normalize();
				float separationWeight = 0.8f; // 分離優先度（0..1で調整）
				Vector3 desired = m_direction * (1.0f - separationWeight) + separation * separationWeight;
				if (desired.LengthSquared() > 1e-8f) {
					desired.Normalize();
					SetDirection(desired);
					m_direction = desired;
				}
			}
		}
	}
	else {
		// スタン中は減速
		stunTimer -= 1;
		if (stunTimer < 0) stunTimer = 0;
		if (m_velocity < 0) m_velocity = 0;
	}

	// 3) フィールド外に出ないようにする
	Vector3 vel = GetDirectionXVelocity();
	bool isRunintoWall = m_Field->ResolveBorder(now_pos, vel, m_Collider.radius);
	if (isRunintoWall) {
		m_direction = vel;
		if (m_direction.LengthSquared() > 1e-8f) m_direction.Normalize();
		stunTimer = 60.f;
	}

	// 4) 絹の壁との衝突判定
	vector<silkWall*> silkWalls = Game::GetInstance()->GetObjects<silkWall>();
	for (auto w : silkWalls)
	{
		Vector3 contactPoint;
		if (Collision::CheckHit(w->GetSegment(), m_Collider, contactPoint))
		{
			// 衝突したらバックさせてスタン
			m_velocity = 0.5f;
			stunTimer = 1.f; // スタンタイマーをセット
			//Vector3 now_pos = GetPosition();
			Vector3 knockbackDir = now_pos - contactPoint;
			m_direction = knockbackDir;
			//SetPosition(GetPosition() + knockbackDir * 2.0f); // 少し後退
			break;
		}
	}

	// 5) 新しい位置
	target_pos = now_pos + (m_direction * m_velocity);
	SetPosition(target_pos);

	
	//float angleRad = atan2f(m_direction.y, m_direction.x);
	//m_Texture2D.SetRotationRad(0.0f, 0.0f, angleRad);
}