#include "MessageUI.h"

using namespace DirectX::SimpleMath;

void MessageUI::Init()
{
    m_Frame.Init();

    // 初期レイアウト
    m_Frame.SetPosition(m_Pos);
    m_Frame.SetScale(m_Size);

    // まだパスが無ければ何も貼らない（SetFrameで貼る）
    m_Visible = false;
}


void MessageUI::Uninit()
{
    m_Frame.Uninit();
}

void MessageUI::Update()
{
    // 今は何もしない（将来アニメとかやるならここ）
    m_Frame.Update();
}

void MessageUI::Draw(Camera* cam)
{
    if (!m_Visible) return;

    // 念のため毎フレ反映（調整が効くように）
    m_Frame.SetPosition(m_Pos);
    m_Frame.SetScale(m_Size);

    m_Frame.Draw(cam);
}

void MessageUI::SetFrame(const std::string& path)
{
    if (path.empty())
        return;

    // 同じなら何もしない
    if (m_FramePath == path)
        return;

    m_FramePath = path;

    // Texture2Dは const char* を取るので変換
    m_Frame.SetTexture(m_FramePath.c_str());
}
