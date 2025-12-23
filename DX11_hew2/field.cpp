#include "Field.h"
#include "DeBugLine2D.h"


void Field::Init()
{
	m_Position = Vector3(m_Center);					// 画面中心を設定
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);					// 回転なし
	m_Scale = Vector3(m_Radius * 2, m_Radius * 2, 0.0f);	// スケールはm_Radiusの2倍
	
	
	// テクスチャ設定
	m_Texture2D.Texture2D::Init();// 親クラス的初始化
	//SetRepeatTexture(m_RepeatTexture::m_false);// 重複テクスチャを設定
	m_Texture2D.SetTexture("assets/texture/field.png");
	m_Texture2D.SetPosition(m_Center);
	m_Texture2D.SetRotation(0.0f, 0.0f, 0.0f);
	m_Texture2D.SetScale(m_Radius*2, m_Radius*2, 0.0f);

	// 16 辺の円形場地境界を生成
	BuildBorder(m_Center, m_Radius, 16);
}

void Field::Update()
{
	m_Texture2D.Texture2D::Update();
}

void Field::Draw(Camera* cam)
{
	m_Texture2D.Texture2D::Draw(cam);
	DrawDebug(cam);
}

void Field::DrawDebug(Camera* cam)
{
	static DeBugLine2D line;
	static bool initialized = false;
	if (!initialized)
	{
		line.Texture2D::Init();
		line.Init();
		line.SetTexture("assets/texture/ui_back.png"); // 準備好的 1x1 白貼圖
		initialized = true;
	}

	for (const auto& ed : m_Edges)
	{
		line.SetLine(ed.p0, ed.p1);
		line.Draw(cam);
	}
}

void Field::Uninit()
{
	m_Texture2D.Uninit();
	m_Edges.clear();
}

//float Field::Dot(const Vector3& v1, const Vector3& v2)
//{
//	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
//}

// 円形場地を N 辺ポリゴンとして構築する
void Field::BuildBorder(const Vector3& center, float radius, int edges)
{
	m_Center = center;
	m_Radius = radius;
	m_EdgeCount = edges;

	m_Edges.clear();
	m_Edges.resize(m_EdgeCount);

	const float step = TWO_PI / (float)m_EdgeCount;

	for (int i = 0; i < m_EdgeCount; ++i)
	{
		float a0 = step * i;
		float a1 = step * (i + 1);

		// XY 平面上に円を描く（Z = 0）
		Vector3 p0 = center + Vector3(cosf(a0) * radius,
			sinf(a0) * radius,
			0.0f);
		Vector3 p1 = center + Vector3(cosf(a1) * radius,
			sinf(a1) * radius,
			0.0f);

		// 辺の方向ベクトル
		Vector3 e = p1 - p0;

		// 辺に垂直なベクトル（候補法線）
		Vector3 n = Vector3(e.y, -e.x, 0.0f);

		// 法線が「場地の内側」を向くように補正する
		// 中心点が法線の正方向にあるかをチェック
		//if (Dot(n, center - p0) < 0.0f)
		//{
		//	n = -n;
		//}

		if (n.Dot(center - p0) < 0.0f)   // もしくは v.Dot(n)
		{
			n = -n;
		}

		n.Normalize();

		m_Edges[i] = { p0, p1, n };
	}
}
// 場地境界との衝突解決（押し戻し＋反射）
bool Field::ResolveBorder(Vector3& pos, Vector3& vel, float objRadius) const
{
	bool hit = false;

	// 凸多角形の各辺に対してチェック
	for (const auto& ed : m_Edges)
	{
		// 有効距離：
		// pos が辺から「法線方向にどれだけ内側にあるか」
		//float dist = Dot(pos - ed.p0, ed.n);
		Vector3 toPos = pos - ed.p0;	// 辺からのベクトル
		float dist = toPos.Dot(ed.n);	// pos が辺から「法線方向にどれだけ内側にあるか」

		// 半径より内側に入り込んでいたら衝突
		if (dist < objRadius)
		{
			hit = true;

			// ① 位置補正
			//   オブジェクトが場地外に出ないよう、内側へ押し戻す
			float push = (objRadius - dist);
			pos += ed.n * push;

			// ② 速度反射
			//   v' = v - 2 * (v・n) * n
			float vn = vel.Dot(ed.n);
			vel = vel - ed.n * (2.0f * vn);
		}
	}

	return hit;
}