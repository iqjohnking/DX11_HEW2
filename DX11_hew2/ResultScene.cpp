#include "ResultScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"

// コンストラクタ
ResultScene::ResultScene()
{
	Init();
}

// デストラクタ
ResultScene::~ResultScene()
{
	Uninit();
}

// 初期化
void ResultScene::Init()
{
	// 背景画像オブジェクト生成
	Texture2D* pt = Game::GetInstance()->AddObject<Texture2D>();
	pt->SetTexture("assets/texture/background2.png");
	pt->SetScale(1280.0f, 720.0f, 0.0f);
	m_MySceneObjects.emplace_back(pt);

	//リザルト文字列オブジェクト生成
	Texture2D* pt2 = Game::GetInstance()->AddObject<Texture2D>();
	pt2->SetTexture("assets/texture/resultString.png");
	pt2->SetScale(700.0f, 100.0f, 0.0f);
	pt2->SetUV(1, 5, 1, 13);

	m_MySceneObjects.emplace_back(pt2);


}

// 更新
void ResultScene::Update()
{
	// エンターキーを押してタイトルへ
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Game::GetInstance()->ChangeScene(TITLE);
	}
}

// 終了処理
void ResultScene::Uninit()
{
	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}
