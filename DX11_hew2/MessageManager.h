#pragma once
#include <vector>
#include <string>

#include "MessagePage.h"
#include "MessageUI.h"
#include "Text.h"
#include "TalkCharacter.h"
#include "Sound.h"

//会話全体の管理
class MessageManager
{
private:
    MessageUI      m_UI;
    Text           m_Text;

    // 左右の立ち絵（1体ずつ）
    TalkCharacter  m_LeftChar;
    TalkCharacter  m_RightChar;

    // 音
    Sound m_Sound;

    // 台本
    std::vector<MessagePage> m_Pages;

    // 進行状態
    bool m_Playing = false;
    int  m_Index = 0;

private:
    // 1ページ分を各部品に反映（心臓部）
    void BeginPage(int index);

    // ボイス停止（クリックで次へ進む瞬間に必ず呼ぶ）
    void StopCurrentVoice();

public:
    void Init();
    void Update();   // 入力を見てAdvanceする
    void Draw(Camera* cam);
    void Uninit();

public:
    // 外部API（Sceneなどから操作）
    void SetPages(const std::vector<MessagePage>& pages) { m_Pages = pages; }
    void ClearPages() { m_Pages.clear(); }

    void Play();     // 会話開始（index=0）
    void Advance();  // 次へ
    void Stop();     // 終了

    bool IsPlaying() const { return m_Playing; }
    int  GetIndex() const { return m_Index; }
    int  GetPageCount() const { return static_cast<int>(m_Pages.size()); }
	
};

