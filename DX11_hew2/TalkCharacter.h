#pragma once
#include"Object.h"
#include <string>
#include "Texture2D.h"

// 立ち絵の表示位置（左右）
enum class TalkSide
{
    Left,
    Right
};

class TalkCharacter : public Object
{
private:
    // どちら側に表示するか
    TalkSide m_Side = TalkSide::Left;

    // キャラ識別・表情識別（画像パス生成に使う）
    std::string m_CharId;   // 例: "miko"
    std::string m_FaceId;   // 例: "normal"

    // 表示状態
    bool m_Visible = false;
    bool m_Focus = true;    // 話者ならtrue（明るい）、非話者ならfalse（暗く）

    // 実装側で使う想定の描画用値
    float m_Alpha = 1.0f;

    // 表示用スプライト
    Texture2D m_Sprite;

    // 仮のレイアウト
    DirectX::SimpleMath::Vector3 m_BasePosL = DirectX::SimpleMath::Vector3(-500.0f, -50.0f, 0.0f);
    DirectX::SimpleMath::Vector3 m_BasePosR = DirectX::SimpleMath::Vector3(500.0f, -50.0f, 0.0f);

    DirectX::SimpleMath::Vector3 m_Size = DirectX::SimpleMath::Vector3(700.0f, 900.0f, 1.0f);

    // 最後に適用したパス（無駄なLoad回避）
    std::string m_LastAppliedPath;


private:
    // charId/faceId から画像パスを作る
    std::string BuildTexturePath() const;

    // 今の状態をm_Spriteに反映（位置、左右反転など）
    void ApplyLayoutToSprite();

    // 必要な時だけSetTextureする
    void ApplyTextureIfNeeded();

public:
    // Object
    void Init() override;
    void Update() override;
    void Draw(Camera* cam) override;
    void Uninit() override;

public:
    // 外部操作API（MessageManagerから呼ぶ）
    void Show(bool v) { m_Visible = v; }
    bool IsVisible() const { return m_Visible; }

    void SetSide(TalkSide side) { m_Side = side; }
    TalkSide GetSide() const { return m_Side; }


    // cpp実装
    void SetFocus(bool focus);
    bool IsFocus() const { return m_Focus; }

    // キャラ/表情
    void SetCharacter(const std::string& charId);
    void SetFace(const std::string& faceId);

    // まとめて設定
    void Set(const std::string& charId, const std::string& faceId);

    const std::string& GetCharacter() const { return m_CharId; }
    const std::string& GetFace() const { return m_FaceId; }

    // 仮レイアウト調整用（後で好きに触れる）
    void SetBasePosLeft(const DirectX::SimpleMath::Vector3& p) { m_BasePosL = p; }
    void SetBasePosRight(const DirectX::SimpleMath::Vector3& p) { m_BasePosR = p; }
    void SetSize(const DirectX::SimpleMath::Vector3& s) { m_Size = s; }

    void SetDummyTexture(const char* path) { m_Sprite.SetTexture(path); }
};
