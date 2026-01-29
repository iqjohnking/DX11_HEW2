#include "ModeSelectScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"
#include "SelectBG.h"

ModeSelectScene::ModeSelectScene() 
{

}

ModeSelectScene::~ModeSelectScene() 
{

}

void ModeSelectScene::Init()
{
	// 背景の生成
	auto* bg = Game::GetInstance()->AddObject<SelectBG>();
	bg->SetPosition(0.0f, 0.0f, 0.5f);
	m_MySceneObjects.emplace_back(bg);
	
	//モード選択場所の外背景
	m_mode_out_L = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_out_L->SetTexture("assets/texture/ui/mode_out.png");
	m_mode_out_L->SetPosition(-350.0f, -30.0f, 0.05f);
	m_mode_out_L->SetScale(700.0f, 930.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_out_L);

	//モード選択場所の外背景
	m_mode_out_R = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_out_R->SetTexture("assets/texture/ui/mode_out.png");
	m_mode_out_R->SetPosition(350.0f, -30.0f, 0.05f);
	m_mode_out_R->SetScale(700.0f, 930.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_out_R);

	//モード選択場所の内背景
	m_mode_in_L = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_in_L->SetTexture("assets/texture/ui/mode_in.png");
	m_mode_in_L->SetPosition(-350.0f, -30.0f, 0.0f);
	m_mode_in_L->SetScale(600.0f, 750.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_in_L);

	//モード選択場所の内背景
	m_mode_in_R = Game::GetInstance()->AddObject<Texture2D>();
	m_mode_in_R->SetTexture("assets/texture/ui/mode_in.png");
	m_mode_in_R->SetPosition(350.0f, -30.0f, 0.0f);
	m_mode_in_R->SetScale(600.0f, 750.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_mode_in_R);
	
	

	//モード選択の説明の文字
	m_modesentaku= Game::GetInstance()->AddObject<Texture2D>();
	m_modesentaku->SetTexture("assets/texture/ui/modesentaku.png");
	m_modesentaku->SetPosition(0.0f, -490.0f, 0.0f);
	m_modesentaku->SetScale(650.0f, 130.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_modesentaku);

	//モード選択の文字枠
	m_modesentakugamen_waku = Game::GetInstance()->AddObject<Texture2D>();
	m_modesentakugamen_waku->SetTexture("assets/texture/ui/modesentakugamen_waku.png");
	m_modesentakugamen_waku->SetPosition(-700.0f, 450.0f, 0.0f);
	m_modesentakugamen_waku->SetScale(450.0f, 150.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_modesentakugamen_waku);

	//モード選択の文字
	m_modesentakugamen = Game::GetInstance()->AddObject<Texture2D>();
	m_modesentakugamen->SetTexture("assets/texture/ui/modesentakugamen.png");
	m_modesentakugamen->SetPosition(-700.0f, 450.0f, 0.0f);
	m_modesentakugamen->SetScale(400.0f, 100.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_modesentakugamen);

	
	// ストーリーボタン 
	m_StoryBtn = Game::GetInstance()->AddObject<Texture2D>();
	m_StoryBtn->SetTexture("assets/texture/ui/storymode.png");
	m_StoryBtn->SetPosition(-350.0f, 280.0f, 0.0f);
	m_StoryBtn->SetScale(400.0f, 150.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_StoryBtn);

	// エンドレスボタン
	m_EndlessBtn = Game::GetInstance()->AddObject<Texture2D>();
	m_EndlessBtn->SetTexture("assets/texture/ui/endlessmode.png");
	m_EndlessBtn->SetPosition(350.0f, 280.0f, 0.0f);
	m_EndlessBtn->SetScale(400.0f, 150.0f, 0.0f);
	m_MySceneObjects.emplace_back(m_EndlessBtn);
	
	m_FadePanel = Game::GetInstance()->AddObject<Texture2D>();
	m_FadePanel->SetTexture("assets/texture/terrain.png");
	m_FadePanel->SetPosition(0.0f, 0.0f, 0.0f);
	m_FadePanel->SetScale(2000.0f, 2000.0f, 0.0f);
	m_FadePanel->SetAlpha(m_fadeAlpha); // 最初は黒
	m_MySceneObjects.emplace_back(m_FadePanel);
}

void ModeSelectScene::Update()
{
	// --- フェードのタイマー ---
	const float FADE_TIME = 0.2f;// 0.5秒で終了
	float Time = 1.0f / 60.0f;

	if (m_isStarting)
	{
		m_fadeAlpha += Time / FADE_TIME;
	}
	else
	{
		m_fadeAlpha -= Time / FADE_TIME;
	}

	if (m_fadeAlpha > 1.0f)
	{
		m_fadeAlpha = 1.0f;
	}
	
	if (m_fadeAlpha < 0.0f)
	{
		m_fadeAlpha = 0.0f;
	}
	//アルファ値を反映
	if (m_FadePanel) m_FadePanel->SetAlpha(m_fadeAlpha);

	// シーン切り替え判定
	if (m_isStarting && m_fadeAlpha >= 1.0f)
	{
		if (m_NextSceneID == 1) Game::GetInstance()->ChangeScene(SceneName::START);
		else if (m_NextSceneID == 2) Game::GetInstance()->ChangeScene(SceneName::STAGE_SELECT);
		else if (m_NextSceneID == 3) Game::GetInstance()->ChangeScene(SceneName::STAGE10);
		return;
	}

	if (m_isStarting) return;
	/*
	float targetAlpha;
	if (m_isStarting)
	{
		targetAlpha = 1.0f; // フェードアウト中：真っ黒へ
	}
	else
	{
		targetAlpha = 0.0f; // フェードイン中：透明へ
	}

	const float fadeSpeed = 0.09f;

	// 滑らかに
	m_fadeAlpha += (targetAlpha - m_fadeAlpha) * fadeSpeed;

	if (m_FadePanel)
	{
		m_FadePanel->SetAlpha(m_fadeAlpha);
	}
	*/

	//クリアしたか
	bool isAllCleared = (Game::GetInstance()->GetMaxClearedStage() >= 9);

	// --- 真っ黒になったらシーン切り替え ---
	if (m_isStarting && m_fadeAlpha > 0.99f)
	{
		if (m_NextSceneID == 1)
		{
			Game::GetInstance()->ChangeScene(SceneName::START);
		}
		else if (m_NextSceneID == 2)
		{
			Game::GetInstance()->ChangeScene(SceneName::STAGE_SELECT);
		}
		else if (m_NextSceneID == 3)
		{
			Game::GetInstance()->ChangeScene(SceneName::STAGE10);
		}
		
		if (m_SelectIndex == 0)
		{
			Game::GetInstance()->ChangeScene(SceneName::STAGE_SELECT);
		}
		else if (isAllCleared)
		{
			Game::GetInstance()->ChangeScene(SceneName::STAGE10);
		}
		
		return;
	}

	// --- フェードアウト中は操作を無視 ---
	if (m_isStarting) return;

	
	if (!m_isStarting)
	{
		//マウス入力
		static DirectX::XMFLOAT2 lastMousePos = { 0, 0 };
		DirectX::XMFLOAT2 currentMousePos = Input::GetMousePosition();

		bool mouseMoved = (currentMousePos.x != lastMousePos.x || currentMousePos.y != lastMousePos.y);
		lastMousePos = currentMousePos;



		DirectX::XMFLOAT2 stick = Input::GetLeftAnalogStick();

		if (Input::GetButtonTrigger(XINPUT_LEFT) || Input::GetKeyTrigger(VK_LEFT) || stick.x < -0.5f)
		{
			Game::GetSound()->Play(SOUND_LABEL_SE_000);
			m_SelectIndex = 0; // ストーリーモード
		}


		if (Input::GetButtonTrigger(XINPUT_RIGHT) || Input::GetKeyTrigger(VK_RIGHT) || stick.x > 0.5f)
		{
			Game::GetSound()->Play(SOUND_LABEL_SE_000);
			m_SelectIndex = 1; // エンドレスモード
		}


		if (mouseMoved)
		{
			if (m_mode_in_L && IsMouseOver(m_mode_in_L)) { m_SelectIndex = 0; }
			if (m_mode_in_R && IsMouseOver(m_mode_in_R)) { m_SelectIndex = 1; }

		}

		float targetStory; //目標サイズ

		if (m_SelectIndex == 0)
		{
			targetStory = 440.0f; // 選択中なら大きく
		}
		else
		{
			targetStory = 400.0f; // 選択してないなら普通
		}

		float targetEndless; // 目標サイズ

		if (m_SelectIndex == 1)
		{
			targetEndless = 440.0f; // 選択中なら大きく
		}
		else
		{
			targetEndless = 400.0f; // 選択してないなら普通
		}

		//滑らかに
		const float speed = 0.5f;
		m_curStoryScale += (targetStory - m_curStoryScale) * speed;
		m_curEndlessScale += (targetEndless - m_curEndlessScale) * speed;

		//サイズを反映
		if (m_StoryBtn && m_EndlessBtn)
		{
			// 左側（ストーリー）一式を現在のスケールで更新
			m_StoryBtn->SetScale(m_curStoryScale, m_curStoryScale * 0.375f, 0.0f);
			m_mode_in_L->SetScale(m_curStoryScale * 1.38f, m_curStoryScale * 1.72f, 0.0f);
			m_mode_out_L->SetScale(m_curStoryScale * 1.68f, m_curStoryScale * 2.13f, 0.0f);

			// 右側（エンドレス）一式を現在のスケールで更新
			m_EndlessBtn->SetScale(m_curEndlessScale, m_curEndlessScale * 0.375f, 0.0f);
			m_mode_in_R->SetScale(m_curEndlessScale * 1.38f, m_curEndlessScale * 1.72f, 0.0f);
			m_mode_out_R->SetScale(m_curEndlessScale * 1.68f, m_curEndlessScale * 2.13f, 0.0f);

		}
	}
	
	bool isMouseClickOnButton = (Input::GetMouseButtonTrigger(0) && (IsMouseOver(m_mode_in_L) || IsMouseOver(m_mode_in_R)));

	// --- 決定処理 (Aボタン or 左クリック or Enterキー) ---
	if (Input::GetButtonTrigger(XINPUT_A) ||
		Input::GetKeyTrigger(VK_RETURN) ||
		isMouseClickOnButton)

	{
		if (m_SelectIndex == 0)
		{
			Game::GetSound()->Play(SOUND_LABEL_SE_010);
			m_NextSceneID = 2;//ステージセレクトへ
			m_isStarting = true;
			//Game::GetInstance()->ChangeScene(SceneName::STAGE_SELECT);
		}
		else if (isAllCleared)
		{
			Game::GetSound()->Play(SOUND_LABEL_SE_010);
			m_NextSceneID = 3;//エンドレスへ
			m_isStarting = true;
			//Game::GetInstance()->ChangeScene(SceneName::STAGE10);
		}
		return;
	}

	//一個戻る
	if (Input::GetButtonTrigger(XINPUT_B) ||
		Input::GetKeyTrigger(VK_SHIFT))
	{
		Game::GetSound()->Play(SOUND_LABEL_SE_000);
		m_NextSceneID = 1;//スタートシーンへ
		m_isStarting = true;
		//Game::GetInstance()->ChangeScene(SceneName::START);
		return;
	}



	if (isAllCleared)
	{
		// エンドレス解放
		m_mode_in_R->SetBrightness(1.0f); // 明るく
		m_mode_out_R->SetBrightness(1.0f);
		m_EndlessBtn->SetBrightness(1.0f);

	}
	else
	{
		// まだ制限中
		m_mode_in_R->SetBrightness(0.3f); // 暗い
		m_mode_out_R->SetBrightness(0.3f);
		m_EndlessBtn->SetBrightness(0.3f);
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