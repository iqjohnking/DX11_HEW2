#include "TitleScene.h"

using namespace DirectX::SimpleMath;

// 最大三つがある
//silkWall* m_SilkWalls[3] = { nullptr, nullptr, nullptr };

// コンストラクタ
TitleScene::TitleScene()
{

}

// デストラクタ
TitleScene::~TitleScene()
{

}

// 初期化
void TitleScene::Init()
{
	// 背景
	auto* bg = Game::GetInstance()->AddObject<TitleBG>();
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

	// hashira
	for (int i = 0; i < 1; ++i)
	{
		hashiras[i] = Game::GetInstance()->AddObject<Hashira>();
		hashiras[i]->SetPosition({ 0.f , 0.f , 0.0f });
		m_MySceneObjects.emplace_back(hashiras[i]);
	}

	// UI用の赤い糸表示
	m_UI_redSilk = Game::GetInstance()->AddObject<UI_redSilk>();
	m_MySceneObjects.emplace_back(m_UI_redSilk);
	m_UI_redSilk->SetHands(m_HandL, m_HandR);

	// UI miko hp
	m_UI_mikoHp = Game::GetInstance()->AddObject<UI_mikoHp>();
	m_MySceneObjects.emplace_back(m_UI_mikoHp);
	m_UI_mikoHp->SetMiko(m_Miko);

	// UI 用の操作説明表示
	m_UI_control = Game::GetInstance()->AddObject<UI_control>();
	m_MySceneObjects.emplace_back(m_UI_control);

	//敵1
	//for (int i = 0; i < 10; ++i)
	//{
	//	Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
	//	Enemy1List[i]->SetTarget(m_Miko);
	//	Enemy1List[i]->SetField(m_Field);
	//	if (i < 5) {
	//
	//		Enemy1List[i]->SetPosition({ -350.f ,  200.f - 50.0f * (i + 1) , 0.0f });
	//	}
	//	else {
	//		Enemy1List[i]->SetPosition({ 350.f ,  200.f - 50.0f * (i - 4) , 0.0f });
	//		Enemy1List[i]->SetRadius(Enemy1List[i]->GetRadius() + 5.f);
	//	}
	//	m_MySceneObjects.emplace_back(Enemy1List[i]);
	//}


	//for (int i = 0; i < 10; ++i)
	//{
	//	Enemy2List[i] = Game::GetInstance()->AddObject<Enemy2>();
	//	Enemy2List[i]->SetTarget(m_Miko);
	//	Enemy2List[i]->SetField(m_Field);
	//	if (i < 5) {
	//
	//		Enemy2List[i]->SetPosition({ -400.f ,  200.f - 50.0f * (i + 1) , 0.0f });
	//	}
	//	else {
	//		Enemy2List[i]->SetPosition({ 400.f ,  200.f - 50.0f * (i - 4) , 0.0f });
	//		Enemy2List[i]->SetRadius(Enemy2List[i]->GetRadius() + 5.f);
	//	}
	//	m_MySceneObjects.emplace_back(Enemy2List[i]);
	//}

	//for (int i = 0; i < 10; ++i)
	//{
	//	Enemy3List[i] = Game::GetInstance()->AddObject<Enemy3>();
	//	Enemy3List[i]->SetTarget(m_Miko);
	//	Enemy3List[i]->SetField(m_Field);
	//	if (i < 5) {
	//
	//		Enemy3List[i]->SetPosition({ -400.f ,  200.f - 50.0f * (i + 1) , 0.0f });
	//	}
	//	else {
	//		Enemy3List[i]->SetPosition({ 400.f ,  200.f - 50.0f * (i - 4) , 0.0f });
	//		Enemy3List[i]->SetRadius(Enemy3List[i]->GetRadius() + 5.f);
	//	}
	//	m_MySceneObjects.emplace_back(Enemy3List[i]);
	//}
	//

	//敵4
	for (int i = 0; i < 10; ++i)
	{
		Enemy4List[i] = Game::GetInstance()->AddObject<Enemy4>();
		Enemy4List[i]->SetTarget(m_Miko);
		Enemy4List[i]->SetField(m_Field);
		if (i < 5) {
	
			Enemy4List[i]->SetPosition({ -300.f ,  200.f - 50.0f * (i + 1) , 0.0f });
		}
		else {
			Enemy4List[i]->SetPosition({ 300.f ,  200.f - 50.0f * (i - 4) , 0.0f });
			Enemy4List[i]->SetRadius(Enemy4List[i]->GetRadius() + 5.f);
		}
		m_MySceneObjects.emplace_back(Enemy4List[i]);
	}
}

// 更新
void TitleScene::Update()
{
	//-----------------------------------------------------------------------------
	// 操作／INPUT
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// 重要：三角形判定で触る前に、死んだオブジェクトを先に掃除する
	//-----------------------------------------------------------------------------


	//m_MySceneObjects中の空間オブジェクトを削除する（erase）
	for (auto it = m_MySceneObjects.begin(); it != m_MySceneObjects.end(); )
	{
		Object* o = *it; // オブジェクト取得
		if (!o)
		{
			it = m_MySceneObjects.erase(it);
			continue;
		}

		if (o->ToBeDeleted())
		{
			Game::GetInstance()->DeleteObject(o);   // ★実体も破棄依頼
			it = m_MySceneObjects.erase(it);        // ★リストからも除去
			continue;
		}

		++it;
	}


	float rt = Input::GetRightTrigger();
	float lt = Input::GetLeftTrigger();
	static bool prevLT = false;
	static bool prevRT = false;
	bool nowLT = (lt >= 0.5f);
	bool nowRT = (rt >= 0.5f);
	bool ltTriggerOnce = (nowLT && !prevLT); 
	bool rtTriggerOnce = (nowRT && !prevRT); 
	prevLT = nowLT;
	prevRT = nowRT;

	if (Input::GetKeyTrigger('D') || Input::GetButtonTrigger(XINPUT_LEFT_SHOULDER) || ltTriggerOnce)   // 
	{
		silkWall* w = nullptr;

		// 1. まず、非アクティブ（消えている）スロットを探す
		for (int i = 0; i < 3; ++i)
		{
			if ((m_SilkWalls[i]->IsActive()) == false)
			{
				w = m_SilkWalls[i];
				break;
			}
		}

		// 2. もし全部埋まっていたら、一番古いもの(m_NextSilkIndex)を上書きする
		if (w == nullptr)
		{
			w = m_SilkWalls[m_NextSilkIndex];
			m_NextSilkIndex = (m_NextSilkIndex + 1) % 3;
		}

		// 3. 発射実行
		if (w && m_HandL && m_HandR)
		{
			w->Fire(m_HandL->GetPosition(), m_HandR->GetPosition());
			w->SetUID(m_SilkCount++);
		}
	}

	// 
	if (Input::GetKeyTrigger('J') || Input::GetKeyTrigger(VK_LEFT) || Input::GetButtonTrigger(XINPUT_RIGHT_SHOULDER)|| rtTriggerOnce)
	{
		silkWall* w = nullptr;

		// 1. まず、非アクティブ（消えている）スロットを探す
		for (int i = 0; i < 3; ++i)
		{
			// 糸壁が非アクティブなら、そこを使う
			if (!(m_SilkWalls[i]->IsActive()))
			{
				w = m_SilkWalls[i];
				break;
			}
		}

		// 2. もし全部埋まっていたら、一番古いもの(m_NextSilkIndex)を上書きする
		if (w == nullptr)
		{
			w = m_SilkWalls[m_NextSilkIndex];
			m_NextSilkIndex = (m_NextSilkIndex + 1) % 3;
		}

		// 3. 発射実行
		if (w && m_HandL && m_HandR)
		{
			Vector3 startPos = m_HandR->GetPosition();  // 右手
			Vector3 targetPos = m_HandL->GetPosition(); // 左手

			w->Fire(startPos, targetPos);
			w->SetUID(m_SilkCount++);
		}
	}

	int count = 0;
	int minIndex = -1;
	int minUID = 0;
	for (int i = 0; i < 3; ++i)
	{
		auto* w = m_SilkWalls[i];
		if (!w) continue;
		//if (m_SilkWalls[i]->IsActive() == false) continue;
		if (w->IsActive())
		{
			++count;
		}
		w->SetOldOne(false);   // 先全部清掉
		int uid = w->GetUID();
		if (minIndex == -1 || uid < minUID)
		{
			minUID = uid;
			minIndex = i;
		}
	}
	//  3 以上ときOldOneを設定 
	if (count > 2 && minIndex != -1)
	{
		
		m_SilkWalls[minIndex]->SetOldOne(true);
	}

	//if (Input::GetKeyTrigger('R'))   // 
	//{
	//	std::vector<Object*> removeList;
	//
	//	//Enemyを探す
	//	for (auto* obj : m_MySceneObjects)
	//	{
	//		if (dynamic_cast<EnemyBase*>(obj))
	//		{
	//			removeList.push_back(obj);
	//		}
	//	}
	//
	//	//　見つけたEnemy1を削除する
	//	for (auto* obj : removeList)
	//	{
	//		Game::GetInstance()->DeleteObject(obj);
	//
	//		auto it = std::find(m_MySceneObjects.begin(), m_MySceneObjects.end(), obj);
	//		if (it != m_MySceneObjects.end())
	//		{
	//			m_MySceneObjects.erase(it);
	//		}
	//	}
	//}

	//-----------------------------------------------------------------------------
	// silkWallの三角形判定
	//-----------------------------------------------------------------------------
	Vector3 A, B, C; // 三角形頂点

	silkWall* walls[3] = { m_SilkWalls[0], m_SilkWalls[1], m_SilkWalls[2] };

	// 3 本とも準備完了しているか？
	const bool allReady = std::all_of(std::begin(walls), std::end(walls),
		[](const silkWall* w) { return w && w->IsActive() && !w->IsGrowing(); });


	if (allReady)
	{
		// 3 本の silkWall から三角形生成を試行
		if (TriangleSilk::TryMakeTriangleFromWallsXY(walls[0], walls[1], walls[2], A, B, C))
		{
			int eliminatedCount = 0;
			bool mikoMayuCount = false;

			// 敵を調べる
			for (auto* obj : m_MySceneObjects)
			{
				if (!obj) continue;
				if (obj->ToBeDeleted()) continue;

				auto* enemy = dynamic_cast<EnemyBase*>(obj);
				if (!enemy) continue;

				const auto pos = enemy->GetPosition();
				if (TriangleSilk::IsInsideTriangleXY(pos, A, B, C))
				{
					Vector3 centroid = (A + B + C) / 3.0f;
					enemy->StartMayuing(centroid);
					++eliminatedCount;
				}
			}

			// 巫女も調べる
			if (m_Miko)
			{
				const auto mikoPos = m_Miko->GetPosition();
				if (TriangleSilk::IsInsideTriangleXY(mikoPos, A, B, C))
				{
					mikoMayuCount = true;
				}
			}

			// Mayu を生成する
			if (eliminatedCount > 0)
			{
				const float baseRadius = 25.0f;
				const float perKill = 5.0f;
				const float mayuRadius = baseRadius + perKill * static_cast<float>(eliminatedCount);

				auto* mayu = Game::GetInstance()->AddObject<EnemyMayu>();
				Vector3 centroid = (A + B + C) / 3.0f;
				mayu->SetPosition(centroid);
				mayu->SetRadius(mayuRadius);
				m_MySceneObjects.emplace_back(mayu);
			}

			// 巫女が三角形に入ったときの処理
			if (mikoMayuCount && m_Miko)
			{
				Vector3 centroid = (A + B + C) / 3.0f;
				m_Miko->SetStartMayuing(centroid);
			}

			for (int i = 0; i < 3; ++i)
			{
				m_SilkWalls[i]->reInit();
			}
		}
	}



	//-----------------------------------------------------------------------------
	// エンターキーを押してステージ1へ
	//-----------------------------------------------------------------------------
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Game::GetInstance()->ChangeScene(SceneName::STAGE1);

		return;
	}

	if (m_Miko->GetDYINGTimer() <= 0) {
		Game::GetInstance()->ChangeScene(SceneName::STAGE1);
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
