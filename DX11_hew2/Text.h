#pragma once
#include "Object.h"
#include <string>
#include "Texture2D.h"

class Text : public Object
{
private:
    std::string m_Name;   // 名前欄
    std::string m_Text;   // 本文
	bool m_Visible = false; // 表示状態

    // 仮表示用
    Texture2D m_Sprite;

    // 枠の中に収まる想定の仮レイアウト
    DirectX::SimpleMath::Vector3 m_Pos = DirectX::SimpleMath::Vector3(0.0f, -420.0f, 0.0f);
    DirectX::SimpleMath::Vector3 m_Size = DirectX::SimpleMath::Vector3(1800.0f, 300.0f, 1.0f);

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
    void SetName(const std::string& name);
    void SetText(const std::string& text);

    const std::string& GetName() const { return m_Name; }
    const std::string& GetText() const { return m_Text; }

};

