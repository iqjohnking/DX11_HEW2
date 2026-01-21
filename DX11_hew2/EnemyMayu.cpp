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

	//m_Texture2D.SetTexture("assets/texture/mayu.png");
	m_Texture2D.SetTexture("assets/texture/mayuanime.png");
	//SetPosition(100.0f, 100.0f, 0.0f);	// 初期位置は外部で設定する想定
	m_Texture2D.SetRotation(m_Rotation);	// 初期値0,0,0//外で変える
	m_Texture2D.SetScale(m_Radius * 4, m_Radius * 4, 0); //初期値25*4  , 25*4  ,0//外で変える
	m_Collider.center = GetPosition();		// 初期値0,0,0//外で変える
	m_Collider.radius = GetRadius();		// 初期値25,25,0//外で変える

	//animation 設定
	m_Texture2D.SetSpriteSheet(3, 3);
	m_Texture2D.AddAnimClip("idle", 0, 0, 10);
	m_Texture2D.AddAnimClip("yowa", 1, 8, 8);

	m_Texture2D.PlayAnim("idle");



	SetDrawOrder(2);
}

void EnemyMayu::Update()
{
	// IsActive が true のときだけ動作 
	// IsActive = false の場合はいないけど

	if (spawnTimer < 60)
	{
		m_Collider.radius = 0; //出現中は当たり判定なし
		spawnTimer++;
		return;
	}

	if (!isActive)
	{
		return;
	}

	// 1)絹の糸当たり判定
	vector<silkWall*> silkWalls = Game::GetInstance()->GetObjects<silkWall>();
	for (auto w : silkWalls)
	{
		if (Collision::CheckHit(w->GetSegment(), m_Collider))
		{
			m_Texture2D.PlayAnim("yowa");
			if (!isExploding)
			{
				SetRadius(m_Radius * 2);
			}
			isExploding = true;
		}
	}


	// 2)消滅判定
	if (explodeTimer > 60) {
		toBeDeleted = true; //消滅
	}

	// 3)当たり判定更新
	m_Collider.center = GetPosition(); //当たり判定位置更新
	m_Collider.radius = GetRadius(); //当たり判定半径更新


	// 4)enemysとの衝突判定処理
	if (isExploding) {
		explodeTimer++;

		auto enemys = Game::GetInstance()->GetObjects<EnemyBase>();
		for (auto* e : enemys)
		{
			if (!e) continue;

			// すでに減速中ならスキップ（衝突判定もしない）
			if (e->GetIsSpdDown()) continue;

			if (Collision::CheckHit(e->GetCollider(), m_Collider))
			{
				e->SetIsSpdDown(true);
			}
		}
	}

	// 5)アニメーション更新
	m_Texture2D.Update();
}

void EnemyMayu::Draw(Camera* cam)
{
	if (spawnTimer < 60)
	{
		spawnTimer++;
		m_Texture2D.SetScale(0, 0, 0);
		return;
	}

	//Vector3 drawPos = Vector3 (m_Position.x, m_Position.y, m_Position.z);
	m_Texture2D.SetPosition(m_Position);
	m_Texture2D.SetScale(m_Radius * 4, m_Radius * 4, 0);
	m_Texture2D.Draw(cam);
}

void EnemyMayu::Uninit()
{
	m_Texture2D.Uninit();
}
