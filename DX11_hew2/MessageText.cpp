#include "MessageText.h"
#include <iostream>   // デバッグ用

using namespace DirectX::SimpleMath;

void MessageText::Init()
{
    m_NameSprite.Init();
    m_TextSprite.Init();

    // 初期レイアウト反映
    m_NameSprite.SetPosition(m_NamePos);
    m_NameSprite.SetScale(m_NameSize);

    m_TextSprite.SetPosition(m_TextPos);
    m_TextSprite.SetScale(m_TextSize);

    //m_Sprite.Init();

    //// レイアウト
    //m_Sprite.SetPosition(m_Pos);
    //m_Sprite.SetScale(m_Size);

    m_Visible = false;

    m_LastNamePath.clear();
    m_LastTextPath.clear();
}

void MessageText::Uninit()
{
    m_NameSprite.Uninit();
    m_TextSprite.Uninit();
    //m_Sprite.Uninit();
}

void MessageText::Update()
{
    //調整用
    //不要なら削除
    m_NameSprite.SetPosition(m_NamePos);
    m_NameSprite.SetScale(m_NameSize);

    m_TextSprite.SetPosition(m_TextPos);
    m_TextSprite.SetScale(m_TextSize);

    m_NameSprite.Update();
    m_TextSprite.Update();

    //m_Sprite.Update();
}

void MessageText::Draw(Camera* cam)
{
    if (!m_Visible) return;

    m_NameSprite.Draw(cam);
    m_TextSprite.Draw(cam);

   //m_Sprite.Draw(cam);
}

void MessageText::Clear()
{
    m_Name.clear();
    m_Text.clear();
}

//void MessageText::SetName(const std::string& name)
//{
//    m_Name = name;
//
//    // 仮：今はログで確認
//    std::cout << "[Message][Name] " << m_Name << std::endl;
//}
//
//void MessageText::SetText(const std::string& text)
//{
//    m_Text = text;
//
//    // 仮：今はログで確認
//    std::cout << "[Message][Text] " << m_Text << std::endl;
//}

void MessageText::SetNameImage(const std::string& path)
{
    if (path.empty()) return;
    if (path == m_LastNamePath) return;

    m_NameSprite.SetTexture(path.c_str());
    m_LastNamePath = path;
}

void MessageText::SetTextImage(const std::string& path)
{
    if (path.empty()) return;
    if (path == m_LastTextPath) return;

    m_TextSprite.SetTexture(path.c_str());
    m_LastTextPath = path;
}