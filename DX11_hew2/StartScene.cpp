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
}

void StartScene::Update()
{
    // --- 1. 入力の取得 ---
    static DirectX::XMFLOAT2 lastMousePos = { 0, 0 };
    DirectX::XMFLOAT2 currentMousePos = Input::GetMousePosition();

    bool mouseMoved = (currentMousePos.x != lastMousePos.x || currentMousePos.y != lastMousePos.y);
    lastMousePos = currentMousePos;

    static bool isSelected = false;
    bool prevSelected = isSelected; // 前フレームの選択状態を保存

    DirectX::XMFLOAT2 stick = Input::GetLeftAnalogStick();

    // --- 2. 選択状態の切り替え ---
    // 上入力でOFF（isSelected = false）
    if (Input::GetButtonTrigger(XINPUT_UP) || Input::GetKeyTrigger(VK_UP) || stick.y > 0.5f)
    {
        isSelected = false;
    }

    // 下入力でON（isSelected = true）
    if (Input::GetButtonTrigger(XINPUT_DOWN) || Input::GetKeyTrigger(VK_DOWN) || stick.y < -0.5f)
    {
        isSelected = true;
    }

    // マウス移動時はマウスの位置を優先
    if (mouseMoved)
    {
        isSelected = IsMouseOver(m_PressEnterwakuImg);
    }

    // --- 3. 選択音が変わった瞬間にSE再生 ---
    if (isSelected != prevSelected)
    {
        // 選択肢が切り替わった時のSE (ラベルは適切なものに変更してください)
        // Game::GetInstance()->GetSound()->Play(SOUND_LABEL_VOICE_STAGE0_START_001); 
    }

    // --- 4. 見た目の反映 (スケール変更) ---
    if (isSelected)
    {
        m_PressEnterwakuImg->SetScale(510.0f, 210.0f, 0.0f);
        m_PressEnterImg->SetScale(210.0f, 110.0f, 0.0f);
    }
    else
    {
        m_PressEnterwakuImg->SetScale(500.0f, 200.0f, 0.0f);
        m_PressEnterImg->SetScale(200.0f, 100.0f, 0.0f);
    }

    // --- 5. 決定処理 ---
    bool isMouseClickOnButton = (Input::GetMouseButtonTrigger(0) && IsMouseOver(m_PressEnterwakuImg));

    if (isMouseClickOnButton ||
        Input::GetButtonTrigger(XINPUT_A) ||
        Input::GetKeyTrigger(VK_RETURN))
    {
        // 選択されている（ボタンが大きくなっている）時だけ決定できる
        if (isSelected)
        {
            // 決定SE再生 (ラベルは適切なものに変更してください)
            // Game::GetInstance()->GetSound()->Play(SOUND_LABEL_VOICE_STAGE0_START_003); 

            Game::GetInstance()->ChangeScene(MODE_SELECT);
            return;
        }
    }

    /*
    static DirectX::XMFLOAT2 lastMousePos = { 0, 0 };
    DirectX::XMFLOAT2 currentMousePos = Input::GetMousePosition();

    bool mouseMoved = (currentMousePos.x != lastMousePos.x || currentMousePos.y != lastMousePos.y);
    lastMousePos = currentMousePos;

    static bool isSelected = false;
    bool prevSelected = isSelected;  

    DirectX::XMFLOAT2 stick = Input::GetLeftAnalogStick();

    if (Input::GetButtonTrigger(XINPUT_UP) || Input::GetKeyTrigger(VK_UP) ||
        Input::GetLeftAnalogStick().y > 0.5f) 
    {
        isSelected = false;
    }

    if (Input::GetButtonTrigger(XINPUT_DOWN) || Input::GetKeyTrigger(VK_DOWN) ||
        Input::GetLeftAnalogStick().y < -0.5f)
    {
        isSelected = true;
    }

    if (mouseMoved)
    {
        isSelected = IsMouseOver(m_PressEnterwakuImg);
    }

    if (isSelected != prevSelected)
    {
        //SE
    }

    /*
    bool isSelected = false;
    if (IsMouseOver(m_PressEnterwakuImg))
    {
        isSelected = true;
    }

    if (isSelected)
    {
        m_PressEnterwakuImg->SetScale(510.0f, 210.0f, 0.0f);
        m_PressEnterImg->SetScale(210.0f, 110.0f, 0.0f);
    }
    else
    {
        m_PressEnterwakuImg->SetScale(500.0f, 200.0f, 0.0f);
        m_PressEnterImg->SetScale(200.0f, 100.0f, 0.0f);
    }

    if (Input::GetMouseButtonTrigger(0) && isSelected || // マウスでボタンの上をクリック
        Input::GetButtonTrigger(XINPUT_A) ||            // コントローラーAボタン
        Input::GetKeyTrigger(VK_RETURN))                // キーボードEnterキー
    {
        Game::GetInstance()->ChangeScene(MODE_SELECT);
        return; // シーン遷移後は即終了
    }
    */
   
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