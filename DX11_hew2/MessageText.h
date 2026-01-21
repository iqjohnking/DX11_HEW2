#pragma once
#include "Object.h"
#include <string>
#include "Texture2D.h"

class MessageText : public Object
{
private:
   //デバック用
    std::string m_Name;   // 名前欄
    std::string m_Text;   // 本文

    // 名前欄・本文の2枚
    Texture2D m_NameSprite;
    Texture2D m_TextSprite;
    // 表示状態
	bool m_Visible = false;

    // 仮表示用//デバッグ用
    //Texture2D m_Sprite;

    // レイアウト(要調整)
    //話者
    DirectX::SimpleMath::Vector3 m_NamePos = { -650.0f, -310.0f, 0.0f };
    DirectX::SimpleMath::Vector3 m_NameSize = { 500.0f,  120.0f, 1.0f };

    //テキスト
    DirectX::SimpleMath::Vector3 m_TextPos = { 0.0f,  -420.0f, 0.0f };
    DirectX::SimpleMath::Vector3 m_TextSize = { 1800.0f, 300.0f, 1.0f };

    // 最後に貼ったパス（無駄なSetTexture回避）
    std::string m_LastNamePath;
    std::string m_LastTextPath;


    // 枠の中に収まる想定の仮レイアウト
    //DirectX::SimpleMath::Vector3 m_Pos = DirectX::SimpleMath::Vector3(0.0f, -420.0f, 0.0f);
    //DirectX::SimpleMath::Vector3 m_Size = DirectX::SimpleMath::Vector3(1800.0f, 300.0f, 1.0f);

public:
    // Object
    void Init() override;
    void Update() override;
    void Draw(Camera* cam) override;
    void Uninit() override;

public:
    // MessageManager から操作するAPI
    void Show(bool v) { m_Visible = v; }
    bool IsVisible() const { return m_Visible; }

    void Clear();
    //void SetName(const std::string& name);
    //void SetText(const std::string& text);
    // ページごとに呼ぶ
    void SetNameImage(const std::string& path);
    void SetTextImage(const std::string& path);

    // 調整用
    //話者
    void SetNameLayout(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& size)
    {
        m_NamePos = pos; m_NameSize = size;
    }
    //テキスト
    void SetTextLayout(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& size)
    {
        m_TextPos = pos; m_TextSize = size;
    }


    //const std::string& GetName() const { return m_Name; }
    //const std::string& GetText() const { return m_Text; }

    //void SetDummyTexture(const char* path) { m_Sprite.SetTexture(path); }
};

