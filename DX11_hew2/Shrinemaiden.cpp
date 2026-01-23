#include "Shrinemaiden.h"
#include "EnemyBase.h"
#include "Field.h"
#include "Game.h"
#include "silkWall.h"

#include <algorithm> // std::clamp
#include <cmath>     // sqrtf, fabsf

using namespace std;
using namespace DirectX::SimpleMath;

//==================================================
// Utility
//==================================================

// 2D Cross (XY)
static float Cross2DXY(const Vector3& a, const Vector3& b)
{
	return a.x * b.y - a.y * b.x;
}

static float Area2D(const Vector3& a, const Vector3& b, const Vector3& c)
{
	return fabsf((b - a).Cross(c - a).z) * 0.5f;
}

static bool IsSegmentInTriangleFanXY(
	const Vector3& a,
	const Vector3& b,
	const Vector3& c,
	const Vector3& s0,
	const Vector3& s1)
{
	Vector3 dirB = b - a; dirB.z = 0.0f;
	Vector3 dirC = c - a; dirC.z = 0.0f;

	Vector3 d0 = s0 - a; d0.z = 0.0f;
	Vector3 d1 = s1 - a; d1.z = 0.0f;

	// 線分の中点もチェック（端点が外でも横切りを拾う）
	Vector3 mid = (s0 + s1) * 0.5f;
	Vector3 dm = mid - a; dm.z = 0.0f;

	auto InWedgeOneWay = [&](const Vector3& d) -> bool
		{
			float c1 = Cross2DXY(dirB, d);
			float c2 = Cross2DXY(d, dirC);
			return (c1 >= 0.0f && c2 >= 0.0f);
		};

	return InWedgeOneWay(d0) || InWedgeOneWay(d1) || InWedgeOneWay(dm);
}

//==================================================
// Init / Update / Draw
//==================================================

void Shrinemaiden::Init()
{
	SetMinSpeed(0.0f);
	SetTargetSpeed(5.0f);
	SetMaxSpeed(10.0f);
	SetAcceleration(0.2f);
	SetSerchDistance(200.0f);

	SetVelocity(0.0f);
	SetDirection(Vector3(1, 0, 0));
	SetIsAlive(true);

	m_Radius = 25.0f;

	m_Texture2D.Texture2D::Init();
	m_Texture2D.SetTexture("assets/texture/mikoanime.png");
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0.0f);

	m_Collider.center = GetPosition();
	m_Collider.radius = m_Radius;

	SetDrawOrder(6);

	m_Texture2D.SetSpriteSheet(4, 4);
	m_Texture2D.AddAnimClip("idle", 0, 3, 10);
	m_Texture2D.AddAnimClip("getH", 4, 7, 10);
	m_Texture2D.AddAnimClip("mayu", 8, 11, 10);
	m_Texture2D.AddAnimClip("test", 12, 15, 10);
	m_Texture2D.PlayAnim("idle");
}

void Shrinemaiden::Update()
{

	switch (m_MoveState)
	{
		case mikoState::SPAWNING:
		{

			// 出現アニメーション終了で生存状態へ
			// ゲーム開始直後に即移動し始めるのを防ぐため、ここでは何もしない
			--m_SPAWNINGTimer;
			if (m_SPAWNINGTimer <= 0)
			{
				m_MoveState = mikoState::ALIVE;
			}
			break;
		}
		case mikoState::ALIVE:
		{
			move();
			break;
		}
		case mikoState::ISMAYUING:
		{
			// 繭になっている演出
			//m_Texture2D.PlayAnim("getH");

			--m_MAYUINGTimer;
			int mayuingTimer = kMayuFrames - m_MAYUINGTimer;
			float t = (float)(mayuingTimer) / (float)kMayuFrames;
			if (t > 1.0f) t = 1.0f;
			Vector3 pos = m_StartMayuPos + (m_TargetMayuPos - m_StartMayuPos) * t;
			SetPosition(pos);

			// 繭になっていた演出
			if (mayuingTimer >= kMayuFrames)
			{
				// 必要がないかも？
				// stageシーンで m_MAYUINGTimer の値を検知して、result シーン へ遷移するようにしている
				--m_DYINGTimer;
				m_Texture2D.PlayAnim("mayu");
				if (m_DYINGTimer <= 0)
				{
					m_MoveState = mikoState::DEAD;
				}
			}

			break;

		}
		case mikoState::DYING:
		{
			// 消滅アニメーション再生中は移動しない
			break;
		}
		case mikoState::DEAD:
		{
			Game::GetInstance()->DeleteObject(this);
			break;
		}
		default:
		{
			break;
		}
	}

	// collider 同期
	m_Collider.center = GetPosition();


	m_Texture2D.Update();
}

void Shrinemaiden::Draw(Camera* cam)
{
	m_Texture2D.SetPosition(GetPosition());
	m_Texture2D.Draw(cam);
	DrawDebugTriangles(cam);
}

void Shrinemaiden::Uninit()
{
	m_Texture2D.Uninit();
}

//==================================================
// Move
//==================================================

void Shrinemaiden::move()
{
	const Vector3 now_pos = GetPosition();
	m_Collider.center = now_pos; // ここで必ず同期（move内で判定するため）

	auto* game = Game::GetInstance();
	const auto enemies = game->GetObjects<EnemyBase>();
	const auto silkWalls = game->GetObjects<silkWall>();

	// クールダウン減算
	if (m_RetreatCooldown > 0) m_RetreatCooldown--;

	//-----------------------------
	// stuck: 少し待って側にずらす
	//-----------------------------
	if (m_IsStuck)
	{
		if (m_StuckTimer < 30)
		{
			m_StuckTimer++;
			SetVelocity(0.0f);
			return;
		}

		Vector3 side(-m_LastFailedDir.y, m_LastFailedDir.x, 0.0f);
		if (side.LengthSquared() > 1e-4f) side.Normalize();

		const float nudge = m_Radius * 0.3f;
		Vector3 tryPos = now_pos + side * nudge;

		if (m_Field)
		{
			Vector3 dummyPos = now_pos;
			Vector3 dummyVel = tryPos - now_pos;
			if (!m_Field->ResolveBorder(dummyPos, dummyVel, m_Radius))
			{
				SetPosition(dummyPos + dummyVel);
			}
		}

		m_IsStuck = false;
		m_StuckTimer = 0;
		m_EscapeState = EscapeState::SearchEscapePoint;
		return;
	}

	//==================================================
	// A) 逃走位置探索
	//==================================================
	if (m_EscapeState == EscapeState::SearchEscapePoint)
	{
		m_DebugTris.clear();

		Vector3 bestCenter = Vector3::Zero;
		float bestArea = 0.0f;
		bool foundSafe = false;

		// --- 判定を小さく分割（構造を簡単にする）
		auto UnsafeByEnemies = [&](const Vector3& a, const Vector3& b, const Vector3& c) -> bool
			{
				Vector3 dirB = b - a; dirB.z = 0.0f;
				Vector3 dirC = c - a; dirC.z = 0.0f;
				const float dirLimitSq = m_serchDistance * m_serchDistance;

				for (auto* e : enemies)
				{
					if (!e || !e->GetIsAlive()) continue;

					Vector3 dirE = e->GetPosition() - a;
					dirE.z = 0.0f;

					// 扇形内 + 距離
					const float c1 = Cross2DXY(dirB, dirE);
					const float c2 = Cross2DXY(dirE, dirC);
					const float dirDistSq = dirE.x * dirE.x + dirE.y * dirE.y;

					if (((c1 >= 0.0f && c2 >= 0.0f) || (c1 <= 0.0f && c2 <= 0.0f)) &&
						dirDistSq <= dirLimitSq)
					{
						return true;
					}

					// 円 vs 三角形（buffer付き）
					if (DoesCircleIntersectTriangleXY(
						e->GetPosition(),
						e->GetRadius() + 50.0f,
						a, b, c))
					{
						return true;
					}
				}
				return false;
			};

		auto UnsafeBySilk = [&](const Vector3& a, const Vector3& b, const Vector3& c) -> bool
			{
				const float silkLimitSq = (m_serchDistance * 0.8f) * (m_serchDistance * 0.8f);

				for (auto* w : silkWalls)
				{
					if (!w || !w->IsActive()) continue;

					const auto& seg = w->GetSegment();

					Vector3 mid = (seg.start + seg.end) * 0.5f;
					Vector3 d = mid - a;
					d.z = 0.0f;

					if (d.LengthSquared() > silkLimitSq) continue;

					if (IsSegmentInTriangleFanXY(a, b, c, seg.start, seg.end))
						return true;
				}
				return false;
			};

		auto UnsafeByFailedDir = [&](const Vector3& a, const Vector3& b, const Vector3& c) -> bool
			{
				if (m_LastFailedDir.LengthSquared() <= 0.0f) return false;

				Vector3 dirToCenter = ((a + b + c) / 3.0f) - a;
				dirToCenter.z = 0.0f;

				if (dirToCenter.LengthSquared() > 1e-4f) dirToCenter.Normalize();
				else return false;

				const float dot = dirToCenter.Dot(m_LastFailedDir);
				return (dot > 0.85f);
			};

		if (m_Field)
		{
			const auto& edges = m_Field->GetEdges();
			for (const auto& ed : edges)
			{
				const Vector3 a = now_pos;
				const Vector3 b = ed.p0;
				const Vector3 c = ed.p1;

				const float area = Area2D(a, b, c);
				if (area <= 1e-4f) continue;

				if (UnsafeByEnemies(a, b, c)) continue;
				if (UnsafeBySilk(a, b, c))    continue;
				if (UnsafeByFailedDir(a, b, c)) continue;

				if (m_DrawDebugTris) m_DebugTris.push_back({ a, b, c });

				if (area > bestArea)
				{
					bestArea = area;
					bestCenter = (a + b + c) / 3.0f;
					foundSafe = true;
				}
			}
		}

		if (foundSafe)
		{
			m_EscapeTarget = bestCenter;
			m_EscapeState = EscapeState::MoveToEscapePoint;
		}
		else
		{
			// 退避直後の連続失敗は stuck に回す（乱跳防止）
			if (m_RetreatCooldown > 0)
			{
				SetVelocity(0.0f);
				m_IsStuck = true;
				m_StuckTimer = 0;
				m_EscapeState = EscapeState::SearchEscapePoint;
				return;
			}

			OnEscapeRouteFailed(now_pos);
			return;
		}
	}

	//==================================================
	// B) 逃走移動
	//==================================================
	if (m_EscapeState == EscapeState::MoveToEscapePoint)
	{
		Vector3 toTarget = m_EscapeTarget - now_pos;
		toTarget.z = 0.0f;

		if (toTarget.LengthSquared() <= m_EscapeArriveDist * m_EscapeArriveDist)
		{
			SetVelocity(0.0f);
			m_EscapeState = EscapeState::SearchEscapePoint;
			return;
		}

		toTarget.Normalize();
		SetDirection(toTarget);

		float v = m_velocity + m_acceleration;
		if (v > m_TargetSpeed) v = m_TargetSpeed;
		SetVelocity(v);
	}

	//==================================================
	// C) Field
	//==================================================
	Vector3 vel = m_direction * m_velocity;
	vel.z = 0.0f;

	if (m_Field)
	{
		Vector3 dummyPos = now_pos;
		Vector3 dummyVel = vel;
		bool hitBorder = m_Field->ResolveBorder(dummyPos, dummyVel, m_Radius);

		Vector3 resolvedPos;
		if ((dummyPos - now_pos).LengthSquared() > 1e-6f)
			resolvedPos = dummyPos;          // B: pos 已被修正
		else
			resolvedPos = dummyPos + dummyVel; // A: 用修正後 vel 前進

		resolvedPos.z = 0.0f;

		if (hitBorder)
		{
			// [FIX-1] 先套用 ResolveBorder 的修正結果（至少推回場?/沿牆滑動）
			SetPosition(resolvedPos);
			SetVelocity(0.0f);

			// [FIX-2] 如果修正後仍幾乎沒動（vel 被夾成 0），做一次「往場?」的小推開
			// ?的場地大概率以原點為中心，所以用 (0,0) - now_pos 當作往?方向
			if ((resolvedPos - now_pos).LengthSquared() < 1e-6f)
			{
				Vector3 inDir = Vector3::Zero - now_pos;
				inDir.z = 0.0f;

				if (inDir.LengthSquared() > 1e-6f)
				{
					inDir.Normalize();

					Vector3 p2 = now_pos;
					Vector3 v2 = inDir * (m_Radius * 0.8f); // 推回場?的距離，可調 0.5~1.5 倍半徑
					m_Field->ResolveBorder(p2, v2, m_Radius);

					Vector3 resolvedPos2;
					if ((p2 - now_pos).LengthSquared() > 1e-6f) resolvedPos2 = p2;
					else resolvedPos2 = p2 + v2;

					resolvedPos2.z = 0.0f;
					SetPosition(resolvedPos2);
				}
			}

			// 退回搜尋，避免連續退避亂跳
			m_EscapeState = EscapeState::SearchEscapePoint;
			m_RetreatCooldown = 20;
			return;
		}
	}

	//==================================================
	// D) SilkWall（壁と同一扱い）
	//==================================================
	for (auto* w : silkWalls)
	{
		if (!w || !w->IsActive()) continue;

		Vector3 contactPoint;
		if (Collision::CheckHit(w->GetSegment(), m_Collider, contactPoint))
		{
			OnEscapeRouteFailed(now_pos);
			return;
		}
	}

	//==================================================
	// E) 位置更新
	//==================================================
	SetPosition(now_pos + vel);
}

//==================================================
// 共通：逃走失敗処理（壁・Silk 共通）
//==================================================
void Shrinemaiden::OnEscapeRouteFailed(const Vector3& now_pos)
{
	auto* game = Game::GetInstance();
	const auto silkWalls = game->GetObjects<silkWall>();
	const auto enemies = game->GetObjects<EnemyBase>();

	// 1) 失敗方向記録
	Vector3 failDir = m_direction;
	failDir.z = 0.0f;
	if (failDir.LengthSquared() > 1e-4f) failDir.Normalize();
	m_LastFailedDir = failDir;

	// 2) 退避候補（後ろ、左右、斜め）
	Vector3 f = failDir;
	if (f.LengthSquared() <= 1e-4f) f = Vector3(1, 0, 0);
	else f.Normalize();

	Vector3 side(-f.y, f.x, 0.0f);
	if (side.LengthSquared() > 1e-4f) side.Normalize();

	// 退避距離
	const float dist = m_Radius * 0.5f;

	Vector3 diag1 = side - f; diag1.z = 0.0f;
	if (diag1.LengthSquared() > 1e-4f) diag1.Normalize();

	Vector3 diag2 = (-side) - f; diag2.z = 0.0f;
	if (diag2.LengthSquared() > 1e-4f) diag2.Normalize();

	Vector3 candidates[] =
	{
		now_pos + (-f) * dist,
		now_pos + (side)*dist,
		now_pos + (-side) * dist,
		now_pos + (diag1)*dist,
		now_pos + (diag2)*dist,
	};

	// はこの関数の外で clamp した finalPos を使うので、
	// ここでは silk / enemy のみチェックする
	auto IsFreeNoField = [&](const Vector3& candidatePos) -> bool
		{
			// SilkWall
			auto testCol = m_Collider;
			testCol.center = candidatePos;

			for (auto* w : silkWalls)
			{
				if (!w || !w->IsActive()) continue;
				Vector3 cp;
				if (Collision::CheckHit(w->GetSegment(), testCol, cp))
					return false;
			}

			// Enemy
			const float enemyBuffer = 30.0f;
			for (auto* e : enemies)
			{
				if (!e || !e->GetIsAlive()) continue;

				Vector3 d = e->GetPosition() - candidatePos;
				d.z = 0.0f;
				const float minDist = (m_Radius + e->GetRadius() + enemyBuffer);
				if (d.LengthSquared() < (minDist * minDist))
					return false;
			}

			return true;
		};

	// 3) 退避を試す（成功したらそこへ移動）
	for (auto& pCand : candidates)
	{
		Vector3 finalPos = pCand;

		// [FIX] hit==true でも continue しない
		// ResolveBorder の結果（clamp後）を finalPos として使う
		if (m_Field)
		{
			Vector3 p = now_pos;
			Vector3 v = pCand - now_pos;
			m_Field->ResolveBorder(p, v, m_Radius); // ★戻り値は無視
			finalPos = p + v;
		}

		if (IsFreeNoField(finalPos))
		{
			SetPosition(finalPos);
			SetVelocity(0.0f);
			m_EscapeState = EscapeState::SearchEscapePoint;

			m_RetreatCooldown = 20;
			return;
		}
	}

	// 4) 退路が無い：stuck
	SetVelocity(0.0f);
	m_EscapeState = EscapeState::SearchEscapePoint;
	m_IsStuck = true;
	m_StuckTimer = 0;
}


//==================================================
// Debug Draw
//==================================================

void Shrinemaiden::DrawDebugTriangles(Camera* cam)
{
	if (!m_DrawDebugTris) return;

	static DeBugLine2D line;
	static bool initialized = false;

	if (!initialized)
	{
		line.Texture2D::Init();
		line.Init();
		line.SetTexture("assets/texture/ui_back.png");
		initialized = true;
	}

	for (const auto& t : m_DebugTris)
	{
		line.SetLine(t.a, t.b); line.Draw(cam);
		line.SetLine(t.b, t.c); line.Draw(cam);
		line.SetLine(t.c, t.a); line.Draw(cam);
	}


	// アニメ
	//if (m_velocity < 1.0f) m_Texture2D.PlayAnim("test");
	//else                   m_Texture2D.PlayAnim("idle");

}

//==================================================
// Geometry
//==================================================

bool Shrinemaiden::IsPointInTriangleXY(
	const Vector3& p,
	const Vector3& a,
	const Vector3& b,
	const Vector3& c)
{
	float c1 = Cross2DXY(b - a, p - a);
	float c2 = Cross2DXY(c - b, p - b);
	float c3 = Cross2DXY(a - c, p - c);

	bool hasNeg = (c1 < 0) || (c2 < 0) || (c3 < 0);
	bool hasPos = (c1 > 0) || (c2 > 0) || (c3 > 0);
	return !(hasNeg && hasPos);
}

float Shrinemaiden::DistPointToSegmentSqXY(
	const Vector3& p,
	const Vector3& a,
	const Vector3& b)
{
	Vector3 ab = b - a;
	ab.z = 0.0f;
	const float abLenSq = ab.x * ab.x + ab.y * ab.y;

	if (abLenSq <= 1e-6f)
	{
		Vector3 d = p - a; d.z = 0.0f;
		return d.x * d.x + d.y * d.y;
	}

	Vector3 ap = p - a; ap.z = 0.0f;
	float t = (ap.x * ab.x + ap.y * ab.y) / abLenSq;
	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 closest = a + ab * t;
	closest.z = 0.0f;

	Vector3 d = p - closest; d.z = 0.0f;
	return d.x * d.x + d.y * d.y;
}

bool Shrinemaiden::DoesCircleIntersectTriangleXY(
	const Vector3& center,
	float radius,
	const Vector3& a,
	const Vector3& b,
	const Vector3& c)
{
	const float r2 = radius * radius;

	if (IsPointInTriangleXY(center, a, b, c))
		return true;

	if (DistPointToSegmentSqXY(center, a, b) <= r2) return true;
	if (DistPointToSegmentSqXY(center, b, c) <= r2) return true;
	if (DistPointToSegmentSqXY(center, c, a) <= r2) return true;

	return false;
}
