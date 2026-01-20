#include "MessageText.h"
#include <iostream>   // デバッグ用

using namespace DirectX::SimpleMath;

void MessageText::Init()
{
    m_Sprite.Init();

    // レイアウト
    m_Sprite.SetPosition(m_Pos);
    m_Sprite.SetScale(m_Size);

    m_Visible = false;
}

void MessageText::Uninit()
{
    m_Sprite.Uninit();
}

void MessageText::Update()
{
    m_Sprite.Update();
}

void MessageText::Draw(Camera* cam)
{
    if (!m_Visible) return;

    // 念のため反映
    m_Sprite.SetPosition(m_Pos);
    m_Sprite.SetScale(m_Size);

    m_Sprite.Draw(cam);
}

void MessageText::Clear()
{
    m_Name.clear();
    m_Text.clear();
}

void MessageText::SetName(const std::string& name)
{
    m_Name = name;

    // 仮：今はログで確認
    std::cout << "[Message][Name] " << m_Name << std::endl;
}

void MessageText::SetText(const std::string& text)
{
    m_Text = text;

    // 仮：今はログで確認
    std::cout << "[Message][Text] " << m_Text << std::endl;
}