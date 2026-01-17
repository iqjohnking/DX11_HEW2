#include "TitleScene.h"


using namespace DirectX::SimpleMath;

// 最大三つがある
//silkWall* m_SilkWalls[3] = { nullptr, nullptr, nullptr };

// コンストラクタ
TitleScene::TitleScene()
{
	Init();
}

// デストラクタ
TitleScene::~TitleScene()
{
	Uninit();
}

// 初期化
void TitleScene::Init()
{
	// 背景
	auto* bg  = Game::GetInstance()->AddObject<TitleBG>();
	m_MySceneObjects.emplace_back(bg);
	bg->Texture2D::SetPosition(0.0f, 0.0f, 2.0f); // Z順序を最後に

	// FIELD
	m_Field = Game::GetInstance()->AddObject<Field>();
	m_MySceneObjects.emplace_back(m_Field);

	// silkWall*3
	for (int i = 0; i < 3; ++i)
	{
		m_SilkWalls[i] = Game::GetInstance()->AddObject<silkWall>();
		m_MySceneObjects.emplace_back(m_SilkWalls[i]);
	}


	// 左手（W / S ）
	m_HandL = Game::GetInstance()->AddObject<playerHand>(0);
	m_MySceneObjects.emplace_back(m_HandL);

	// 右手（↑ / ↓ ）
	m_HandR = Game::GetInstance()->AddObject<playerHand>(1);
	m_MySceneObjects.emplace_back(m_HandR);

	m_HandL->SetAnotherHand(m_HandR);
	//m_HandL->SetField(m_Field);
	m_HandR->SetAnotherHand(m_HandL);
	//m_HandR->SetField(m_Field);

	////巫女
	m_Miko = Game::GetInstance()->AddObject<Shrinemaiden>();
	m_MySceneObjects.emplace_back(m_Miko);
	m_Miko->SetField(m_Field);

	////敵1
	for (int i = 0; i < 10; ++i)
	{
		Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
		Enemy1List[i]->SetTarget(m_Miko);
		Enemy1List[i]->SetField(m_Field);
		if (i < 5) {

			Enemy1List[i]->SetPosition({ -350.f ,  200.f - 50.0f * (i + 1) , 0.0f });
		}
		else {
			Enemy1List[i]->SetPosition({ 350.f ,  200.f - 50.0f * (i - 4) , 0.0f });
			Enemy1List[i]->SetRadius(Enemy1List[i]->GetRadius()+5.f);
		}
		m_MySceneObjects.emplace_back(Enemy1List[i]);
	}
}

// 更新
void TitleScene::Update()
{
	//-----------------------------------------------------------------------------
	// 操作／INPUT
	//-----------------------------------------------------------------------------

	if (Input::GetKeyTrigger('D') || Input::GetButtonTrigger(XINPUT_LEFT_SHOULDER))   // 
	{
		silkWall* w = m_SilkWalls[m_NextSilkIndex];
		if (w && m_HandL && m_HandR)
		{
			Vector3 startPos = m_HandL->GetPosition();  // 左手
			Vector3 targetPos = m_HandR->GetPosition(); // 右手
			w->Fire(startPos, targetPos);

			// 用掉一條絲 → 往下一個 index
			++m_NextSilkIndex;
			if (m_NextSilkIndex >= 3)
			{
				m_NextSilkIndex = 0;
			}
		}
	}

	// 
	if (Input::GetKeyTrigger('J') || Input::GetKeyTrigger(VK_LEFT) || Input::GetButtonTrigger(XINPUT_RIGHT_SHOULDER))
	{
		silkWall* w = m_SilkWalls[m_NextSilkIndex];
		if (w && m_HandL && m_HandR)
		{
			Vector3 startPos = m_HandR->GetPosition();  // 右手
			Vector3 targetPos = m_HandL->GetPosition(); // 左手
			w->Fire(startPos, targetPos);

			++m_NextSilkIndex;
			if (m_NextSilkIndex >= 3)
			{
				m_NextSilkIndex = 0;
			}
		}
	}

	if (Input::GetKeyTrigger('R'))   // 
	{
		std::vector<Object*> removeList;

		//Enemyを探す
		for (auto* obj : m_MySceneObjects)
		{
			if (dynamic_cast<Enemy_base*>(obj))
			{
				removeList.push_back(obj);
			}
		}

		//　見つけたEnemy1を削除する
		for (auto* obj : removeList)
		{
			Game::GetInstance()->DeleteObject(obj);

			auto it = std::find(m_MySceneObjects.begin(), m_MySceneObjects.end(), obj);
			if (it != m_MySceneObjects.end())
			{
				m_MySceneObjects.erase(it);
			}
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		silkWall* wall = m_SilkWalls[i];
		if (!wall) continue;              // 防禦性チェック
	}

	//-----------------------------------------------------------------------------
	// silkWallの三角形判定
	//-----------------------------------------------------------------------------
	Vector3 A, B, C; // 三角形頂点

	silkWall* walls[3] = { m_SilkWalls[0], m_SilkWalls[1], m_SilkWalls[2] };

	// 3 本とも準備完了しているか？
	const bool allReady = std::all_of(std::begin(walls), std::end(walls),
		[](const silkWall* w) { return w && !w->IsGrowing(); });


	// nullptr チェックのみ（就緒判定は行わない）
	if (allReady)
	{
		// 3 本の silkWall から三角形生成を試行
		// false の場合は三角形が構成できなかった（平行／交差なし／面積ゼロなど）
		if (TriangleSilk::TryMakeTriangleFromWallsXY(walls[0], walls[1], walls[2], A, B, C))
		{
			int eliminatedCount = 0;
			std::vector<Object*> toRemove; // 倒す予定リスト

			// 敵を調べて、三角形内にいるやつを倒す予定リストに格納する
			for (auto* obj : m_MySceneObjects)
			{
				auto* enemy = dynamic_cast<Enemy_base*>(obj);
				if (!enemy) continue;

				const auto pos = enemy->GetPosition();
				if (TriangleSilk::IsInsideTriangleXY(pos, A, B, C))
				{
					toRemove.push_back(obj);
					++eliminatedCount;
				}
			}

			// 倒す予定の敵を削除する
			//アニメーションの追加する必要あるか？
			for (auto* obj : toRemove)
			{
				Game::GetInstance()->DeleteObject(obj);
				auto it = std::find(m_MySceneObjects.begin(), m_MySceneObjects.end(), obj);
				if (it != m_MySceneObjects.end())
				{
					m_MySceneObjects.erase(it);
				}
			}

			if (eliminatedCount > 0)
			{
				// 半径の設計例：基準 25 + 1体ごとに +5
				const float baseRadius = 25.0f;
				const float perKill = 5.0f;
				const float mayuRadius = baseRadius + perKill * static_cast<float>(eliminatedCount);

				auto* mayu = Game::GetInstance()->AddObject<EnemyMayu>();
				// 生成位置は三角形の重心に配置（必要なら別ロジックに変更）
				Vector3 centroid = (A + B + C) / 3.0f;
				mayu->SetPosition(centroid);
				mayu->SetRadius(mayuRadius); // 半径設定（後述の連動対応が必要）
				m_MySceneObjects.emplace_back(mayu);
			}

			for (int i = 0; i < 3; ++i)
			{
				m_SilkWalls[i]->reInit();
			}

		}
	}


	//-----------------------------------------------------------------------------
	// silkWall と　mayu の当たり判定
	//-----------------------------------------------------------------------------

	//MAYUのなかにいるから、こちはなにもしない

	for (auto it = m_MySceneObjects.begin(); it != m_MySceneObjects.end(); )
	{
		Object* o = *it; // オブジェクト取得
		if (!o || o->ToBeDeleted())
		{
			Game::GetInstance()->DeleteObject(o); // オブジェクト削除
			it = m_MySceneObjects.erase(it); // イテレータを更新
		}
		else
		{
			++it; // 次へ
		}
	}

	//-----------------------------------------------------------------------------
	// エンターキーを押してステージ1へ
	//-----------------------------------------------------------------------------
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Game::GetInstance()->ChangeScene(STAGE1);

		return;
	}
}

// 終了処理
void TitleScene::Uninit()
{
	// このシーンのオブジェクトを削除する
	// 
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}
