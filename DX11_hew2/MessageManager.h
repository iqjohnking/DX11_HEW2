#pragma once
#include <vector>
#include <string>
#include "Object.h"

#include "MessagePage.h"
#include "MessageUI.h"
#include "Text.h"
#include "TalkCharacter.h"
#include "Sound.h"

//会話全体の管理
class MessageManager : public Object
{
private:
    MessageUI* m_UI = nullptr;
    Text* m_Text = nullptr;
    TalkCharacter* m_LeftChar = nullptr;
    TalkCharacter* m_RightChar = nullptr;

    // 仮素材パス
    std::string m_FramePath;
    std::string m_TextDummyPath;
    std::string m_CharaDummyPath;

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
    void Init() override;
    void Update() override;
    void Draw(Camera* cam) override;   // 描画しない
    void Uninit() override;

public:
    // 外部API（Sceneなどから操作）
    void SetPages(const std::vector<MessagePage>& pages) { m_Pages = pages; }
    void ClearPages() { m_Pages.clear(); }

    void SetFramePath(const std::string& path);
    void SetTextDummyPath(const std::string& path);
    void SetCharaDummyPath(const std::string& path);

    void Play();     // 会話開始（index=0）
    void Advance();  // 次へ
    void Stop();     // 終了

    bool IsPlaying() const { return m_Playing; }
    int  GetIndex() const { return m_Index; }
    int  GetPageCount() const { return static_cast<int>(m_Pages.size()); }
	
};

