#include "MessageManager.h"
#include "input.h"
#include "Game.h"

void MessageManager::Init()
{
    // UI部品の初期化
    m_UI = Game::GetInstance()->AddObject<MessageUI>();
    m_Text = Game::GetInstance()->AddObject<Text>();
    m_LeftChar = Game::GetInstance()->AddObject<TalkCharacter>();
    m_RightChar = Game::GetInstance()->AddObject<TalkCharacter>();

    // 描画順（必要なら調整）
    m_LeftChar->SetDrawOrder(900);
    m_RightChar->SetDrawOrder(900);
    m_UI->SetDrawOrder(1000);
    m_Text->SetDrawOrder(1100);

    // 左右設定
    m_LeftChar->SetSide(TalkSide::Left);
    m_RightChar->SetSide(TalkSide::Right);


    // 非表示で開始
    m_UI->Show(false);
    m_Text->Show(false);
    m_LeftChar->Show(false);
    m_RightChar->Show(false);

    m_Playing = false;
    m_Index = 0;
}

void MessageManager::Uninit()
{
    Stop();
}

void MessageManager::Draw(Camera* cam)
{
   //各部品が勝手に描画するので何もなし
}

void MessageManager::Update()
{
    if (!m_Playing) return;

    // 仮：Spaceで次へ
    if (Input::GetKeyTrigger(VK_SPACE))
    {
        Advance();
    }
}

void MessageManager::SetFramePath(const std::string& path)
{
    m_FramePath = path;
    if (m_UI) m_UI->SetFrame(m_FramePath.c_str());
}

void MessageManager::SetTextDummyPath(const std::string& path)
{
    m_TextDummyPath = path;
    if (m_Text) m_Text->SetDummyTexture(path.c_str());
}

void MessageManager::SetCharaDummyPath(const std::string& path)
{
    m_CharaDummyPath = path;

    if (m_LeftChar)  m_LeftChar->SetDummyTexture(path.c_str());
    if (m_RightChar) m_RightChar->SetDummyTexture(path.c_str());
}

void MessageManager::Play()
{
    if (m_Pages.empty())
        return;

    m_Playing = true;
    m_Index = 0;

    m_UI->Show(true);
    m_Text->Show(true);
    m_LeftChar->Show(true);
    m_RightChar->Show(true);

    BeginPage(m_Index);
}

void MessageManager::Stop()
{
    if (!m_Playing) return;

    StopCurrentVoice();

    m_Playing = false;
    m_Index = 0;

    m_UI->Show(false);
    m_Text->Show(false);
    m_LeftChar->Show(false);
    m_RightChar->Show(false);
}

void MessageManager::Advance()
{
    if (!m_Playing) return;

    StopCurrentVoice();

    m_Index++;
    if (m_Index >= (int)m_Pages.size())
    {
        Stop();
        return;
    }

    BeginPage(m_Index);
}

void MessageManager::BeginPage(int index)
{
    if (index < 0 || index >= (int)m_Pages.size()) return;

    const MessagePage& p = m_Pages[index];

    // 文字
    m_Text->SetName(p.speakerName);
    m_Text->SetText(p.text);

    // フォーカス
    if (p.focus == FocusSide::Left)
    {
        m_LeftChar->SetFocus(true);
        m_RightChar->SetFocus(false);
    }
    else if (p.focus == FocusSide::Right)
    {
        m_LeftChar->SetFocus(false);
        m_RightChar->SetFocus(true);
    }
    else
    {
        m_LeftChar->SetFocus(true);
        m_RightChar->SetFocus(true);
    }
}

void MessageManager::StopCurrentVoice()
{
    // m_Sound.StopVoice();
}

