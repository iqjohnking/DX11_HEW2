#include "StageSelectScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"
#include "TitleBG.h"


StageSelectScene::StageSelectScene()
{

}

StageSelectScene::~StageSelectScene()
{

}

void StageSelectScene::Init()
{
	// 背景の生成
	auto* bg = Game::GetInstance()->AddObject<TitleBG>();
	bg->SetPosition(0.0f, 0.0f, 0.5f);
	m_MySceneObjects.emplace_back(bg);

	//モード選択場所の内背景
	m_mode_in_L = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_in_L->SetTexture("assets/texture/enemy.png");
	m_mode_in_L->SetPosition(-550.0f, 50.0f, 0.0f);
	m_mode_in_L->SetScale(400.0f, 400.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_in_L);

	m_mode_in_M = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_in_M->SetTexture("assets/texture/enemy.png");
	m_mode_in_M->SetPosition(0.0f, 50.0f, 0.0f);
	m_mode_in_M->SetScale(400.0f, 400.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_in_M);

	//モード選択場所の内背景
	m_mode_in_R = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_in_R->SetTexture("assets/texture/enemy.png");
	m_mode_in_R->SetPosition(550.0f, 50.0f, 0.0f);
	m_mode_in_R->SetScale(400.0f, 400.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_in_R);

	//ステージ選択画面　文字枠
	m_stagesentakugamen_waku = Game::GetInstance()->AddObject<Texture2D>();
	m_stagesentakugamen_waku->SetTexture("assets/texture/ui/stagesentakugamen_waku.png");
	m_stagesentakugamen_waku->SetPosition(-700.0f, 450.0f, 0.0f);
	m_stagesentakugamen_waku->SetScale(500.0f, 150.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_stagesentakugamen_waku);

	//ステージ選択画面　文字
	m_stagesentakugamen = Game::GetInstance()->AddObject<Texture2D>();
	m_stagesentakugamen->SetTexture("assets/texture/ui/stagesentakugamen.png");
	m_stagesentakugamen->SetPosition(-700.0f, 450.0f, 0.0f);
	m_stagesentakugamen->SetScale(450.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_stagesentakugamen);

	//第一章　文字枠
	m_daiissyou_waku = Game::GetInstance()->AddObject<Texture2D>();
	m_daiissyou_waku->SetTexture("assets/texture/ui/daiissyou_waku.png");
	m_daiissyou_waku->SetPosition(0.0f, -330.0f, 0.0f);
	m_daiissyou_waku->SetScale(400.0f, 200.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_daiissyou_waku);

	//第一章　文字
	m_daiissyou = Game::GetInstance()->AddObject<Texture2D>();
	m_daiissyou->SetTexture("assets/texture/ui/daiissyou.png");
	m_daiissyou->SetPosition(0.0f, -330.0f, 0.0f);
	m_daiissyou->SetScale(200.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_daiissyou);

	//右矢印
	m_migiyazirusi= Game::GetInstance()->AddObject<Texture2D>();
	m_migiyazirusi->SetTexture("assets/texture/ui/migiyazirusi.png");
	m_migiyazirusi->SetPosition(350.0f, -330.0f, 0.0f);
	m_migiyazirusi ->SetScale(100.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_migiyazirusi);
	
	//ストーリー選択　説明
	m_storysentaku= Game::GetInstance()->AddObject<Texture2D>();
	m_storysentaku->SetTexture("assets/texture/ui/storysentaku.png");
	m_storysentaku->SetPosition(0.0f, -480.0f, 0.0f);
	m_storysentaku->SetScale(650.0f, 200.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_storysentaku);
	
}

void StageSelectScene::Update()
{
	if (m_mode_in_L && IsMouseOver(m_mode_in_L))
	{
		m_SelectIndex = 0;
	}
	if (m_mode_in_M && IsMouseOver(m_mode_in_M))
	{
		m_SelectIndex = 1;
	}
	if (m_mode_in_R && IsMouseOver(m_mode_in_R))
	{
		m_SelectIndex = 2;
	}

	DirectX::XMFLOAT2 stick = Input::GetLeftAnalogStick();

	// スティックの倒し始めを判定するための変数
	static bool stickFree = true;

	if (stickFree) 
	{
		if (stick.x < -0.5f) // 左に倒した
		{
			
			m_SelectIndex = (m_SelectIndex + 2) % 3;
			stickFree = false;
		}
		else if (stick.x > 0.5f)// 右に倒した 
		{ 
			m_SelectIndex = (m_SelectIndex + 1) % 3;
			stickFree = false;
		}
	}
	if (abs(stick.x) < 0.2f) stickFree = true; // スティックを離したら入力を受け付ける

	// --- コントローラーの左右で選択を動かす ---
	if (Input::GetButtonTrigger(XINPUT_LEFT)) { m_SelectIndex = (m_SelectIndex + 2) % 3; }
	if (Input::GetButtonTrigger(XINPUT_RIGHT)){ m_SelectIndex = (m_SelectIndex + 1) % 3; }
	
	// --- 左ボタン (Index: 0) ---
	if (m_mode_in_L)
	{
		if (m_SelectIndex == 0) 
		{
			if (IsMouseOver(m_mode_in_L)) m_SelectIndex = 0;
			m_mode_in_L->SetScale(430.0f, 430.0f, 0.0f); // 選択中
		}
		else
		{
			m_mode_in_L->SetScale(400.0f, 400.0f, 0.0f); // 非選択
		}
	}

	// --- 真ん中ボタン (Index: 1) ---
	if (m_mode_in_M)
	{
		if (m_SelectIndex == 1)
		{
			if (IsMouseOver(m_mode_in_L)) m_SelectIndex = 1;
			m_mode_in_M->SetScale(430.0f, 430.0f, 0.0f);
		}
		else
		{
			m_mode_in_M->SetScale(400.0f, 400.0f, 0.0f);
		}
	}

	// --- 右ボタン (Index: 2) ---
	if (m_mode_in_R)
	{
		if (m_SelectIndex == 2)
		{
			if (IsMouseOver(m_mode_in_L)) m_SelectIndex = 2;
			m_mode_in_R->SetScale(430.0f, 430.0f, 0.0f);
		}
		else
		{
			m_mode_in_R->SetScale(400.0f, 400.0f, 0.0f);
		}
	}

	// --- 決定処理（Aボタン） ---
	if (Input::GetButtonTrigger(XINPUT_A) || Input::GetMouseButtonTrigger(0))
	{
		if (m_Chapter == 1) 
		{
			// 第1章
			if (m_SelectIndex == 0) Game::GetInstance()->ChangeScene(STAGE1);
			if (m_SelectIndex == 1) Game::GetInstance()->ChangeScene(STAGE2);
			if (m_SelectIndex == 2) Game::GetInstance()->ChangeScene(STAGE3);
		}
		else if (m_Chapter == 2)
		{
			// 第2章
			if (m_SelectIndex == 0) Game::GetInstance()->ChangeScene(STAGE4);
			if (m_SelectIndex == 1) Game::GetInstance()->ChangeScene(STAGE5);
			if (m_SelectIndex == 2) Game::GetInstance()->ChangeScene(STAGE6);
		}
		else if (m_Chapter == 3)
		{
			// 第3章
			if (m_SelectIndex == 0) Game::GetInstance()->ChangeScene(STAGE7);
			if (m_SelectIndex == 1) Game::GetInstance()->ChangeScene(STAGE8);
			if (m_SelectIndex == 2) Game::GetInstance()->ChangeScene(STAGE9);
		}
		return;
	}
	

	// --- Lボタン(LB)でチャプター切り替え ---
	if (Input::GetButtonTrigger(XINPUT_RIGHT_SHOULDER)|| Input::GetKeyTrigger(VK_RIGHT))
	{
		m_Chapter++;
		if (m_Chapter > 3) m_Chapter = 1; // 3章（3-3）までなので、4になったら1に戻す

		if (m_Chapter == 1)
		{
			m_daiissyou->SetTexture("assets/texture/ui/daiissyou.png");

			m_mode_in_L->SetTexture("assets/texture/enemy.png");
			m_mode_in_M->SetTexture("assets/texture/enemy.png");
			m_mode_in_R->SetTexture("assets/texture/enemy.png");
		}
		else if (m_Chapter == 2)
		{
			m_daiissyou->SetTexture("assets/texture/hand.png");

			m_mode_in_L->SetTexture("assets/texture/miko.png");
			m_mode_in_M->SetTexture("assets/texture/miko.png");
			m_mode_in_R->SetTexture("assets/texture/miko.png");
		}
		else if (m_Chapter == 3)
		{
			m_daiissyou->SetTexture("assets/texture/titlerogo.png");

			m_mode_in_L->SetTexture("assets/texture/ground.png");
			m_mode_in_M->SetTexture("assets/texture/ground.png");
			m_mode_in_R->SetTexture("assets/texture/ground.png");
		}
	}
	else if (Input::GetButtonTrigger(XINPUT_LEFT_SHOULDER) || Input::GetKeyTrigger(VK_LEFT))
	{
		m_Chapter--;
		if (m_Chapter < 1) m_Chapter = 3;
		
		if (m_Chapter == 1)
		{
			m_daiissyou->SetTexture("assets/texture/ui/daiissyou.png");

			m_mode_in_L->SetTexture("assets/texture/enemy.png");
			m_mode_in_M->SetTexture("assets/texture/enemy.png");
			m_mode_in_R->SetTexture("assets/texture/enemy.png");
		}
		else if (m_Chapter == 2)
		{
			m_daiissyou->SetTexture("assets/texture/hand.png");

			m_mode_in_L->SetTexture("assets/texture/miko.png");
			m_mode_in_M->SetTexture("assets/texture/miko.png");
			m_mode_in_R->SetTexture("assets/texture/miko.png");
		}
		else if (m_Chapter == 3)
		{
			m_daiissyou->SetTexture("assets/texture/titlerogo.png");

			m_mode_in_L->SetTexture("assets/texture/ground.png");
			m_mode_in_M->SetTexture("assets/texture/ground.png");
			m_mode_in_R->SetTexture("assets/texture/ground.png");
		}
	}

}

void StageSelectScene::Uninit()
{
	for (auto& o : m_MySceneObjects)
	{
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}


bool StageSelectScene::IsMouseOver(Texture2D* obj)
{
	if (!obj) return false;


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

