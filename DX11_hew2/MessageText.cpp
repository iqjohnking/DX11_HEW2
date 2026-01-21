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

    m_Visible = false;

    m_LastNamePath.clear();
    m_LastTextPath.clear();
}

void MessageText::Uninit()
{
    m_NameSprite.Uninit();
    m_TextSprite.Uninit();
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
}

void MessageText::Draw(Camera* cam)
{
    if (!m_Visible) return;

    m_NameSprite.Draw(cam);
    m_TextSprite.Draw(cam);
}

void MessageText::Clear()
{

}

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