#include "TalkCharacter.h"

using namespace DirectX::SimpleMath;

std::string TalkCharacter::BuildTexturePath() const
{
    // 例: assets/texture/character/miko/miko_normal.png
    std::string base = "assets/texture/Message/character/";
    //実際
    /*    std::string path = base + m_CharId + "/" + m_CharId + "_" + m_FaceId + ".png";*/
	//テスト用仮画像
    std::string path = "assets/texture/Message/character/MessageCharaTest.png";
    return path;
}

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

    // 初期テクスチャ（char/faceが未設定なら貼らない）
    if (!m_CharId.empty() && !m_FaceId.empty())
    {
        auto path = BuildTexturePath();
        m_Sprite.SetTexture(path.c_str());
    }
}

void TalkCharacter::Update()
{
    // 今はアニメ無し。Texture2D側のUpdateは呼んでおく
    m_Sprite.Update();
}

void TalkCharacter::Draw(Camera* cam)
{
	// 非表示なら描画しない
    if (!m_Visible) return;

    // char/face が更新される運用の場合は、ここで毎回貼り直すより
    // MessageManager::BeginPage で Set() → 必要時だけ SetTexture する方が軽い
	//のちに修正予定
    ApplyLayoutToSprite();
    m_Sprite.Draw(cam);
}

void TalkCharacter::Uninit()
{
    m_Sprite.Uninit();
}