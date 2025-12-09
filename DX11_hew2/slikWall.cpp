#include "silkWall.h"

using namespace DirectX::SimpleMath;

void silkWall::Init()
{
	// 親クラス的初始化
	Texture2D::Init();

	// 重複テクスチャを有効に（レンダラー側でもサンプラがWRAPなら有効）
	SetRepeatTexture(m_RepeatTexture::xOnly);

	// テクスチャ設定
	SetTexture("assets/texture/spiderSilkKari.png");
	// 初期位置設定
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 180.0f);
	SetScale(0.0f, 15.f, 0.0f);

	// ピボットを右端に設定（モデル空間で頂点が -0.5..0.5 の想定）
	// あとは伸びる方向に合わせて回転させればOK(fire()のときを回転させる)
	SetPivot(0.5f, 0.0f, 0.0f);

	// collider 初期化
	m_Segment.start = m_StartPos;
	m_Segment.end = m_StartPos;

}

void silkWall::Update()
{
	if (m_IsActive == false)
	{
		return;
	}

	Vector3 ScaleNow = m_Scale;

	// 目標位置までの長さを超えたら止める
	if (ScaleNow.x >= m_MaxLength)
	{
		ScaleNow.x = m_MaxLength; // 今の長さを確認
		m_IsActive = false;       // 停止
	}
	else {
		ScaleNow.x += m_ExpandSpeed; // ｘ方向に伸ばす
		SetScale(ScaleNow.x, GetScale().y, 0.0f);
	}

	UpdateCollider();
}

////todo : obbになるか？sphereになる?
bool silkWall::CheckHit(const Collision::Sphere& sphere) const
{
	if (!m_IsActive)
	{
		return false;
	}
	
	// bool CheckHit(const Segment& segment, const Sphere& sphere);
	return Collision::CheckHit(m_Segment, sphere);
}
void silkWall::Fire(const Vector3& startPos, const Vector3& targetPos)
{
	// 位置を設定（右手/左手の座標を渡す）
	SetStartPos(startPos);
	SetTargetPos(targetPos);

	SetPosition(m_StartPos);

	// 目標までの距離を最大長さにしておく
	Vector3 dir = m_TargetPos - m_StartPos;
	m_MaxLength = dir.Length();   // ← 長さ

	// 方向を向く
	float angleRad = atan2f(dir.y, dir.x);		// 弧
	SetRotationRad(0.0f, 0.0f, angleRad + PI);	// ラジアンを入れて回転させる

	// 長さリセット
	SetScale(0.0f, GetScale().y, GetScale().z);

	m_IsActive = true;

	UpdateCollider();
}

//private:
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void silkWall::UpdateCollider()
{
	// 非アクティブのときは「長さ 0 の線分」にしておく
	if (m_IsActive == false)
	{
		m_Segment.start = m_StartPos;
		m_Segment.end	= m_StartPos;
		return;
	}

	// start → target の方向ベクトル
	Vector3 dir = m_TargetPos - m_StartPos;
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
	if (curLength > m_MaxLength) curLength = m_MaxLength;

	// 牆的線分：從手的位置開始，沿著 dir 延伸 curLength
	m_Segment.start = m_StartPos;
	m_Segment.end	= m_StartPos + dir * curLength;
}
