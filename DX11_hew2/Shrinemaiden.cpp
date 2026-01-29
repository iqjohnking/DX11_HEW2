#include "Shrinemaiden.h"
#include "Field.h"
#include "Game.h"

#include <algorithm> // std::clamp
#include <cmath>     // fabsf

using namespace std;
using namespace DirectX::SimpleMath;

//==================================================
// Utility (local)
//==================================================

namespace
{
	constexpr float kEpsSq = 1e-6f;

	// 2D Cross (XY)
	inline float Cross2DXY(const Vector3& a, const Vector3& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	inline float Area2DXY(const Vector3& a, const Vector3& b, const Vector3& c)
	{
		return fabsf((b - a).Cross(c - a).z) * 0.5f;
	}

	// Segment inside triangle fan wedge (accept both windings)
	inline bool IsSegmentInTriangleFanXY(
		const Vector3& a,
		const Vector3& b,
		const Vector3& c,
		const Vector3& s0,
		const Vector3& s1)
	{
		Vector3 dirB = b - a; dirB.z = 0.0f;
		Vector3 dirC = c - a; dirC.z = 0.0f;

		auto InWedgeBothWays = [&](const Vector3& d) -> bool
			{
				float c1 = Cross2DXY(dirB, d);
				float c2 = Cross2DXY(d, dirC);
				return (c1 >= 0.0f && c2 >= 0.0f) || (c1 <= 0.0f && c2 <= 0.0f);
			};

		Vector3 d0 = s0 - a; d0.z = 0.0f;
		Vector3 d1 = s1 - a; d1.z = 0.0f;

		Vector3 mid = (s0 + s1) * 0.5f;
		Vector3 dm = mid - a; dm.z = 0.0f;

		return InWedgeBothWays(d0) || InWedgeBothWays(d1) || InWedgeBothWays(dm);
	}
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

	m_Radius = 35.0f;

	m_Texture2D.Texture2D::Init();
	m_Texture2D.SetTexture("assets/texture/mikoanime.png");
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0.0f);

	m_Collider.center = GetPosition();
	m_Collider.radius = m_Radius;

	SetDrawOrder(7);

	m_Texture2D.SetSpriteSheet(4, 3);
	m_Texture2D.AddAnimClip("idle", 0, 3, 10);
	m_Texture2D.AddAnimClip("getH", 4, 7, 10);
	m_Texture2D.AddAnimClip("mayu", 8, 11, 10);
	//m_Texture2D.AddAnimClip("test", 12, 15, 10);
	m_Texture2D.PlayAnim("idle");

	// 無敵・検索フラグ初期化
	m_MutekiTimer = 0;
	m_IgnoreEnemyInSearch = false;

	m_Hitpoint = 3;
}

void Shrinemaiden::Update()
{
	switch (m_MoveState)
	{
	case mikoState::SPAWNING:
	{
		--m_SPAWNINGTimer;
		if (m_SPAWNINGTimer <= 0)
		{
			m_MoveState = mikoState::ALIVE;
		}
		break;
	}
	case mikoState::ALIVE:
	{
		// 無敵時間減算（ALIVE中のみ）
		if (m_MutekiTimer > 0)
		{
			--m_MutekiTimer;
			if (m_MutekiTimer <= 80)
			{
				m_IgnoreEnemyInSearch = false;
			}

			// 無敵終了
			if (m_MutekiTimer == 0)
			{
				m_Texture2D.PlayAnim("idle"); // ★無敵じゃない時は idle に戻す
			}
		}
		if (m_Hitpoint <= 0)
		{
			m_MoveState = mikoState::DYING;
		}

		move();
		break;
	}
	case mikoState::ISMAYUING:
	{
		// 繭移行（線形補間）
		--m_MAYUINGTimer;

		int mayuingTimer = kMayuFrames - m_MAYUINGTimer;
		float t = (float)mayuingTimer / (float)kMayuFrames;
		if (t > 1.0f) t = 1.0f;

		Vector3 pos = m_StartMayuPos + (m_TargetMayuPos - m_StartMayuPos) * t;
		pos.z = 0.0f;
		SetPosition(pos);

		// 繭演出後のタイマー
		if (mayuingTimer >= kMayuFrames)
		{
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
		--m_DYINGTimer;
		m_Texture2D.PlayAnim("getH");
		if (m_DYINGTimer <= 0)
		{
			m_MoveState = mikoState::DEAD;
		}
		break;
	}
	case mikoState::DEAD:
	{
	//	Game::GetInstance()->DeleteObject(this);
		break;
	}
	default:
		break;
	}

	// collider 同期
	m_Collider.center = GetPosition();
	m_Texture2D.Update();
}

void Shrinemaiden::Draw(Camera* cam)
{
	m_Texture2D.SetPosition(GetPosition());
	m_Texture2D.SetScale(m_Radius * 2 +30.f , m_Radius * 2 + 30.f, 0.0f);


	m_Texture2D.Draw(cam);
	//DrawDebugTriangles(cam);
}

void Shrinemaiden::Uninit()
{
	m_Texture2D.Uninit();
}

//==================================================
// Move (managed / refactored + muteki)
//==================================================

bool Shrinemaiden::TryStuckNudge(const Vector3& now_pos)
{
	if (!m_IsStuck) return false;

	if (m_StuckTimer < 30)
	{
		m_StuckTimer++;
		SetVelocity(0.0f);
		return true;
	}

	Vector3 side(-m_LastFailedDir.y, m_LastFailedDir.x, 0.0f);
	if (side.LengthSquared() > 1e-4f) side.Normalize();

	const float nudge = m_Radius * 0.3f;
	Vector3 nextPos = now_pos + side * nudge;

	if (m_Field)
	{
		Vector3 p = now_pos;
		Vector3 v = nextPos - now_pos;
		m_Field->ResolveBorder(p, v, m_Radius);
		nextPos = p + v;
		nextPos.z = 0.0f;
	}

	SetPosition(nextPos);

	m_IsStuck = false;
	m_StuckTimer = 0;
	m_EscapeState = EscapeState::SearchEscapePoint;
	return true;
}

bool Shrinemaiden::SearchEscapeTarget(
	const Vector3& now_pos,
	const std::vector<EnemyBase*>& enemies,
	const std::vector<silkWall*>& silkWalls)
{
	if (m_EscapeState != EscapeState::SearchEscapePoint) return true;

	// debug triangles
	if (m_DrawDebugTris)
	{
		m_DebugTris.clear();
		if (m_Field) m_DebugTris.reserve(m_Field->GetEdges().size());
	}

	Vector3 bestCenter = Vector3::Zero;
	float bestArea = 0.0f;
	bool foundSafe = false;

	auto UnsafeByEnemies = [&](const Vector3& a, const Vector3& b, const Vector3& c) -> bool
		{
			// ★無敵中：探索では敵を考慮しない
			if (m_IgnoreEnemyInSearch)
				return false;

			Vector3 dirB = b - a; dirB.z = 0.0f;
			Vector3 dirC = c - a; dirC.z = 0.0f;
			const float dirLimitSq = m_serchDistance * m_serchDistance;

			for (auto* e : enemies)
			{
				if (!e || !e->GetIsAlive()) continue;

				Vector3 dirE = e->GetPosition() - a;
				dirE.z = 0.0f;

				// distance early reject
				const float distSq = dirE.x * dirE.x + dirE.y * dirE.y;

				// fan check (both winding)
				const float c1 = Cross2DXY(dirB, dirE);
				const float c2 = Cross2DXY(dirE, dirC);

				if (((c1 >= 0.0f && c2 >= 0.0f) || (c1 <= 0.0f && c2 <= 0.0f)) &&
					distSq <= dirLimitSq)
				{
					return true;
				}

				// circle vs triangle (buffer)
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
			const float silkLimit = (m_serchDistance * 0.8f);
			const float silkLimitSq = silkLimit * silkLimit;

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
			if (m_LastFailedDir.LengthSquared() <= kEpsSq) return false;

			Vector3 dirToCenter = ((a + b + c) / 3.0f) - a;
			dirToCenter.z = 0.0f;
			if (dirToCenter.LengthSquared() <= kEpsSq) return false;

			dirToCenter.Normalize();
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

			const float area = Area2DXY(a, b, c);
			if (area <= 1e-4f) continue;

			if (UnsafeByEnemies(a, b, c))      continue;
			if (UnsafeBySilk(a, b, c))         continue;
			if (UnsafeByFailedDir(a, b, c))    continue;

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
		return true;
	}

	// not found
	if (m_RetreatCooldown > 0)
	{
		SetVelocity(0.0f);
		m_IsStuck = true;
		m_StuckTimer = 0;
		m_EscapeState = EscapeState::SearchEscapePoint;
		return false;
	}

	OnEscapeRouteFailed(now_pos, silkWalls, enemies);
	return false;
}

bool Shrinemaiden::UpdateEscapeMove(const Vector3& now_pos, Vector3& outVel)
{
	outVel = Vector3::Zero;

	if (m_EscapeState != EscapeState::MoveToEscapePoint) return true;

	Vector3 toTarget = m_EscapeTarget - now_pos;
	toTarget.z = 0.0f;

	if (toTarget.LengthSquared() <= (m_EscapeArriveDist * m_EscapeArriveDist))
	{
		SetVelocity(0.0f);
		m_EscapeState = EscapeState::SearchEscapePoint;
		return false;
	}

	toTarget.Normalize();
	SetDirection(toTarget);

	float v = m_velocity + m_acceleration;
	if (v > m_TargetSpeed) v = m_TargetSpeed;
	SetVelocity(v);

	outVel = m_direction * m_velocity;
	outVel.z = 0.0f;
	return true;
}

bool Shrinemaiden::ApplyField(const Vector3& now_pos, const Vector3& vel, Vector3& outNextPos)
{
	outNextPos = now_pos + vel;
	outNextPos.z = 0.0f;

	if (!m_Field) return false;

	Vector3 p = now_pos;
	Vector3 v = vel;
	const bool hit = m_Field->ResolveBorder(p, v, m_Radius);

	outNextPos = p + v;
	outNextPos.z = 0.0f;
	return hit;
}

bool Shrinemaiden::CheckSilkCollisionAt(
	const Vector3& pos,
	const std::vector<silkWall*>& silkWalls) const
{
	auto testCol = m_Collider;
	testCol.center = pos;

	for (auto* w : silkWalls)
	{
		if (!w || !w->IsActive()) continue;

		Vector3 cp;
		if (Collision::CheckHit(w->GetSegment(), testCol, cp))
			return true;
	}
	return false;
}

void Shrinemaiden::move()
{
	const Vector3 now_pos = GetPosition();
	m_Collider.center = now_pos;

	auto* game = Game::GetInstance();
	const auto enemies = game->GetObjects<EnemyBase>();
	const auto silkWalls = game->GetObjects<silkWall>();

	// クールダウン
	if (m_RetreatCooldown > 0) m_RetreatCooldown--;

	//==================================================
	// ★敵と重なったら無敵開始（探索では敵を無視できるようにする）
	// m_Hitpoint--
	//==================================================
	if (!IsMuteki())
	{
		for (auto* e : enemies)
		{
			if (!e || !e->GetIsAlive()) continue;

			Vector3 d = e->GetPosition() - now_pos;
			d.z = 0.0f;

			const float r = (m_Radius + e->GetRadius());
			if (d.LengthSquared() <= r * r)
			{
				m_MutekiTimer = 180;            // ここを調整
				m_IgnoreEnemyInSearch = true;  // 既にHにあるので使う
				m_Texture2D.PlayAnim("getH");  // 受けた演出

				int pick = static_cast<int>(fabsf(e->GetPosition().x)) % 2;
				if (pick == 0)
					Game::GetSound()->Play(SOUND_LABEL_VOICE_ANOTHER_003);
				else
					Game::GetSound()->Play(SOUND_LABEL_VOICE_ANOTHER_004);
				
				--m_Hitpoint;
				break;
			}
		}
	}

	// stuck
	if (TryStuckNudge(now_pos))
		return;

	// A) search escape target
	if (m_EscapeState == EscapeState::SearchEscapePoint)
	{
		if (!SearchEscapeTarget(now_pos, enemies, silkWalls))
			return;

		if (m_EscapeState != EscapeState::MoveToEscapePoint)
			return;
	}

	// B) compute vel
	Vector3 vel;
	if (!UpdateEscapeMove(now_pos, vel))
		return;

	if (vel.x > 0) {
		m_Texture2D.SetFlipX(true);
	}
	else {
		m_Texture2D.SetFlipX(false);
	}

	// C) field -> nextPos
	Vector3 nextPos;
	const bool hit = ApplyField(now_pos, vel, nextPos);

	if (hit)
	{
		SetPosition(nextPos);
		SetVelocity(0.0f);

		m_EscapeState = EscapeState::SearchEscapePoint;
		m_RetreatCooldown = 20;
		return;
	}

	// D) silk at nextPos
	if (CheckSilkCollisionAt(nextPos, silkWalls))
	{
		OnEscapeRouteFailed(now_pos, silkWalls, enemies);
		return;
	}

	// E) apply
	SetPosition(nextPos);
}

//==================================================
// 共通：逃走失敗処理（壁・Silk 共通）
//==================================================

void Shrinemaiden::OnEscapeRouteFailed(
	const Vector3& now_pos,
	const std::vector<silkWall*>& silkWalls,
	const std::vector<EnemyBase*>& enemies)
{
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

	auto IsFreeNoField = [&](const Vector3& candidatePos) -> bool
		{
			// SilkWall（無敵でも壁は無視しない）
			auto testCol = m_Collider;
			testCol.center = candidatePos;

			for (auto* w : silkWalls)
			{
				if (!w || !w->IsActive()) continue;
				Vector3 cp;
				if (Collision::CheckHit(w->GetSegment(), testCol, cp))
					return false;
			}

			// Enemy（★無敵中は避けない）
			if (!(IsMuteki() || m_IgnoreEnemyInSearch))
			{
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
			}

			return true;
		};

	// 3) 退避を試す（Field clamp はここで適用して finalPos で判定）
	for (auto& pCand : candidates)
	{
		Vector3 finalPos = pCand;

		if (m_Field)
		{
			Vector3 p = now_pos;
			Vector3 v = pCand - now_pos;
			m_Field->ResolveBorder(p, v, m_Radius);
			finalPos = p + v;
			finalPos.z = 0.0f;
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
