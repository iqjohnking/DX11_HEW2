#include "StartScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"
#include "TitleBG.h"

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
    auto* bg = Game::GetInstance()->AddObject<TitleBG>();
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
   
    if (IsMouseOver(m_PressEnterwakuImg))
    {
        m_PressEnterwakuImg->SetScale(510.0f, 210.0f, 0.0f);
        m_PressEnterImg->SetScale(210.0f, 110.0f, 0.0f); // ホバーで少し大きく
        if (Input::GetMouseButtonTrigger(0))
        {
            Game::GetInstance()->ChangeScene(MODE_SELECT);
            return; // シーン遷移後は即終了
        }
    }
    else
    {
        m_PressEnterwakuImg->SetScale(500.0f, 200.0f, 0.0f); // 通常サイズ
        m_PressEnterImg->SetScale(200.0f, 100.0f, 0.0f); // 通常サイズ
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