#include "silkWall.h"

using namespace DirectX::SimpleMath;

void silkWall::Init()
{
	m_Hitpoint = 3;
	m_IsGrowing = false;
	m_PoiseLevel = 0;
	m_ExpandSpeed = 50.0f;
	//m_TargetLength = 0.0f; // fire()で設定されるので初期化不要

	SetPosition(Vector3(-500000.0f, -500000.0f, 0.0f));// 初期位置（画面外）

	m_Scale = DirectX::SimpleMath::Vector3(0.0f, 15.0f, .0f);

	// 親クラス的初始化
	m_Texture2D.Texture2D::Init();
	// 重複テクスチャを有効に（レンダラー側でもサンプラがWRAPなら有効）
	m_Texture2D.SetRepeatTexture(m_RepeatTexture::xOnly);
	m_Texture2D.SetTexture("assets/texture/spiderSilkKari.png");
	m_Texture2D.SetPosition(m_Position);
	m_Texture2D.SetRotation(Vector3(0.0f, 0.0f, 180.0f));
	m_Texture2D.SetScale(0.0f, 15.f, 0.0f);

	// ピボットを右端に設定（モデル空間で頂点が -0.5..0.5 の想定）
	// あとは伸びる方向に合わせて回転させればOK(fire()のときを回転させる)
	m_Texture2D.SetPivot(0.5f, 0.0f, 0.0f);

	// collider 初期化
	m_Segment.start = m_StartPos;
	m_Segment.end = m_StartPos;

	SetDrawOrder(3);
}

void silkWall::Update()
{
	if (isActive == false)
	{
		return;
	}
	if (hitStopTimer > 0)
	{
		hitStopTimer--;
		return;
	}


	if(m_PoiseTimer>0)
		m_PoiseTimer--;
	else
		m_PoiseLevel = 0;


	Vector3 ScaleNow = m_Scale;

	// 目標位置までの長さを超えたら止める
	if (ScaleNow.x >= m_TargetLength)
	{
		ScaleNow.x = m_TargetLength; // 今の長さを確認
		m_IsGrowing = false;       // 停止
	}
	else {
		ScaleNow.x += m_ExpandSpeed; // ｘ方向に伸ばす
	}

	Vector3 newScale = Vector3(ScaleNow.x, GetScale().y, 0.0f);
	SetScale(newScale); // 親クラスのスケールも更新しておく

	UpdateCollider();
}

void silkWall::Draw(Camera* cam)
{
	if (!isActive) return;

	m_Texture2D.SetPosition(GetPosition()); // 親クラスの位置を反映
	m_Texture2D.SetScale(GetScale());	// 親クラスの大きさを反映
	m_Texture2D.SetRotation(GetRotation()); // 親クラスの回転を反映

	if(m_PoiseLevel==0)
	{
		// 白
		m_Texture2D.SetMulColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (m_PoiseLevel == 1)
	{
		// 淺紫
		m_Texture2D.SetMulColor(DirectX::SimpleMath::Color(0.85f, 0.70f, 1.00f, 1.0f));
	}
	else if (m_PoiseLevel == 2)
	{
		// 中紫
		m_Texture2D.SetMulColor(DirectX::SimpleMath::Color(0.60f, 0.25f, 0.85f, 1.0f));
	}
	else if (m_PoiseLevel >= 3)
	{
		// 深紫
		m_Texture2D.SetMulColor(DirectX::SimpleMath::Color(0.35f, 0.05f, 0.55f, 1.0f));
	}

	m_Texture2D.Draw(cam);
}

void silkWall::Uninit()
{
	m_Texture2D.Uninit();
}

void silkWall::Fire(const Vector3& startPos, const Vector3& targetPos)
{
	isActive = true;
	m_IsGrowing = true;

	// 位置を設定（右手/左手の座標を渡す）
	SetStartPos(startPos);
	SetEndPos(targetPos);

	SetPosition(m_StartPos); // 親クラスの位置も更新

	// 目標までの距離を最大長さにしておく
	Vector3 dir = m_EndPos - m_StartPos;
	m_TargetLength = dir.Length();   // ← 長さ

	// 方向を向く 
	float angleRad = atan2f(dir.y, dir.x);		// 弧
	Vector3 newRot = Vector3(0.0f, 0.0f, angleRad * (180.0f / PI) + 180.0f); // 角度に変換してデグリーに
	SetRotation(newRot);	// ラジアンを入れて回転させる

	// 長さリセット Scaleリセット
	Vector3 resetScale = Vector3(0.0f, GetScale().y, GetScale().z);
	SetScale(resetScale);

	UpdateCollider();
}

void silkWall::reInit()
{
	// 場に影響しない状態へ
	isActive = false;      // UpdateCollider() が長さ0にする
	m_IsGrowing = false;     // 伸張停止
	m_TargetLength = 0.0f;   // 任意: 目標長さもゼロに戻す（次回Fire時に再設定）

	// 表示スケールのX（長さ）をゼロへ。Y/Zは太さ維持ならそのままでもOK
	m_Scale.x = 0.0f;

	// 親クラス側のTransformにも反映（描画の整合）
	SetScale(Vector3(0.0f, GetScale().y, GetScale().z));
	// 端点は保持する（m_StartPos/m_EndPos は削除しない）

	// コライダ更新（isActive=false なので start=end になる）
	UpdateCollider();
}

//private:
////////////////////////////////////////////////////////
void silkWall::UpdateCollider()
{
	// 非アクティブのときは「長さ 0 の線分」にしておく
	if (isActive == false)
	{
		m_Segment.start = m_StartPos;
		m_Segment.end	= m_StartPos;
		return;
	}

	// start → target の方向ベクトル
	Vector3 dir = m_EndPos - m_StartPos;
	if (dir.LengthSquared() <= 0.000001f)
	{
		// 方向が取れないときも長さ 0 にしておく
		m_Segment.start = m_StartPos;
		m_Segment.end	= m_StartPos;
		return;
	}
	dir.Normalize();

	// 現在の表示上の長さ（Scale.x をそのまま使う）
	float curLength = m_Scale.x;
	if (curLength < 0.0f)  curLength = -curLength;
	if (curLength > m_TargetLength) curLength = m_TargetLength;

	// ------------------------------
	// 判定用に両端を延長する
	// ------------------------------
	// ゲームスケールに合わせて調整（例: 20?40）
	const float extendLen = 100.0f;

	// 実際の線分の有効長さ（スタート?ターゲット）
	const float maxLength = m_TargetLength;

	// 片側 extendLen ずつ延長したいので、両端で合計 2*extendLen 長くなる。
	// ただし元の start から target を超えないようにクランプ。
	float extra = extendLen;

	// 判定用の start/end を計算
	// start を「後ろ側」に、end を「前側」に延ばす
	Vector3 collStart = m_StartPos - dir * extra;
	Vector3 collEnd = m_StartPos + dir * (curLength + extra);

	// コライダ更新
	m_Segment.start = collStart;
	m_Segment.end	= collEnd;
}
