#include "StartScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"
#include "SelectBG.h"

// コンストラクタ
StartScene::StartScene()
{
}

// デストラクタ
StartScene::~StartScene()
{
}

void StartScene::Init()
{
    // 背景の追加
    auto* bg = Game::GetInstance()->AddObject<SelectBG>();
    m_MySceneObjects.push_back(bg);

    m_Title = Game::GetInstance()->AddObject<Texture2D>();
    m_Title->SetTexture("assets/texture/ui/title.png");
    m_Title->SetPosition(0.0f, 250.0f, 0.0f);
    m_Title->SetScale(1000.0f, 400.0f, 0.0f);
    m_MySceneObjects.emplace_back(m_Title);

    m_Title_hurigana = Game::GetInstance()->AddObject<Texture2D>();
    m_Title_hurigana->SetTexture("assets/texture/ui/title_hurigana.png");
    m_Title_hurigana->SetPosition(0.0f, 0.0f, 0.0f);
    m_Title_hurigana->SetScale(600.0f, 200.0f, 0.0f);
    m_MySceneObjects.emplace_back(m_Title_hurigana);

    m_PressEnterwakuImg= Game::GetInstance()->AddObject<Texture2D>();
    m_PressEnterwakuImg->SetTexture("assets/texture/ui/kaisi_waku.png");
    m_PressEnterwakuImg->SetPosition(0.0f, -250.0f, 0.0f);
    m_PressEnterwakuImg->SetScale(600.0f, 100.0f, 0.0f);
    m_MySceneObjects.emplace_back(m_PressEnterwakuImg);

    m_PressEnterImg = Game::GetInstance()->AddObject<Texture2D>();
    m_PressEnterImg->SetTexture("assets/texture/ui/kaisi.png");
    m_PressEnterImg->SetPosition(0.0f, -250.0f, 0.0f);
    m_PressEnterImg->SetScale(600.0f, 100.0f, 0.0f);
    m_MySceneObjects.emplace_back(m_PressEnterImg);

    /*
    m_FadePanel = Game::GetInstance()->AddObject<Texture2D>();
    m_FadePanel->SetTexture("assets/texture/terrain.png"); 
    m_FadePanel->SetPosition(0.0f, 0.0f, 0.0f);            
    m_FadePanel->SetScale(2000.0f, 2000.0f, 0.0f);         
    m_FadePanel->SetAlpha(1.0f);                           // 最初は黒
    m_MySceneObjects.emplace_back(m_FadePanel);
    */

    // 左の障子
    m_FadeL = Game::GetInstance()->AddObject<Texture2D>();
    m_FadeL->SetTexture("assets/texture/Cloud.png"); // 障子用の画像があれば差し替え
    m_FadeL->SetScale(960.0f, 450.0f, 0.0f);


    // 左の障子
    m_FadeLL = Game::GetInstance()->AddObject<Texture2D>();
    m_FadeLL->SetTexture("assets/texture/Cloud.png"); // 障子用の画像があれば差し替え
    m_FadeLL->SetScale(960.0f, 450.0f, 0.0f);

    // 右の障子
    m_FadeR = Game::GetInstance()->AddObject<Texture2D>();
    m_FadeR->SetTexture("assets/texture/Cloud.png");
    m_FadeR->SetScale(960.0f, 450.0f, 0.0f);


    // 右の障子
    m_FadeRR = Game::GetInstance()->AddObject<Texture2D>();
    m_FadeRR->SetTexture("assets/texture/Cloud.png");
    m_FadeRR->SetScale(960.0f, 450.0f, 0.0f);

    // 最初はフェードイン（障子が開く状態）から始めたいのでタイマーを1.0(全閉)に
    m_shojiTimer = 1.0f;
    m_isStarting = false;

    //開始ボイス用の乱数
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<int> dist(1, 2);

    int v = dist(rng);

    if (v == 1)
    {
		Game::GetSound()->Play(SOUND_LABEL_VOICE_ANOTHER_007);
	}
    else if (v == 2)
    {
		Game::GetSound()->Play(SOUND_LABEL_VOICE_ANOTHER_008);
	}
}

void StartScene::Update()
{
    // --- 障子フェード処理 ---
    const float FADE_DURATION = 1.0f; // 0.5秒で閉まる
    float deltaTime = 1.0f / 60.0f;

    if (m_isStarting)
    {
        m_shojiTimer += deltaTime / FADE_DURATION;
    }
    else
    {
        m_shojiTimer -= deltaTime / FADE_DURATION;
    }

    // 0.0 ～ 1.0
    if (m_shojiTimer > 1.0f) m_shojiTimer = 1.0f;
    if (m_shojiTimer < 0.0f) m_shojiTimer = 0.0f;

    // --- 座標の計算 ---
    // 全開(0.0)のとき左右に隠れる、全閉(1.0)のとき真ん中で合わさる
    // 中心を0として、左障子は -960(開) ⇔ -460(閉)
    // 中心を0として、右障子は  960(開) ⇔  460(閉)
    float openX = 960.0f;  // 画面の外
    float closeX = 460.0f; // 画面の半分

    // 線形補間(Lerp)で現在のX座標を出す
    float timer1 = m_shojiTimer;
    float currentX = openX + (closeX - openX) * timer1;

    float timer2 = m_shojiTimer;
    float currentXX = openX + (closeX - openX) * timer2;


    if (m_FadeL && m_FadeLL && m_FadeR && m_FadeRR)
    {
        m_FadeL->SetPosition(-currentX, -200.0f, 0.0f);
        m_FadeR->SetPosition(currentX, 0.0f, 0.0f);
        m_FadeLL->SetPosition(-currentXX, 200.0f, 0.0f);
        m_FadeRR->SetPosition(currentXX, -400.0f, 0.0f);
       
         // アルファも連動
        float alpha = m_shojiTimer;

        m_FadeL->SetAlpha(alpha);
        m_FadeRR->SetAlpha(alpha);

        
        m_FadeLL->SetAlpha(alpha * 0.8f);
        m_FadeR->SetAlpha(alpha * 0.8f);
    }

    // シーン切り替え判定（タイマーが1.0になったら）
    if (m_isStarting && m_shojiTimer >= 1.0f)
    {
        Game::GetInstance()->ChangeScene(SceneName::MODE_SELECT);
        return;
    }

    if (m_isStarting) return; // 閉まりかけの時は入力を受け付けない
    

    // --- 入力の取得 ---
    static DirectX::XMFLOAT2 lastMousePos = { 0, 0 };
    DirectX::XMFLOAT2 currentMousePos = Input::GetMousePosition();

    bool mouseMoved = (currentMousePos.x != lastMousePos.x || currentMousePos.y != lastMousePos.y);
    lastMousePos = currentMousePos;

    static bool isSelected = false;
    //bool prevSelected = isSelected; // 前フレームの選択状態を保存

    DirectX::XMFLOAT2 stick = Input::GetLeftAnalogStick();

    // --- 選択状態の切り替え ---
    // 上入力でOFF（isSelected = false）
    
    if (Input::GetButtonTrigger(XINPUT_UP) || Input::GetKeyTrigger(VK_UP) || stick.y > 0.5f)
    {
        isSelected = false;
    }

    // 下入力でON（isSelected = true）
    if (Input::GetButtonTrigger(XINPUT_DOWN) || Input::GetKeyTrigger(VK_DOWN) || stick.y < -0.5f)
    {
        isSelected = true;
        Game::GetSound()->Play(SOUND_LABEL_SE_000);
    }
    

    // マウス
    if (mouseMoved)
    {
        isSelected = IsMouseOver(m_PressEnterwakuImg);
    }

    float targetStart;
    if (isSelected == true) 
    {
        targetStart = 540.0f; // 選択中なら大きく
    }
    else 
    {
        targetStart = 500.0f; // 選択してないなら普通
    }

    //滑らかに
    const float speed = 0.5f;
    m_curWakuScale += (targetStart - m_curWakuScale) * speed;

    // サイズを反映
    if (m_PressEnterwakuImg && m_PressEnterImg)
    {
        // 枠のサイズ
        m_PressEnterwakuImg->SetScale(m_curWakuScale, m_curWakuScale * 0.42f, 0.0f); 

        // 中の文字
        m_PressEnterImg->SetScale(m_curWakuScale * 0.42f, m_curWakuScale * 0.22f, 0.0f);
    }

    // ---決定処理 ---
    bool isMouseClickOnButton = (Input::GetMouseButtonTrigger(0) && IsMouseOver(m_PressEnterwakuImg));
    bool isEnterPressed = Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A);
    
    if (isMouseClickOnButton || isEnterPressed)
    {
        m_isStarting = true;
        Game::GetSound()->Play(SOUND_LABEL_SE_010);
        // Enterで決定した時も強制的にボタンを大きく
        isSelected = true;

        //SE
    }

  
}

void StartScene::Uninit()
{
    for (auto& o : m_MySceneObjects) 
    {
        Game::GetInstance()->DeleteObject(o);
    }
    m_MySceneObjects.clear();
}

bool StartScene::IsMouseOver(Texture2D* obj)
{
    if (!obj) return false;

    DirectX::XMFLOAT2 mouse = Input::GetMousePosition();
    DirectX::SimpleMath::Vector3 pos = obj->GetPosition();
    DirectX::SimpleMath::Vector3 scale = obj->GetScale();

    float left = pos.x - (scale.x / 2.0f);
    float right = pos.x + (scale.x / 2.0f);
    float top = pos.y + (scale.y / 2.0f);
    float bottom = pos.y - (scale.y / 2.0f);

    return (mouse.x >= left && mouse.x <= right && mouse.y >= bottom && mouse.y <= top);
}