#include "Stage1Scene.h"
#include "Game.h"
#include "Input.h"
#include "GolfBall.h"
#include "Ground.h"

// コンストラクタ
Stage1Scene::Stage1Scene()
{
	Init();
}

// デストラクタ
Stage1Scene::~Stage1Scene()
{
	Uninit();
}

// 初期化
void Stage1Scene::Init()
{
	// オブジェクトを作成
	Camera* cam = Game::GetInstance()->GetCamera();

	m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<GolfBall>(cam));
	m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Ground>());

}

//更新
void Stage1Scene::Update()
{
	// エンターキーを押してリザルトへ
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Game::GetInstance()->ChangeScene(RESULT);
	}
}

// 終了処理
void Stage1Scene::Uninit()
{
	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}
