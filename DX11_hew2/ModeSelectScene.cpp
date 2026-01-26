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
	// 背景の生成
	auto* bg = Game::GetInstance()->AddObject<TitleBG>();
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
	m_modesentaku->SetScale(350.0f, 100.0f, 0.0f);
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
	
}

void ModeSelectScene::Update()
{
	DirectX::XMFLOAT2 stick = Input::GetLeftAnalogStick();
	if (stick.x < -0.5f) { m_SelectIndex = 0; } // 左
	if (stick.x > 0.5f) { m_SelectIndex = 1; } // 右

	// キーボード入力 
	if (Input::GetKeyTrigger(VK_LEFT)) { m_SelectIndex = 0; }
	if (Input::GetKeyTrigger(VK_RIGHT)) { m_SelectIndex = 1; }

	// マウス入力 
	if (m_mode_in_L && IsMouseOver(m_mode_in_L)) { m_SelectIndex = 0; }
	if (m_mode_in_R && IsMouseOver(m_mode_in_R)) { m_SelectIndex = 1; }


	if (m_StoryBtn && m_EndlessBtn)
	{
		if (m_SelectIndex == 0) // ストーリー選択中
		{
			// 左側を大きく
			m_StoryBtn->SetScale(440.0f, 165.0f, 0.0f);
			m_mode_in_L->SetScale(610.0f, 760.0f, 0.0f);
			m_mode_out_L->SetScale(740.0f, 940.0f, 0.0f);
			// 右側を通常サイズ
			m_EndlessBtn->SetScale(400.0f, 150.0f, 0.0f);
			m_mode_in_R->SetScale(600.0f, 750.0f, 0.0f);
			m_mode_out_R->SetScale(730.0f, 930.0f, 0.0f);
		}
		else // エンドレス選択中
		{
			// 左側を通常サイズ
			m_StoryBtn->SetScale(400.0f, 150.0f, 0.0f);
			m_mode_in_L->SetScale(600.0f, 750.0f, 0.0f);
			m_mode_out_L->SetScale(730.0f, 930.0f, 0.0f);
			// 右側を大きく
			m_EndlessBtn->SetScale(440.0f, 165.0f, 0.0f);
			m_mode_in_R->SetScale(610.0f, 760.0f, 0.0f);
			m_mode_out_R->SetScale(740.0f, 940.0f, 0.0f);
		}
	}

	// --- 決定処理 (Aボタン or 左クリック or Enterキー) ---
	if (Input::GetButtonTrigger(XINPUT_A) ||
		Input::GetMouseButtonTrigger(0) ||
		Input::GetKeyTrigger(VK_RETURN))
	{
		if (m_SelectIndex == 0)
		{
			Game::GetInstance()->ChangeScene(STAGE_SELECT);
		}
		else
		{
			Game::GetInstance()->ChangeScene(TITLE);
		}
		return;
	}
	/*
	// --- ストーリーボタンの処理 ---
	if (m_mode_in_L) // 存在を確認
	{
		if (IsMouseOver(m_mode_in_L))
		{
			m_StoryBtn->SetScale(410.0f, 160.0f, 0.0f);
			m_mode_in_L->SetScale(610.0f, 810.0f, 0.0f);
			m_mode_out_L->SetScale(710.0f, 920.0f, 0.0f);
			if (Input::GetMouseButtonTrigger(0)) 
			{
				Game::GetInstance()->ChangeScene(STAGE_SELECT);
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
	if (m_mode_in_R) // 存在を確認
	{
		if (IsMouseOver(m_mode_in_R))
		{
			m_EndlessBtn->SetScale(410.0f, 160.0f, 0.0f);
			m_mode_in_R->SetScale(610.0f, 810.0f, 0.0f);
			m_mode_out_R->SetScale(710.0f, 920.0f, 0.0f);
			if (Input::GetMouseButtonTrigger(0)) 
			{
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

	// ---  スティック入力で選択を切り替える ---
	DirectX::XMFLOAT2 stick = Input::GetLeftAnalogStick(); 

	if (stick.x < -0.5f) { m_SelectIndex = 0; } // 左に倒したらストーリー
	if (stick.x > 0.5f) { m_SelectIndex = 1; } // 右に倒したらエンドレス

	// --- 選択状態に合わせてボタンの見た目（スケール）を変える ---
	if (m_StoryBtn && m_EndlessBtn)
	{
		if (m_SelectIndex == 0) 
		{
			m_StoryBtn->SetScale(440.0f, 165.0f, 0.0f);   // 選択中
			m_mode_in_L->SetScale(610.0f, 810.0f, 0.0f);
			m_mode_out_L->SetScale(710.0f, 920.0f, 0.0f);
			
			m_EndlessBtn->SetScale(400.0f, 150.0f, 0.0f); // 非選択
			m_mode_in_R->SetScale(600.0f, 800.0f, 0.0f);
			m_mode_out_R->SetScale(700.0f, 900.0f, 0.0f);
		}
		else 
		{
			m_StoryBtn->SetScale(400.0f, 150.0f, 0.0f);   //非選択
			m_mode_in_L->SetScale(600.0f, 800.0f, 0.0f);
			m_mode_out_L->SetScale(700.0f, 900.0f, 0.0f);

			m_EndlessBtn->SetScale(440.0f, 165.0f, 0.0f); // 選択中
			m_mode_in_R->SetScale(610.0f, 810.0f, 0.0f);
			m_mode_out_R->SetScale(710.0f, 920.0f, 0.0f);
		}
	}

	// ---  Aボタンで決定 ---
	if (Input::GetButtonTrigger(XINPUT_A)) 
	{
		if (m_SelectIndex == 0) {
			Game::GetInstance()->ChangeScene(STAGE_SELECT); // ストーリーへ
		}
		else 
		{
			Game::GetInstance()->ChangeScene(TITLE);//エンドレスへ
		}
	}
	*/
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