#include "Stage3.h"
#include "Game.h"

void Stage3::Init()
{
    // まず開始会話の台本を作る
    BuildStartPages();

    // 司令塔をObjectとして追加
    m_Message = Game::GetInstance()->AddObject<MessageManager>();
    m_MySceneObjects.emplace_back(m_Message);

    // 会話参加者の指定（キャラID）
    m_Message->SetParticipants("kumo", "miko");

    // 会話素材の指定
    m_Message->SetFramePath("assets/texture/Message/UI/MessageUI.png");

    m_Message->SetBackgroundPath("assets/texture/Message/bg/bg_stage1.jpg");

    // 台本セット
    m_Message->SetPages(m_Pages);

    // 開始時
    m_Message->Play();

    m_Flow = Flow::StartTalk;
}

void Stage3::Uninit()
{
    if (m_Message)
    {
        m_Message->Stop();
    }

    m_Pages.clear();

    // このシーンのオブジェクトを削除する
    for (auto& o : m_MySceneObjects) {
        Game::GetInstance()->DeleteObject(o);
    }
    m_MySceneObjects.clear();
}

void Stage3::Update()
{
    MessageUpdate();
    GameUpdate();
    UpdateEnemySpawn();
    // 終了会話が終わったらリザルトへ
    if (m_Flow == Flow::EndTalk)
    {
        if (!m_Message->IsPlaying())
        {
            Game::GetInstance()->ChangeScene(RESULT);
        }
    }
}

void Stage3::MessageUpdate()
{
    if (!m_Message) return;

    switch (m_Flow)
    {
    case Flow::StartTalk:
        // 開始会話が終わったらゲーム開始へ
        if (!m_Message->IsPlaying())
        {
            m_Flow = Flow::Gameplay;
        }
        break;

    case Flow::Gameplay:
        // 仮：Enterでステージ終了扱い→終了会話へ
        if (Input::GetKeyTrigger(VK_SPACE))
        {
            BuildEndPages();
            m_Message->SetPages(m_Pages);
            m_Message->Play();
            m_Flow = Flow::EndTalk;
        }
        break;

    case Flow::EndTalk:
        break;
    }
}

void Stage3::GameUpdate()
{
    if (m_Flow != Flow::Gameplay) return;

    //m_MySceneObjects中の空間オブジェクトを削除する（erase）
    for (auto it = m_MySceneObjects.begin(); it != m_MySceneObjects.end(); )
    {
        Object* o = *it; // オブジェクト取得
        if (!o || o->ToBeDeleted())
        {
            it = m_MySceneObjects.erase(it); // イテレータを更新
        }
        else
        {
            ++it; // 次へ
        }
    }
}

void Stage3::BuildStartPages()
{
    m_Pages.clear();

    // Page0:
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";//表示名(テキスト)
        p.textId = "stage3_start";
        p.textIndex = 0;//(stage1_start_000.png)

        p.focus = FocusSide::Right;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";//蜘蛛初期表情
        p.rightFaceId = "normal";//巫女初期表情
        p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
        //今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
        //表情を変更しないときは何も書かないように

         // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE3_START_000;

        m_Pages.push_back(p);
        //…そうだ！
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage3_start";
        p.textIndex = 1;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE3_START_001;

        m_Pages.push_back(p);
        //この辺りの妖怪たちを倒したら村のほうに向かいます
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage3_start";
        p.textIndex = 2;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE3_START_002;

        m_Pages.push_back(p);
        //…好きにしろ
    }
}

void Stage3::BuildEndPages()
{
    m_Pages.clear();

    // Page0
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "kumo";        // name_miko.png
        p.textId = "stage3_end";  // text_stage1_end_***
        p.textIndex = 0;          // 000

        p.focus = FocusSide::Left;

        // Page0必須：左右の初期表情
        p.leftFaceId = "normal";
        p.rightFaceId = "normal";
        p.speakerFaceId = "";

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE3_END_000;

        m_Pages.push_back(p);
        //…終わったぞ、さっさとしろ
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage3_end";
        p.textIndex = 1;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE3_END_001;

        m_Pages.push_back(p);
        //今から向かう村には、古くから伝わる神託書が中があるんです
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage3_end";
        p.textIndex = 2;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";
        
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE3_END_002;

        m_Pages.push_back(p);
        //一度きりの、万が一の時にのみ使うことを許された神託書が
    }
}

void Stage3::UpdateEnemySpawn()
{

}

void Stage3::StageClearCheck()
{

}

void Stage3::StageFailedCheck()
{

}
