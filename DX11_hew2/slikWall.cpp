#include "silkWall.h"

using namespace DirectX::SimpleMath;

void silkWall::Init()
{
	m_Hitpoint = 3;
	m_IsGrowing = false;
	m_IsPoised = false;
	m_ExpandSpeed = 50.0f;
	//m_TargetLength = 0.0f; // fire()で設定されるので初期化不要

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

}

void silkWall::Update()
{
	if (isActive == false)
	{
		return;
	}

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
	m_Texture2D.SetPosition(GetPosition()); // 親クラスの位置を反映
	m_Texture2D.SetScale(GetScale());	// 親クラスの大きさを反映
	m_Texture2D.SetRotation(GetRotation()); // 親クラスの回転を反映
	m_Texture2D.Draw(cam);
}

void silkWall::Uninit()
{
	m_Texture2D.Uninit();
}

////todo : obbになるか？sphereになる?
bool silkWall::CheckHit(const Collision::Sphere& sphere) const
{
	if (!isActive)
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

	isActive = true;
	m_IsGrowing = true;

	UpdateCollider();
}

//private:
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
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

	// 牆的線分：從手的位置開始，沿著 dir 延伸 curLength
	m_Segment.start = m_StartPos;
	m_Segment.end	= m_StartPos + dir * curLength;
}
