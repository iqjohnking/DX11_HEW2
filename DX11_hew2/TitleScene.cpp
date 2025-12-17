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
	//m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<TitleBG>());
	//m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<field>());
	//m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<silkWall>());

	// 背景
	auto* bg = Game::GetInstance()->AddObject<TitleBG>();
	m_MySceneObjects.emplace_back(bg);
	// 場地
	auto* fld = Game::GetInstance()->AddObject<field>();
	m_MySceneObjects.emplace_back(fld);


	// silkWall*3
	for (int i = 0; i < 3; ++i)
	{
		m_SilkWalls[i] = Game::GetInstance()->AddObject<silkWall>();
		m_MySceneObjects.emplace_back(m_SilkWalls[i]);

		// 
		//m_SilkWalls[i]->SetPosition({ -350.f ,  300.f -100.0f * (i+1) , 0.0f });
	}
	// 左手（W / S 控制）
	m_HandL = Game::GetInstance()->AddObject<playerHand>(0);
	//handL->SetSide(HandSide::Left);

	// 右手（↑ / ↓ 控制）
	m_HandR = Game::GetInstance()->AddObject<playerHand>(1);
	//handR->SetSide(HandSide::Right);
	m_HandL->SetAnotherHand(m_HandR);
	m_HandR->SetAnotherHand(m_HandL);


}

// 更新
void TitleScene::Update()
{
	// エンターキーを押してステージ1へ
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Game::GetInstance()->ChangeScene(STAGE1);
	}
	// 例：スペースキーを押したら糸の壁を順番に発射する
	// 左手往右手發（D）
	if (Input::GetKeyTrigger('D'))   // 或 VK_D 也可，建議用 'D'
	{
		silkWall* w = m_SilkWalls[m_NextSilkIndex];
		if (w && m_HandL && m_HandR)
		{
			Vector3 startPos = m_HandL->GetPosition();  // 左手位置
			Vector3 targetPos = m_HandR->GetPosition(); // 右手位置
			w->Fire(startPos, targetPos);

			// 用掉一條絲 → 往下一個 index
			++m_NextSilkIndex;
			if (m_NextSilkIndex >= 3)
			{
				m_NextSilkIndex = 0;
			}
		}
	}

	// 右手往左手發（J 或者 ←）
	if (Input::GetKeyTrigger('J') || Input::GetKeyTrigger(VK_LEFT))
	{
		silkWall* w = m_SilkWalls[m_NextSilkIndex];
		if (w && m_HandL && m_HandR)
		{
			Vector3 startPos = m_HandR->GetPosition();  // 右手位置
			Vector3 targetPos = m_HandL->GetPosition(); // 左手位置
			w->Fire(startPos, targetPos);

			++m_NextSilkIndex;
			if (m_NextSilkIndex >= 3)
			{
				m_NextSilkIndex = 0;
			}
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		silkWall* wall = m_SilkWalls[i];
		if (!wall) continue;              // 防禦性檢

		// 例）球との当たり判定（Ball クラスに GetCollider() がある前提）
	   /*

	   */
	}
}

// 終了処理
void TitleScene::Uninit()
{
	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}
