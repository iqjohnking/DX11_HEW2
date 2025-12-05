#include "TitleScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"

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
	//背景画像オブジェクト生成
	Texture2D* bg = Game::GetInstance()->AddObject<Texture2D>();
	bg->SetTexture("assets/texture/background1.png");
	bg->SetPosition(0.0f, 0.0f, 0.0f);
	bg->SetRotation(0.0f, 0.0f, 0.0f);
	bg->SetScale(1280.0f, 720.0f, 0.0f);
	m_MySceneObjects.emplace_back(bg);

}

// 更新
void TitleScene::Update()
{
	// エンターキーを押してステージ1へ
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Game::GetInstance()->ChangeScene(STAGE1);
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
