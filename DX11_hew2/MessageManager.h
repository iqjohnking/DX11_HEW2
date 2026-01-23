#pragma once
#include <vector>
#include <string>
#include "Object.h"

#include "MessagePage.h"
#include "MessageUI.h"
#include "MessageText.h"
#include "TalkCharacter.h"
#include "Sound.h"

//会話全体の管理
class MessageManager : public Object
{
private:
    MessageUI* m_Background = nullptr;
    MessageUI* m_UI = nullptr;
    MessageText* m_Text = nullptr;
    TalkCharacter* m_LeftChar = nullptr;
    TalkCharacter* m_RightChar = nullptr;

	// 表示キャラID
    std::string m_LeftCharId;
    std::string m_RightCharId;

    // 現在再生中のボイス
    SOUND_LABEL m_CurrentVoice = SOUND_LABEL_MAX;

    // 台本
    std::vector<MessagePage> m_Pages;

    // UIパス
    std::string m_FramePath; 

    // 背景パス
    std::string m_BackgroundPath;

private:
    // 進行状態
    bool m_Playing = false;
    int  m_Index = 0;

    // 1ページ分を各部品に反映（心臓部）
    void BeginPage(int index);

    // ボイス停止（クリックで次へ進む瞬間に必ず呼ぶ）
    void StopCurrentVoice();

    // 部品が未生成なら生成して初期設定まで行う(保険)
    void CreatePartsIfNeeded();

    // 部品に共通設定を適用
    void SetupParts();

    // 部品を非表示にする
    void CleanupParts();

    std::string BuildNameImagePath(const std::string& nameId) const;
    std::string BuildTextImagePath(const std::string& textId, int index) const;

    //台本のチェック
    bool ValidatePagesForPlay() const;

public:
    void Init() override;
    void Update() override;
    void Draw(Camera* cam) override;   // 描画しない
    void Uninit() override;

public:
    // 外部API（Sceneなどから操作）
    void SetPages(const std::vector<MessagePage>& pages) { m_Pages = pages; }
    void ClearPages() { m_Pages.clear(); }

    //エッセージUI設定
    void SetFramePath(const std::string& path);

    // メッセージ背景設定
    void SetBackgroundPath(const std::string& path);

    // 左右参加者を固定（Play前に呼ぶ）
    void SetParticipants(const std::string& leftCharId, const std::string& rightCharId);

    void Play();     // 会話開始（index=0）
    void Advance();  // 次へ
    void Stop();     // 終了


    bool IsPlaying() const { return m_Playing; }
    int  GetIndex() const { return m_Index; }
    int  GetPageCount() const { return static_cast<int>(m_Pages.size()); }
};

