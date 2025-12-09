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
	if (Input::GetKeyTrigger(VK_D))
	{
		silkWall* w = m_SilkWalls[m_NextSilkIndex];
		if (w)
		{
			// TODO: ここは本当は「右手 / 左手の位置」を渡す
			// 例：Vector3 startPos = rightHand->GetPosition();
			Vector3 startPos(-350.0f, 300.0f - (m_NextSilkIndex * 200.f), 0.0f);
			Vector3 targetPos(0.0f, 0.0f, 0.0f);
			w->Fire(startPos, targetPos);
		}

		// 次に使うインデックスを進める（0 → 1 → 2 → 0 → …）
		++m_NextSilkIndex;
		if (m_NextSilkIndex >= 3)
		{
			m_NextSilkIndex = 0;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		silkWall* wall = m_SilkWalls[i];
		if (!wall) continue;              // 防禦性檢?

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
