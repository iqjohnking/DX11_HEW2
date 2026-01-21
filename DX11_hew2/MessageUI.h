#pragma once
#include "Object.h"
#include <string>
#include "Texture2D.h"

class MessageUI : public Object
{
private:
    std::string m_FramePath; // 枠画像パス
    bool m_Visible = false;

    Texture2D m_Frame;

    // 仮レイアウト（画面下に固定）//(要調整)
    DirectX::SimpleMath::Vector3 m_Pos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 m_Size = DirectX::SimpleMath::Vector3(1920.0f, 1080.0f, 1.0f);

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

    void SetFrame(const std::string& path);
    const std::string& GetFramePath() const { return m_FramePath; }

    // 仮調整用
    void SetPosition(const DirectX::SimpleMath::Vector3& p) { m_Pos = p; }
    void SetSize(const DirectX::SimpleMath::Vector3& s) { m_Size = s; }
};

