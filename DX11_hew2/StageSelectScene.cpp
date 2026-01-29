#include "StageSelectScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"
#include "SelectBG.h"


StageSelectScene::StageSelectScene()
{

}

StageSelectScene::~StageSelectScene()
{

}

void StageSelectScene::Init()
{
	// 背景の生成
	auto* bg = Game::GetInstance()->AddObject<SelectBG>();
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
	
	m_FadePanel = Game::GetInstance()->AddObject<Texture2D>();
	m_FadePanel->SetTexture("assets/texture/terrain.png");
	m_FadePanel->SetPosition(0.0f, 0.0f, 0.0f);
	m_FadePanel->SetScale(2000.0f, 2000.0f, 0.0f);
	m_FadePanel->SetAlpha(1.0f);                           // 最初は黒
	m_MySceneObjects.emplace_back(m_FadePanel);
}

void StageSelectScene::Update()
{
	// --- フェードのタイマー ---
	const float FADE_TIME = 0.2f; //0.5秒で終了
	float Time = 1.0f / 60.0f;

	if (m_isStarting)
	{
		m_fadeAlpha += Time / FADE_TIME;
	}
	else
	{
		m_fadeAlpha -= Time / FADE_TIME;
	}

	if (m_fadeAlpha > 1.0f) m_fadeAlpha = 1.0f;
	if (m_fadeAlpha < 0.0f) m_fadeAlpha = 0.0f;

	// アルファ値を反映
	if (m_FadePanel) m_FadePanel->SetAlpha(m_fadeAlpha);

	// 暗転完了後の切り替え
	if (m_isStarting && m_fadeAlpha >= 1.0f)
	{
		if (m_NextSceneID == 1) Game::GetInstance()->ChangeScene(STAGE1);
		else if (m_NextSceneID == 2) Game::GetInstance()->ChangeScene(STAGE2);
		else if (m_NextSceneID == 3) Game::GetInstance()->ChangeScene(STAGE3);
		else if (m_NextSceneID == 4) Game::GetInstance()->ChangeScene(STAGE4);
		else if (m_NextSceneID == 5) Game::GetInstance()->ChangeScene(STAGE5);
		else if (m_NextSceneID == 6) Game::GetInstance()->ChangeScene(STAGE6);
		else if (m_NextSceneID == 7) Game::GetInstance()->ChangeScene(STAGE7);
		else if (m_NextSceneID == 8) Game::GetInstance()->ChangeScene(STAGE8);
		else if (m_NextSceneID == 9) Game::GetInstance()->ChangeScene(STAGE9);
		else if (m_NextSceneID == 10) Game::GetInstance()->ChangeScene(MODE_SELECT);
		return;
	}

	if (m_isStarting) return;
	/*
	// --- フェード処理 ---
	float targetAlpha;
	if (m_isStarting)
	{
		targetAlpha = 1.0f;
	}
	else
	{
		targetAlpha = 0.0f;

	}

	float fadeSpeed ;
	if (m_isStarting)
	{
		fadeSpeed = 0.09;
	}
	else
	{
		fadeSpeed = 0.09;
	}

	m_fadeAlpha += (targetAlpha - m_fadeAlpha) * fadeSpeed;

	if (m_FadePanel) m_FadePanel->SetAlpha(m_fadeAlpha);

	// 暗転完了後の切り替え
	if (m_isStarting && m_fadeAlpha > 0.99f)
	{
		
			 if (m_NextSceneID == 1) Game::GetInstance()->ChangeScene(STAGE1);
		else if (m_NextSceneID == 2) Game::GetInstance()->ChangeScene(STAGE2);
		else if (m_NextSceneID == 3) Game::GetInstance()->ChangeScene(STAGE3);
		else if (m_NextSceneID == 4) Game::GetInstance()->ChangeScene(STAGE4);
		else if (m_NextSceneID == 5) Game::GetInstance()->ChangeScene(STAGE5);
		else if (m_NextSceneID == 6) Game::GetInstance()->ChangeScene(STAGE6);
		else if (m_NextSceneID == 7) Game::GetInstance()->ChangeScene(STAGE7);
		else if (m_NextSceneID == 8) Game::GetInstance()->ChangeScene(STAGE8);
		else if (m_NextSceneID == 9) Game::GetInstance()->ChangeScene(STAGE9);
		else if (m_NextSceneID == 10) Game::GetInstance()->ChangeScene(MODE_SELECT);
		
		return;
	}

	// フェードアウト中（暗転中）は以下の操作を受け付けない
	if (m_isStarting) return;
	*/
	static DirectX::XMFLOAT2 lastMousePos = { 0, 0 };
	DirectX::XMFLOAT2 currentMousePos = Input::GetMousePosition();

	bool mouseMoved = (currentMousePos.x != lastMousePos.x || currentMousePos.y != lastMousePos.y);
	lastMousePos = currentMousePos;

	if (mouseMoved)
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

	}
	
	/*
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
	*/

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
	if (Input::GetButtonTrigger(XINPUT_LEFT)&&Input::GetKeyTrigger(VK_A)) 
	{ 
		Game::GetSound()->Play(SOUND_LABEL_SE_000);
		m_SelectIndex = (m_SelectIndex + 2) % 3; 
	}
	if (Input::GetButtonTrigger(XINPUT_RIGHT) && Input::GetKeyTrigger(VK_D))
	{ 
		Game::GetSound()->Play(SOUND_LABEL_SE_000);
		m_SelectIndex = (m_SelectIndex + 1) % 3; 
	}
	
	// --- 目標サイズ ---
	float targetL;
	float targetM;
	float targetR;

	if (m_SelectIndex == 0) 
	{
		targetL = 440.0f;// 選択中なら大きく
	}
	else 
	{
		targetL = 400.0f;// 選択してないなら普通
	}

	if (m_SelectIndex == 1) 
	{
		targetM = 440.0f;// 選択中なら大きく
	}
	else 
	{
		targetM = 400.0f;// 選択してないなら普通
	}

	if (m_SelectIndex == 2) 
	{
		targetR = 440.0f;// 選択中なら大きく
	}
	else 
	{
		targetR = 400.0f;// 選択してないなら普通
	}

	// 滑らかに 
	const float speed = 0.5f;
	m_curScaleL += (targetL - m_curScaleL) * speed;
	m_curScaleM += (targetM - m_curScaleM) * speed;
	m_curScaleR += (targetR - m_curScaleR) * speed;

	// サイズを反映
	// 左ボタン
	if (m_mode_in_L) 
	{
		m_mode_in_L->SetScale(m_curScaleL, m_curScaleL, 0.0f);
	}
	
	// 真ん中ボタン
	if (m_mode_in_M) 
	{
		m_mode_in_M->SetScale(m_curScaleM, m_curScaleM, 0.0f);
	}
	
	// 右ボタン
	if (m_mode_in_R) 
	{
		m_mode_in_R->SetScale(m_curScaleR, m_curScaleR, 0.0f);
	}

	bool isMouseClickOnButton = (IsMouseOver(m_mode_in_L) || IsMouseOver(m_mode_in_M) || IsMouseOver(m_mode_in_R));

	// --- 決定処理（Aボタン） ---
	if (Input::GetButtonTrigger(XINPUT_A) || Input::GetKeyTrigger(VK_RETURN) || (Input::GetMouseButtonTrigger(0) && isMouseClickOnButton))
	{
		m_isStarting = true;
		Game::GetSound()->Play(SOUND_LABEL_SE_000);
		if (m_Chapter == 1) 
		{
			// 第1章
			if (m_SelectIndex == 0) m_NextSceneID = 1; // STAGE1
			if (m_SelectIndex == 1 )m_NextSceneID = 2; // STAGE2
			if (m_SelectIndex == 2) m_NextSceneID = 3; // STAGE3
		}
		else if (m_Chapter == 2)
		{
			// 第2章
			if (m_SelectIndex == 0) m_NextSceneID = 4; // STAGE4
			if (m_SelectIndex == 1) m_NextSceneID = 5; // STAGE5
			if (m_SelectIndex == 2) m_NextSceneID = 6; // STAGE6
		}
		else if (m_Chapter == 3)
		{
			// 第3章
			if (m_SelectIndex == 0) m_NextSceneID = 7; // STAGE7
			if (m_SelectIndex == 1) m_NextSceneID = 8; // STAGE8
			if (m_SelectIndex == 2) m_NextSceneID = 9; // STAGE9
		}
		return;
	}
	
	//一個戻る
	if (Input::GetButtonTrigger(XINPUT_B) ||
		Input::GetKeyTrigger(VK_SHIFT))
	{
		Game::GetSound()->Play(SOUND_LABEL_SE_000);
		m_isStarting = true;
		m_NextSceneID = 10; // MODE_SELECT へ
		//Game::GetInstance()->ChangeScene(MODE_SELECT);
		
		return;
	}


	// --- Lボタン(LB)でチャプター切り替え ---
	if (Input::GetButtonTrigger(XINPUT_RIGHT_SHOULDER)|| Input::GetKeyTrigger(VK_RIGHT))
	{
		Game::GetSound()->Play(SOUND_LABEL_SE_000);
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
			m_daiissyou->SetTexture("assets/texture/ui/2222.png");

			m_mode_in_L->SetTexture("assets/texture/miko.png");
			m_mode_in_M->SetTexture("assets/texture/miko.png");
			m_mode_in_R->SetTexture("assets/texture/miko.png");
		}
		else if (m_Chapter == 3)
		{
			m_daiissyou->SetTexture("assets/texture/ui/3333.png");

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
			m_daiissyou->SetTexture("assets/texture/ui/2222.png");

			m_mode_in_L->SetTexture("assets/texture/miko.png");
			m_mode_in_M->SetTexture("assets/texture/miko.png");
			m_mode_in_R->SetTexture("assets/texture/miko.png");
		}
		else if (m_Chapter == 3)
		{
			m_daiissyou->SetTexture("assets/texture/ui/3333.png");

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

