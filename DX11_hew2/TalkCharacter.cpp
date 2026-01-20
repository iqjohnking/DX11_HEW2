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

    // Focus（暗くする等）は Texture2D 側に色/α設定が無いので今は未適用
    // 後で Texture2D に頂点Color/Material Diffuse を変更する関数を足せば実装できる
    //のちに実装予定
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
    // 仮運用：charId/faceId が空ならテスト画像
    if (m_CharId.empty() || m_FaceId.empty())
    {
        return "assets/texture/Message/character/MessageCharaTest.png";
    }

    // focus反映：非話者なら _dark を付ける
    std::string face = m_FaceId;
    if (!m_Focus)
    {
        // すでに "_dark" なら二重に付けない
        const std::string suffix = "_dark";
        if (face.size() < suffix.size() ||
            face.compare(face.size() - suffix.size(), suffix.size(), suffix) != 0)
        {
            face += suffix; // normal -> normal_dark
        }
    }

    // 例：assets/texture/Message/character/miko_normal.png
    // 例：assets/texture/Message/character/miko_normal_dark.png
    std::string base = "assets/texture/Message/character/";
    std::string path = base + m_CharId + "_" + face + ".png";
    return path;
}

void TalkCharacter::ApplyTextureIfNeeded()
{
    const std::string path = BuildTexturePath();
    if (path.empty()) return;

    if (path == m_LastAppliedPath)
        return; // 変化なし

    m_Sprite.SetTexture(path.c_str());
    m_LastAppliedPath = path;
}

void TalkCharacter::SetFocus(bool focus)
{
    if (m_Focus == focus)
        return;

    m_Focus = focus;
    ApplyTextureIfNeeded(); // normal <-> normal_dark へ切替
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
