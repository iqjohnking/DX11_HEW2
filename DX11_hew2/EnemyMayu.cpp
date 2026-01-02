#include "EnemyMayu.h"
#include "Shrinemaiden.h"
#include "Field.h"
#include "Game.h"
#include "silkWall.h"

using namespace std;
using namespace DirectX::SimpleMath;

void EnemyMayu::Init()
{
	//初期化処理
	m_Texture2D.Texture2D::Init();

	m_Texture2D.SetTexture("assets/texture/mayu.png");
	//SetPosition(100.0f, 100.0f, 0.0f);	// 初期位置は外部で設定する想定
	m_Texture2D.SetRotation(m_Rotation);	// 初期値0,0,0//外で変える
	m_Texture2D.SetScale(m_Radius * 4, m_Radius * 4, 0); //初期値0,0,0//外で変える
	m_Collider.center = GetPosition();		// 初期値0,0,0//外で変える
	m_Collider.radius = GetRadius();		// 初期値0,0,0//外で変える
	SetDrawOrder(2);
}

void EnemyMayu::Update()
{
	// IsActive が true のときだけ動作 
	// IsActive = false の場合はいないけど
	if (isActive)
	{
		//当たり判定更新
		m_Collider.center = GetPosition();
		m_Collider.radius = GetRadius();
	}
}

void EnemyMayu::Draw(Camera* cam)
{
	//Vector3 drawPos = Vector3 (m_Position.x, m_Position.y, m_Position.z);
	m_Texture2D.SetPosition(m_Position);
	m_Texture2D.SetScale(m_Radius * 4, m_Radius * 4, 0);
	m_Texture2D.Draw(cam);
}

void EnemyMayu::Uninit()
{
	m_Texture2D.Uninit();
}
