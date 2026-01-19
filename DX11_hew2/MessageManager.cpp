#include "MessageManager.h"
#include "input.h"

void MessageManager::Init()
{
    // UI部品の初期化
    m_UI.Init();
    m_Text.Init();

    m_UI.SetFrame("assets/texture/UI/field.jpg");

    m_LeftChar.Init();
    m_RightChar.Init();

    // 左右の配置を固定
    m_LeftChar.SetSide(TalkSide::Left);
    m_RightChar.SetSide(TalkSide::Right);

    // 左右に出すキャラID固定（ミニ版運用）
    m_LeftChar.SetCharacter("miko");
    m_RightChar.SetCharacter("kumo");

    // 表情も固定（素材が無いなら貼らなくてもOK）
    m_LeftChar.SetFace("normal");
    m_RightChar.SetFace("normal");

    // 最初は非表示
    m_UI.Show(false);
    m_Text.Show(false);
    m_LeftChar.Show(false);
    m_RightChar.Show(false);

    m_Playing = false;
    m_Index = 0;
}

void MessageManager::Uninit()
{
    Stop();

    m_RightChar.Uninit();
    m_LeftChar.Uninit();
    m_Text.Uninit();
    m_UI.Uninit();
}

void MessageManager::Draw(Camera* cam)
{
    // 会話中だけ描く（ゲーム画面はScene側が先に描く想定）
    if (!m_Playing) return;

    // 立ち絵 → 枠 → 文字
    m_LeftChar.Draw(cam);
    m_RightChar.Draw(cam);
    m_UI.Draw(cam);
    m_Text.Draw(cam);
}

void MessageManager::Update()
{
    if (!m_Playing) return;

    // 仮：Spaceで次へ
    if (Input::GetKeyTrigger(VK_SPACE))
    {
        Advance();
    }

    // 部品側Update（今はほぼ空でもOK）
    m_UI.Update();
    m_Text.Update();
    m_LeftChar.Update();
    m_RightChar.Update();
}

void MessageManager::Play()
{
    if (m_Pages.empty())
        return;

    m_Playing = true;
    m_Index = 0;

    m_UI.Show(true);
    m_Text.Show(true);
    m_LeftChar.Show(true);
    m_RightChar.Show(true);

    BeginPage(m_Index);
}

void MessageManager::Stop()
{
    if (!m_Playing) return;

    StopCurrentVoice();

    m_Playing = false;
    m_Index = 0;

    m_UI.Show(false);
    m_Text.Show(false);
    m_LeftChar.Show(false);
    m_RightChar.Show(false);
}

void MessageManager::Advance()
{
    if (!m_Playing) return;

    StopCurrentVoice();

    m_Index++;

    if (m_Index >= static_cast<int>(m_Pages.size()))
    {
        Stop();
        return;
    }

    BeginPage(m_Index);
}

void MessageManager::BeginPage(int index)
{
    if (index < 0 || index >= static_cast<int>(m_Pages.size()))
        return;

    const MessagePage& page = m_Pages[index];

    // 文字反映
    m_Text.SetName(page.speakerName);
    m_Text.SetText(page.text);

    // フォーカス反映（ミニ版の核）
    if (page.focus == FocusSide::Left)
    {
        m_LeftChar.SetFocus(true);
        m_RightChar.SetFocus(false);
    }
    else if (page.focus == FocusSide::Right)
    {
        m_LeftChar.SetFocus(false);
        m_RightChar.SetFocus(true);
    }
    else
    {
        // Noneは両方明るい（おすすめ）
        m_LeftChar.SetFocus(true);
        m_RightChar.SetFocus(true);
    }

    // ---- ボイス再生（Sound APIが未確定なので保留）----
    // TODO: あなたのSoundクラスの関数名に合わせてここだけ直す
    // if (!page.voiceId.empty()) { m_Sound.PlayVoice(page.voiceId.c_str()); }
}


void MessageManager::StopCurrentVoice()
{
    // m_Sound.StopVoice();
}

