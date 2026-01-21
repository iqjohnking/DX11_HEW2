#include "Shrinemaiden.h"
#include "Enemy_base.h"
#include "Field.h"
#include "Game.h"
#include "silkWall.h"

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

	// ★追加：線分の中点もチェック（端点が外でも、横切りを拾える）
	Vector3 mid = (s0 + s1) * 0.5f;
	Vector3 dm = mid - a; dm.z = 0.0f;

	// 端点 or 中点が扇形に入るなら unsafe
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

	m_Texture2D.SetSpriteSheet(2, 2);
	m_Texture2D.AddAnimClip("idle", 0, 1, 10);
	m_Texture2D.AddAnimClip("yowa", 2, 3, 10);
	m_Texture2D.PlayAnim("idle");
}

void Shrinemaiden::Update()
{
	move();

	m_Texture2D.Update();
	if (m_velocity < 1.0f)
		m_Texture2D.PlayAnim("yowa");
	else
		m_Texture2D.PlayAnim("idle");

	m_Collider.center = GetPosition();
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
	const auto enemies = Game::GetInstance()->GetObjects<Enemy_base>();

	if (m_IsStuck)
	{
		// 30フレイムことろ待機
		if (m_StuckTimer < 30)   // 約 0.5 秒（60fps）
		{
			m_StuckTimer++;
			SetVelocity(0.0f);
			return;
		}

		// 側に移動し試して
		Vector3 side(-m_LastFailedDir.y, m_LastFailedDir.x, 0.0f);
		if (side.LengthSquared() > 1e-4f)
			side.Normalize();

		const float nudge = m_Radius * 0.3f; // 小さいすぎる
		Vector3 tryPos = GetPosition() + side * nudge;

		// フィールド境界チェック
		if (m_Field)
		{
			Vector3 dummyVel = tryPos - GetPosition();
			Vector3 dummyPos = GetPosition();
			bool hit = m_Field->ResolveBorder(dummyPos, dummyVel, m_Radius);

			if (!hit)
			{
				SetPosition(dummyPos + dummyVel);
			}
		}

		// リセット
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

		if (m_Field)
		{
			const auto& edges = m_Field->GetEdges();

			for (const auto& ed : edges)
			{
				const Vector3 a = now_pos;
				const Vector3 b = ed.p0;
				const Vector3 c = ed.p1;

				float area = Area2D(a, b, c);
				if (area <= 1e-4f)
					continue;

				Vector3 dirB = b - a; dirB.z = 0.0f;
				Vector3 dirC = c - a; dirC.z = 0.0f;

				bool unsafe = false;

				for (auto* e : enemies)
				{
					if (!e || !e->GetIsAlive())
						continue;

					Vector3 dirE = e->GetPosition() - a;
					dirE.z = 0.0f;

					// 方向ベクトル同士の角度チェック
					float c1 = Cross2DXY(dirB, dirE);
					float c2 = Cross2DXY(dirE, dirC);
					const float dirDistSq = dirE.x * dirE.x + dirE.y * dirE.y;
					const float dirLimitSq = m_serchDistance * m_serchDistance;

					if (((c1 >= 0.0f && c2 >= 0.0f) || (c1 <= 0.0f && c2 <= 0.0f)) 
						&& dirDistSq <= dirLimitSq)
					{
						unsafe = true;
						break;
					}


					// 三角形扇形内に敵がいるか？
					// 円 vs 三角形
					if (DoesCircleIntersectTriangleXY(
						e->GetPosition(),
						e->GetRadius() + 50.0f, // BUFFER
						a, b, c))
					{
						unsafe = true;
						break;
					}
				}

				const auto silkWalls = Game::GetInstance()->GetObjects<silkWall>();
				for (auto* w : silkWalls)
				{
					if (!w->IsActive()) continue;

					const auto& seg = w->GetSegment();

					// 與起點距離太遠的 SilkWall 不考慮
					Vector3 mid = (seg.start + seg.end) * 0.5f;
					Vector3 d = mid - a;
					d.z = 0.0f;

					const float silkLimitSq = (m_serchDistance * 0.8f) * (m_serchDistance * 0.8f);

					if (d.LengthSquared() > silkLimitSq)
						continue;

					if (IsSegmentInTriangleFanXY(a, b, c, seg.start, seg.end))
					{
						unsafe = true;
						break;
					}
				}

				if (m_LastFailedDir.LengthSquared() > 0.0f)
				{
					Vector3 dirToCenter = ((a + b + c) / 3.0f) - a;
					dirToCenter.z = 0.0f;

					if (dirToCenter.LengthSquared() > 1e-4f)
						dirToCenter.Normalize();

					// 如果方向太接近上次失敗方向，就排除
					const float dot = dirToCenter.Dot(m_LastFailedDir);
					if (dot > 0.85f) // 約 30 度?
					{
						unsafe = true;
					}
				}

				if (unsafe)
					continue;

				if (m_DrawDebugTris)
					m_DebugTris.push_back({ a, b, c });

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
			// 視為「這個位置本身就不好」
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
		if (toTarget.LengthSquared() <= m_EscapeArriveDist * m_EscapeArriveDist)
		{
			SetVelocity(0.0f);
			m_EscapeState = EscapeState::SearchEscapePoint;
			return;
		}

		toTarget.Normalize();
		SetDirection(toTarget);

		float v = m_velocity + m_acceleration;
		if (v > m_TargetSpeed)
			v = m_TargetSpeed;

		SetVelocity(v);
	}

	//==================================================
	// C) 壁判定（滑りなし）
	//==================================================
	Vector3 vel = m_direction * m_velocity;

	if (m_Field)
	{
		Vector3 dummyPos = now_pos;
		Vector3 dummyVel = vel;

		bool hitBorder = m_Field->ResolveBorder(dummyPos, dummyVel, m_Radius);
		if (hitBorder)
		{
			OnEscapeRouteFailed(now_pos);
			return;
		}
	}

	//==================================================
	// D) SilkWall（壁と同一扱い）
	//==================================================
	const auto silkWalls = Game::GetInstance()->GetObjects<silkWall>();
	for (auto* w : silkWalls)
	{
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

	const float dist = m_Radius * 2.5f;

	// 斜め方向は Normalize() してから使う（Normalized() は無い）
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

	auto IsFree = [&](const Vector3& candidatePos) -> bool
		{
			// Field
			if (m_Field)
			{
				Vector3 p = now_pos;
				Vector3 v = candidatePos - now_pos;
				const bool hit = m_Field->ResolveBorder(p, v, m_Radius);
				if (hit) return false;
			}

			// SilkWall（Sphere 型を使わず、m_Collider と同型で判定）
			const auto silkWalls = Game::GetInstance()->GetObjects<silkWall>();

			auto testCol = m_Collider;     // ← m_Collider と同じ型になる
			testCol.center = candidatePos;

			for (auto* w : silkWalls)
			{
				if (!w || !w->IsActive()) continue;
				Vector3 cp;
				if (Collision::CheckHit(w->GetSegment(), testCol, cp))
					return false;
			}

			return true;
		};

	// 3) 退避を試す（成功したらそこへ移動して再探索）
	for (auto& pCand : candidates)
	{
		Vector3 finalPos = pCand;

		if (m_Field)
		{
			Vector3 p = now_pos;
			Vector3 v = pCand - now_pos;
			const bool hit = m_Field->ResolveBorder(p, v, m_Radius);
			if (hit) continue;
			finalPos = p + v;
		}

		if (IsFree(finalPos))
		{
			SetPosition(finalPos);
			SetVelocity(0.0f);
			m_EscapeState = EscapeState::SearchEscapePoint;
			return;
		}
	}

	// 4) 退路が無い：stuck モードへ
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
	float abLenSq = ab.x * ab.x + ab.y * ab.y;

	if (abLenSq <= 1e-6f)
	{
		Vector3 d = p - a;
		return d.x * d.x + d.y * d.y;
	}

	Vector3 ap = p - a;
	float t = (ap.x * ab.x + ap.y * ab.y) / abLenSq;
	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 closest = a + ab * t;
	Vector3 d = p - closest;
	return d.x * d.x + d.y * d.y;
}

bool Shrinemaiden::DoesCircleIntersectTriangleXY(
	const Vector3& center,
	float radius,
	const Vector3& a,
	const Vector3& b,
	const Vector3& c)
{
	float r2 = radius * radius;

	if (IsPointInTriangleXY(center, a, b, c))
		return true;

	if (DistPointToSegmentSqXY(center, a, b) <= r2) return true;
	if (DistPointToSegmentSqXY(center, b, c) <= r2) return true;
	if (DistPointToSegmentSqXY(center, c, a) <= r2) return true;

	return false;
}
