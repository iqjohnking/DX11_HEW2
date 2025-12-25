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
	m_HandR->SetAnotherHand(m_HandL);

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
		Enemy1List[i]->SetPosition({ -350.f ,  500.f - 50.0f * (i + 1) , 0.0f });
		m_MySceneObjects.emplace_back(Enemy1List[i]);
	}
}

// 更新
void TitleScene::Update()
{
	// こちのものcontrol.h/cppに移行する予定
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
		if (!wall) continue;              // 防禦性チェック
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
