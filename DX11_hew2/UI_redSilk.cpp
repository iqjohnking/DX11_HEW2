#include "UI_redSilk.h"

using namespace std;
using namespace DirectX::SimpleMath;

void UI_redSilk::Init()
{
	m_StartPos = Vector3::Zero;
	m_EndPos = Vector3::Zero;

	m_Texture2D.Texture2D::Init();
	//m_Texture2D.SetRepeatTexture(m_RepeatTexture::xOnly);
	m_Texture2D.SetTexture("assets/texture/spiderSilknerau.png");
	m_Texture2D.SetPosition(Vector3::Zero);
	m_Texture2D.SetRotation(Vector3::Zero);
	m_Texture2D.SetScale(0.0f, 15.f, 0.0f);

	SetDrawOrder(2);
}

void UI_redSilk::Update()
{
	if (!m_LeftHand || !m_RightHand)
	{
		return;
	}

	// 左右手の位置を取得
	m_StartPos = m_LeftHand->GetPosition();
	m_EndPos = m_RightHand->GetPosition();

	// 中点を計算してそこに配置
	Vector3 dir = m_EndPos - m_StartPos;
	float   len = dir.Length();
	if (len <= 0.0001f)
	{
		// 手が同じ位置にあるならスケールを 0 にして非表示的にする
		m_Texture2D.SetScale(0.0f, 15.0f, 1.0f);
		return;
	}

	Vector3 mid = (m_StartPos + m_EndPos) * 0.5f;
	Vector3 dirN = dir / len; // 正規化方向

	// ----------------------------------------
	// 2D UI として使う場合 (XY 平面上で回す想定)
	// ----------------------------------------
	// XY 平面上での角度（ラジアン）
	float angleZ = atan2f(dirN.y, dirN.x);  // X 軸からの角度
	// Texture2D::SetRotation は度指定なので度に変換
	//float angleDeg = angleZ * 180.0f / PI;

	// 位置と回転・スケールを更新
	m_Texture2D.SetPosition(mid);
	m_Texture2D.SetRotationRad(0.f, 0.f, angleZ);
	// X 方向スケールを「長さ」として使う
	m_Texture2D.SetScale(len, 15.0f, 0.0f);

	// アニメーションなど Texture2D 側の更新があれば
	//m_Texture2D.Update();


}

void UI_redSilk::Draw(Camera* cam)
{
	m_Texture2D.Draw(cam);
}

void UI_redSilk::Uninit()
{
	m_Texture2D.Uninit();
}
