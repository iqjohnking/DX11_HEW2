#include "Stage2.h"
#include "Game.h"

void Stage2::Init()
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

void Stage2::Uninit()
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

void Stage2::Update()
{
    MessageUpdate();
    GameUpdate();
    // 終了会話が終わったらリザルトへ
    if (m_Flow == Flow::EndTalk)
    {
        if (!m_Message->IsPlaying())
        {
            Game::GetInstance()->ChangeScene(RESULT);
        }
    }
}

void Stage2::MessageUpdate()
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
        //// 終了会話が終わったらリザルトへ
        //if (!m_Message->IsPlaying())
        //{
        //    Game::GetInstance()->ChangeScene(RESULT);
        //}
        break;
    }
}

void Stage2::GameUpdate()
{
    if (m_Flow != Flow::Gameplay) return;

    //60フレーム経過するごとに1秒プラス
    elapsedFrames++;
    elapsedSeconds = elapsedFrames / 60;
}

void Stage2::BuildStartPages()
{
    m_Pages.clear();
    // Page0:
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";//表示名(テキスト)
        p.textId = "stage2_start";
        p.textIndex = 0;//(stage1_start_000.png)

        p.voiceId = "";
        p.focus = FocusSide::Right;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";//蜘蛛初期表情
        p.rightFaceId = "normal";//巫女初期表情
        p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
        //今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
        //表情を変更しないときは何も書かないように
        
        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE2_START_000;

        m_Pages.push_back(p);
        //まだこんなに…！？
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage2_start";
        p.textIndex = 1;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE2_START_001;

        m_Pages.push_back(p);
        //精々囮として役に立つことだな、小娘！
    }
}

void Stage2::BuildEndPages()
{
    m_Pages.clear();

    // Page0
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";        // name_miko.png
        p.textId = "stage2_end";  // text_stage1_end_***
        p.textIndex = 0;          // 000

        p.voiceId = "";
        p.focus = FocusSide::Left;

        // Page0必須：左右の初期表情
        p.leftFaceId = "normal";
        p.rightFaceId = "normal";
        p.speakerFaceId = "";

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE2_END_000;

        m_Pages.push_back(p);
        //はぁ…はぁ…まだいるの…？
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage2_end";
        p.textIndex = 1;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE2_END_001;

        m_Pages.push_back(p);
        //この程度で疲れるな、まだ２割も倒していない
    }
    // Page3
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage2_end";
        p.textIndex = 2;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE2_END_002;

        m_Pages.push_back(p);
        //この程度で疲れるな、まだ２割も倒していない
    }
}


void Stage2::UpdateEnemySpawn()
{

}

void Stage2::StageClearCheck()
{

}

void Stage2::StageFailedCheck()
{

}
