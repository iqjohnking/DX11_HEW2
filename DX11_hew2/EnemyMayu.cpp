#include "EnemyMayu.h"
#include "Shrinemaiden.h"
#include "Field.h"
#include "Game.h"
#include "silkWall.h"

using namespace std;
using namespace DirectX::SimpleMath;

void EnemyMayu::Init()
{
	SetMinSpeed(0.0f);		// 移動しない
	SetTargetSpeed(0.0f);   // 移動しない
	SetMaxSpeed(0.0f);      // 移動しない
	SetAcceleration(0.00f); // 移動しない
	SetVelocity(0.0f);      // 移動しない
	SetDirection(Vector3(1.0f, 0.0f, 0.0f)); // 初期向き（何でもOK）
	SetIsAlive(true);
	SetRadius(25.0f);

	//初期化処理
	m_Texture2D.Texture2D::Init();

	m_Texture2D.SetTexture("assets/texture/mayu.png");
	//SetPosition(100.0f, 100.0f, 0.0f);	// 初期位置は外部で設定する想定
	m_Texture2D.SetRotation(m_Rotation);	// 初期値0,0,0//外で変える
	m_Texture2D.SetScale(m_Radius * 4, m_Radius * 4, 0); //初期値0,0,0//外で変える
	m_Collider.center = GetPosition();		// 初期値0,0,0//外で変える
	m_Collider.radius = GetRadius();		// 初期値0,0,0//外で変える
}

void EnemyMayu::Update()
{
	// IsActive が true のときだけ動作 
	// IsActive = false の場合はいないけど
	if (isActive)
	{
		m_Collider.center = GetPosition();
		m_Collider.radius = GetRadius();
	}
}

void EnemyMayu::Draw(Camera* cam)
{
	m_Texture2D.SetPosition(GetPosition());
	//m_Texture2D.SetScale(m_Radius * 4, m_Radius * 4, 0);
	m_Texture2D.Draw(cam);
}

void EnemyMayu::Uninit()
{
	m_Texture2D.Uninit();
}

void Enemy1::move()
{
	
}