#include "ModeSelectScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"
#include "TitleBG.h"

ModeSelectScene::ModeSelectScene() 
{

}

ModeSelectScene::~ModeSelectScene() 
{

}

void ModeSelectScene::Init()
{
	// 背景の生成 (TitleBGの書き方に合わせる)
	auto* bg = Game::GetInstance()->AddObject<TitleBG>();
	bg->SetPosition(0.0f, 0.0f, 0.5f);
	m_MySceneObjects.emplace_back(bg);
	
	//モード選択場所の背景
	m_mode_in_L = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_in_L->SetTexture("assets/texture/ui/mode_in.png");
	m_mode_in_L->SetPosition(-350.0f, 0.0f, 0.05f);
	m_mode_in_L->SetScale(600.0f, 800.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_in_L);

	//モード選択場所の背景
	m_mode_in_R = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_in_R->SetTexture("assets/texture/ui/mode_in.png");
	m_mode_in_R->SetPosition(350.0f, 0.0f, 0.05f);
	m_mode_in_R->SetScale(600.0f, 800.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_in_R);
	

	//モード選択の説明の文字
	m_modesentaku= Game::GetInstance()->AddObject<Texture2D>();
	m_modesentaku->SetTexture("assets/texture/ui/modesentaku.png");
	m_modesentaku->SetPosition(0.0f, -450.0f, 0.0f);
	m_modesentaku->SetScale(400.0f, 150.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_modesentaku);

	//モード選択の文字枠
	m_modesentakugamen_waku = Game::GetInstance()->AddObject<Texture2D>();
	m_modesentakugamen_waku->SetTexture("assets/texture/ui/modesentakugamen_waku.png");
	m_modesentakugamen_waku->SetPosition(-800.0f, 450.0f, 0.0f);
	m_modesentakugamen_waku->SetScale(200.0f, 150.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_modesentakugamen_waku);

	//モード選択の文字
	m_modesentakugamen = Game::GetInstance()->AddObject<Texture2D>();
	m_modesentakugamen->SetTexture("assets/texture/ui/modesentakugamen.png");
	m_modesentakugamen->SetPosition(-800.0f, 450.0f, 0.0f);
	m_modesentakugamen->SetScale(150.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_modesentakugamen);

	
	

	// ストーリーボタン (最初はスケール0で隠す)
	m_StoryBtn = Game::GetInstance()->AddObject<Texture2D>();
	m_StoryBtn->SetTexture("assets/texture/ui/storymode.png");
	m_StoryBtn->SetPosition(-350.0f, 350.0f, 0.0f);
	m_StoryBtn->SetScale(400.0f, 150.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_StoryBtn);

	// エンドレスボタン (最初はスケール0で隠す)
	m_EndlessBtn = Game::GetInstance()->AddObject<Texture2D>();
	m_EndlessBtn->SetTexture("assets/texture/ui/endlessmode.png");
	m_EndlessBtn->SetPosition(350.0f, 350.0f, 0.0f);
	m_EndlessBtn->SetScale(400.0f, 150.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_EndlessBtn);
	
}

void ModeSelectScene::Update()
{
	// --- ストーリーボタンの処理 ---
	if (m_mode_in_L) // 1. まず存在するか確認
	{
		if (IsMouseOver(m_mode_in_L))
		{
			m_StoryBtn->SetScale(410.0f, 160.0f, 0.0f);
			m_mode_in_L->SetScale(610.0f, 810.0f, 0.0f);
			if (Input::GetMouseButtonTrigger(0)) {
				Game::GetInstance()->ChangeScene(TITLE);
				return;
			}
		}
		else
		{
			m_StoryBtn->SetScale(400.0f, 150.0f, 0.0f);
			m_mode_in_L->SetScale(600.0f, 800.0f, 0.0f); // 2. 存在が保証されているので安全
		}
	}

	// --- エンドレスボタンの処理 ---
	if (m_mode_in_R) // 1. 存在を確認
	{
		if (IsMouseOver(m_mode_in_R))
		{
			m_EndlessBtn->SetScale(410.0f, 160.0f, 0.0f);
			m_mode_in_R->SetScale(610.0f, 810.0f, 0.0f);
			if (Input::GetMouseButtonTrigger(0)) {
				Game::GetInstance()->ChangeScene(TITLE);
				return;
			}
		}
		else
		{
			m_EndlessBtn->SetScale(400.0f, 150.0f, 0.0f);
			m_mode_in_R->SetScale(600.0f, 800.0f, 0.0f);
		}
	}
}

void ModeSelectScene::Uninit()
{
	for (auto& o : m_MySceneObjects) 
	{
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}

bool ModeSelectScene::IsMouseOver(Texture2D* obj)
{
	if (!obj) return false;

	// 前の手順で Input に追加した関数を使用
	DirectX::XMFLOAT2 mouse = Input::GetMousePosition();

	DirectX::SimpleMath::Vector3 pos = obj->GetPosition();
	DirectX::SimpleMath::Vector3 scale = obj->GetScale();

	// 画像の中心から上下左右の端を計算
	float left = pos.x - (scale.x / 2.0f);
	float right = pos.x + (scale.x / 2.0f);
	float top = pos.y + (scale.y / 2.0f);
	float bottom = pos.y - (scale.y / 2.0f);

	return (mouse.x >= left && mouse.x <= right && mouse.y >= bottom && mouse.y <= top);
}