#include "Text.h"
#include <iostream>   // デバッグ用

using namespace DirectX::SimpleMath;

void Text::Init()
{
    m_Sprite.Init();

    // 仮テキスト画像をセット
    m_Sprite.SetTexture("assets/texture/text/Test_Text.png");

    // レイアウト
    m_Sprite.SetPosition(m_Pos);
    m_Sprite.SetScale(m_Size);

    m_Visible = false;
}

void Text::Uninit()
{
    m_Sprite.Uninit();
}

void Text::Update()
{
    m_Sprite.Update();
}

void Text::Draw(Camera* cam)
{
    if (!m_Visible) return;

    // 念のため反映
    m_Sprite.SetPosition(m_Pos);
    m_Sprite.SetScale(m_Size);

    m_Sprite.Draw(cam);
}

void Text::Clear()
{
    m_Name.clear();
    m_Text.clear();
}

void Text::SetName(const std::string& name)
{
    m_Name = name;

    // 仮：今はログで確認
    std::cout << "[Message][Name] " << m_Name << std::endl;
}

void Text::SetText(const std::string& text)
{
    m_Text = text;

    // 仮：今はログで確認
    std::cout << "[Message][Text] " << m_Text << std::endl;
}