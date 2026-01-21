#include "TalkCharacter.h"

using namespace DirectX::SimpleMath;

void TalkCharacter::ApplyLayoutToSprite()
{
    // サイズ
    m_Sprite.SetScale(m_Size);

    // 位置（左右で基準を変える）
    if (m_Side == TalkSide::Left)
    {
        m_Sprite.SetPosition(m_BasePosL);
        m_Sprite.SetFlipX(false);
    }
    else
    {
        m_Sprite.SetPosition(m_BasePosR);
        // 右側キャラを左右反転したいなら true（素材による）
        m_Sprite.SetFlipX(false);
    }

    // フォーカス状態に応じて明るさを反映
    //m_Sprite.SetBrightness(m_Focus ? 1.0f : 0.45f);
}

void TalkCharacter::Init()
{
    m_Sprite.Init();
    ApplyLayoutToSprite();

    // 最初は非表示
    m_Visible = false;
    m_Focus = true;

    ApplyTextureIfNeeded();
}

void TalkCharacter::Update()
{
    // 今はアニメ無し。Texture2D側のUpdateは呼んでおく
    ApplyLayoutToSprite();
    m_Sprite.Update();
}

void TalkCharacter::Draw(Camera* cam)
{
	// 非表示なら描画しない
    if (!m_Visible) return;

    m_Sprite.Draw(cam);
}

void TalkCharacter::Uninit()
{
    m_Sprite.Uninit();
}

std::string TalkCharacter::BuildTexturePath() const
{
    // charId/faceIdが揃ってないなら何も描画しない
    if (m_CharId.empty() || m_FaceId.empty())
    {
        return "";
    }

    //// focus反映：非話者なら _dark を付ける
    //std::string face = m_FaceId;
    //if (!m_Focus)
    //{
    //    // すでに "_dark" なら二重に付けない
    //    const std::string suffix = "_dark";
    //    if (face.size() < suffix.size() ||
    //        face.compare(face.size() - suffix.size(), suffix.size(), suffix) != 0)
    //    {
    //        face += suffix; // normal -> normal_dark
    //    }
    //}

    // 例：assets/texture/Message/character/miko_normal.png
    // 例：assets/texture/Message/character/miko_normal_dark.png
    std::string base = "assets/texture/Message/character/";
    std::string path = base + m_CharId + "_" + m_FaceId + ".png";
    return path;
}

void TalkCharacter::ApplyTextureIfNeeded()
{
    const std::string path = BuildTexturePath();
    if (path.empty()) return;

    if (path == m_LastAppliedPath)
        return; // 変化なし

    m_Sprite.SetTexture(path.c_str());
    m_Sprite.SetBrightness(m_Focus ? 1.0f : 0.3f);
    m_LastAppliedPath = path;
}

void TalkCharacter::SetFocus(bool focus)
{
    if (m_Focus == focus)
        return;

    m_Focus = focus;

    // 話者は明るく、非話者は暗く
    m_Sprite.SetBrightness(m_Focus ? 1.0f : 0.3f);
}



void TalkCharacter::SetCharacter(const std::string& charId)
{
    if (m_CharId == charId) return;
    m_CharId = charId;
    ApplyTextureIfNeeded();
}

void TalkCharacter::SetFace(const std::string& faceId)
{
    if (m_FaceId == faceId) return;
    m_FaceId = faceId;
    ApplyTextureIfNeeded();
}

void TalkCharacter::Set(const std::string& charId, const std::string& faceId)
{
    bool changed = false;
    if (m_CharId != charId) { m_CharId = charId; changed = true; }
    if (m_FaceId != faceId) { m_FaceId = faceId; changed = true; }

    if (changed)
        ApplyTextureIfNeeded();
}
