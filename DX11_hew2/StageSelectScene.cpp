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
	m_mode_in_L->SetTexture("assets/texture/stageselect/stageselect000.png");
	m_mode_in_L->SetPosition(-530.0f, 50.0f, 0.0f);
	m_mode_in_L->SetScale(m_curScaleL * 2.0f, m_curScaleL, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_in_L);

	m_mode_in_M = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_in_M->SetTexture("assets/texture/stageselect/stageselect001.png");
	m_mode_in_M->SetPosition(30.0f, 50.0f, 0.0f);
	m_mode_in_M->SetScale(m_curScaleM * 2.0f, m_curScaleM, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_in_M);

	//モード選択場所の内背景
	m_mode_in_R = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_in_R->SetTexture("assets/texture/stageselect/stageselect002.png");
	m_mode_in_R->SetPosition(580.0f, 50.0f, 0.0f);
	m_mode_in_R->SetScale(m_curScaleR * 2.0f, m_curScaleR, 0.0f);
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
	m_daiissyou_waku->SetPosition(0.0f, -250.0f, 0.0f);
	m_daiissyou_waku->SetScale(400.0f, 200.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_daiissyou_waku);

	//第一章　文字
	m_daiissyou = Game::GetInstance()->AddObject<Texture2D>();
	m_daiissyou->SetTexture("assets/texture/ui/daiissyou.png");
	m_daiissyou->SetPosition(0.0f, -250.0f, 0.0f);
	m_daiissyou->SetScale(200.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_daiissyou);

	//右矢印
	m_migiyazirusi= Game::GetInstance()->AddObject<Texture2D>();
	m_migiyazirusi->SetTexture("assets/texture/ui/migiyazirusi.png");
	m_migiyazirusi->SetPosition(260.0f, -250.0f, 0.0f);
	m_migiyazirusi ->SetScale(100.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_migiyazirusi);

	//右矢印
	m_hidariyazirusi = Game::GetInstance()->AddObject<Texture2D>();
	m_hidariyazirusi->SetTexture("assets/texture/ui/migiyazirusi.png");
	m_hidariyazirusi->SetPosition(-260.0f, -250.0f, 0.0f);
	m_hidariyazirusi->SetRotation(0.0f, 0.0f, 180.0f);
	m_hidariyazirusi->SetScale(100.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_hidariyazirusi);
	
	//左右ステージを選ぶ
	m_storysentaku= Game::GetInstance()->AddObject<Texture2D>();
	m_storysentaku->SetTexture("assets/texture/stageselecthint/stageselecthint000.png");
	m_storysentaku->SetPosition(-700.0f, -495.0f, 0.0f);
	m_storysentaku->SetScale(500.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_storysentaku);

	//下で決定
	m_storysentaku2 = Game::GetInstance()->AddObject<Texture2D>();
	m_storysentaku2->SetTexture("assets/texture/stageselecthint/stageselecthint001.png");
	m_storysentaku2->SetPosition(430.0f, -495.0f, 0.0f);
	m_storysentaku2->SetScale(250.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_storysentaku2);

	//LR
	m_storysentaku3 = Game::GetInstance()->AddObject<Texture2D>();
	m_storysentaku3->SetTexture("assets/texture/stageselecthint/stageselecthint002.png");
	m_storysentaku3->SetPosition(-250.0f, -495.0f, 0.0f);
	m_storysentaku3->SetScale(350.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_storysentaku3);
	
	//モード選択へ
	m_storysentaku4 = Game::GetInstance()->AddObject<Texture2D>();
	m_storysentaku4->SetTexture("assets/texture/stageselecthint/stageselecthint003.png");
	m_storysentaku4->SetPosition(700.0f, -502.0f, 0.0f);
	m_storysentaku4->SetScale(350.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_storysentaku4);

	m_FadePanel = Game::GetInstance()->AddObject<Texture2D>();
	m_FadePanel->SetTexture("assets/texture/black.png");
	m_FadePanel->SetPosition(0.0f, 0.0f, 0.0f);
	m_FadePanel->SetScale(2000.0f, 2000.0f, 0.0f);
	m_FadePanel->SetAlpha(1.0f);                           // 最初は黒
	m_MySceneObjects.emplace_back(m_FadePanel);
}

void StageSelectScene::Update()
{
	// --- フェードのタイマー ---
	const float FADE_TIME = 0.5f; //0.5秒で終了
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
		if (m_NextSceneID == 1) Game::GetInstance()->ChangeScene(SceneName::STAGE1);
		else if (m_NextSceneID == 2) Game::GetInstance()->ChangeScene(SceneName::STAGE2);
		else if (m_NextSceneID == 3) Game::GetInstance()->ChangeScene(SceneName::STAGE3);
		else if (m_NextSceneID == 4) Game::GetInstance()->ChangeScene(SceneName::STAGE4);
		else if (m_NextSceneID == 5) Game::GetInstance()->ChangeScene(SceneName::STAGE5);
		else if (m_NextSceneID == 6) Game::GetInstance()->ChangeScene(SceneName::STAGE6);
		else if (m_NextSceneID == 7) Game::GetInstance()->ChangeScene(SceneName::STAGE7);
		else if (m_NextSceneID == 8) Game::GetInstance()->ChangeScene(SceneName::STAGE8);
		else if (m_NextSceneID == 9) Game::GetInstance()->ChangeScene(SceneName::STAGE9);
		else if (m_NextSceneID == 10) Game::GetInstance()->ChangeScene(SceneName::MODE_SELECT);
		return;
	}

	if (m_isStarting) return;
	
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
	if (Input::GetButtonTrigger(XINPUT_LEFT)||Input::GetKeyTrigger(VK_A)) 
	{ 
		Game::GetSound()->Play(SOUND_LABEL_SE_010);
		m_SelectIndex = (m_SelectIndex + 2) % 3; 
	}
	if (Input::GetButtonTrigger(XINPUT_RIGHT) || Input::GetKeyTrigger(VK_D))
	{ 
		Game::GetSound()->Play(SOUND_LABEL_SE_010);
		m_SelectIndex = (m_SelectIndex + 1) % 3; 
	}
	
	// --- 目標サイズ ---
	float targetL;
	float targetM;
	float targetR;

	if (m_SelectIndex == 0) 
	{
		targetL = 620.0f;// 選択中なら大きく
	}
	else 
	{
		targetL = 600.0f;// 選択してないなら普通
	}

	if (m_SelectIndex == 1) 
	{
		targetM = 620.0f;// 選択中なら大きく
	}
	else 
	{
		targetM = 600.0f;// 選択してないなら普通
	}

	if (m_SelectIndex == 2) 
	{
		targetR = 620.0f;// 選択中なら大きく
	}
	else 
	{
		targetR = 600.0f;// 選択してないなら普通
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
		m_mode_in_L->SetScale(m_curScaleL * 2.0f, m_curScaleL, 0.0f);
	}
	
	// 真ん中ボタン
	if (m_mode_in_M) 
	{
		m_mode_in_M->SetScale(m_curScaleM * 2.0f, m_curScaleM, 0.0f);
	}
	
	// 右ボタン
	if (m_mode_in_R) 
	{
		m_mode_in_R->SetScale(m_curScaleR * 2.0f, m_curScaleR, 0.0f);
	}

	bool isMouseClickOnButton = (IsMouseOver(m_mode_in_L) || IsMouseOver(m_mode_in_M) || IsMouseOver(m_mode_in_R));

	// --- 決定処理（Aボタン） ---
	if (Input::GetButtonTrigger(XINPUT_A) || Input::GetKeyTrigger(VK_RETURN) || (Input::GetMouseButtonTrigger(0) && isMouseClickOnButton))
	{
		m_isStarting = true;
		Game::GetSound()->Play(SOUND_LABEL_SE_011);
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
		Input::GetKeyTrigger(VK_SHIFT)	  ||
		Input::GetMouseButtonTrigger(1))
	{
		Game::GetSound()->Play(SOUND_LABEL_SE_010);
		m_isStarting = true;
		m_NextSceneID = 10; // MODE_SELECT へ
		//Game::GetInstance()->ChangeScene(MODE_SELECT);
		
		return;
	}

	// --- 矢印の表示制御 ---
	if (m_Chapter == 1) 
	{
		// 1章のときは左に戻れないので、左矢印を半透明にする
		if (m_hidariyazirusi) m_hidariyazirusi->SetAlpha(0.2f);
	}
	else 
	{
		if (m_hidariyazirusi) m_hidariyazirusi->SetAlpha(1.0f);
	}

	if (m_Chapter == 3) 
	{
		// 3章のときは右に行けないので、右矢印を半透明にする
		if (m_migiyazirusi) m_migiyazirusi->SetAlpha(0.2f);
	}
	else 
	{
		if (m_migiyazirusi) m_migiyazirusi->SetAlpha(1.0f);
	}

	// --- Lボタン(LB)でチャプター切り替え ---
	if (Input::GetButtonTrigger(XINPUT_RIGHT_SHOULDER)|| Input::GetKeyTrigger(VK_RIGHT))
	{
		Game::GetSound()->Play(SOUND_LABEL_SE_010);
		m_Chapter++;
		if (m_Chapter >= 3) m_Chapter = 3; // 3章（3-3）までなので、4になったら1に戻す

		if (m_Chapter == 1)
		{
			m_daiissyou->SetTexture("assets/texture/ui/daiissyou.png");

			m_mode_in_L->SetTexture("assets/texture/stageselect/stageselect000.png");
			m_mode_in_M->SetTexture("assets/texture/stageselect/stageselect001.png");
			m_mode_in_R->SetTexture("assets/texture/stageselect/stageselect002.png");
		}
		else if (m_Chapter == 2)
		{
			m_daiissyou->SetTexture("assets/texture/ui/2222.png");

			m_mode_in_L->SetTexture("assets/texture/stageselect/stageselect003.png");
			m_mode_in_M->SetTexture("assets/texture/stageselect/stageselect004.png");
			m_mode_in_R->SetTexture("assets/texture/stageselect/stageselect005.png");
		}
		else if (m_Chapter == 3)
		{
			m_daiissyou->SetTexture("assets/texture/ui/3333.png");

			m_mode_in_L->SetTexture("assets/texture/stageselect/stageselect006.png");
			m_mode_in_M->SetTexture("assets/texture/stageselect/stageselect007.png");
			m_mode_in_R->SetTexture("assets/texture/stageselect/stageselect008.png");
		}
	}
	else if (Input::GetButtonTrigger(XINPUT_LEFT_SHOULDER) || Input::GetKeyTrigger(VK_LEFT))
	{
		Game::GetSound()->Play(SOUND_LABEL_SE_010);
		m_Chapter--;
		if (m_Chapter <= 1) m_Chapter = 1;
		
		if (m_Chapter == 1)
		{
			m_daiissyou->SetTexture("assets/texture/ui/daiissyou.png");

			m_mode_in_L->SetTexture("assets/texture/stageselect/stageselect000.png");
			m_mode_in_M->SetTexture("assets/texture/stageselect/stageselect001.png");
			m_mode_in_R->SetTexture("assets/texture/stageselect/stageselect002.png");
			
		}
		else if (m_Chapter == 2)
		{
			m_daiissyou->SetTexture("assets/texture/ui/2222.png");

			m_mode_in_L->SetTexture("assets/texture/stageselect/stageselect003.png");
			m_mode_in_M->SetTexture("assets/texture/stageselect/stageselect004.png");
			m_mode_in_R->SetTexture("assets/texture/stageselect/stageselect005.png");
		}
		else if (m_Chapter == 3)
		{
			m_daiissyou->SetTexture("assets/texture/ui/3333.png");

			m_mode_in_L->SetTexture("assets/texture/stageselect/stageselect006.png");
			m_mode_in_M->SetTexture("assets/texture/stageselect/stageselect007.png");
			m_mode_in_R->SetTexture("assets/texture/stageselect/stageselect008.png");
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
	
	float hitWidth = scale.y;
	float hitHeight = scale.y;

	// 画像の中心から上下左右の端を計算
	float left = pos.x - (hitWidth / 2.0f);
	float right = pos.x + (hitWidth / 2.0f);
	float top = pos.y + (hitHeight / 2.0f);
	float bottom = pos.y - (hitHeight / 2.0f);
	/*
	// 画像の中心から上下左右の端を計算
	float left = pos.x - (scale.x / 2.0f);
	float right = pos.x + (scale.x / 2.0f);
	float top = pos.y + (scale.y / 2.0f);
	float bottom = pos.y - (scale.y / 2.0f);
	*/
	return (mouse.x >= left && mouse.x <= right && mouse.y >= bottom && mouse.y <= top);
}

