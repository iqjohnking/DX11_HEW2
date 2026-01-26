#include "Stage5.h"
#include "Game.h"

void Stage5::Init()
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

    //SoundFlg
    m_Conversation_BGM_flg_1 = false;
    m_Conversation_BGM_flg_2 = false;
    m_Conversation_BGM_flg_3 = false;
}

void Stage5::Uninit()
{
    if (m_Message)
    {
        m_Message->Stop();
    }

    m_Pages.clear();

    Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_005);

    // このシーンのオブジェクトを削除する
    for (auto& o : m_MySceneObjects) {
        Game::GetInstance()->DeleteObject(o);
    }
    m_MySceneObjects.clear();
}

void Stage5::Update()
{
    MessageUpdate();
    GameUpdate();
    SoundUpdate();

    // 終了会話が終わったらリザルトへ
    if (m_Flow == Flow::EndTalk)
    {
        if (!m_Message->IsPlaying())
        {
            Game::GetInstance()->ChangeScene(RESULT);
        }
    }
}

void Stage5::MessageUpdate()
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

void Stage5::GameUpdate()
{
    if (m_Flow != Flow::Gameplay) return;

    //60フレーム経過するごとに1秒プラス
    elapsedFrames++;
    elapsedSeconds = elapsedFrames / 60;
}

void Stage5::SoundUpdate()
{
    if (m_Message->GetIndex() == 1 && m_Conversation_BGM_flg_1 == false)
    {
        m_Conversation_BGM_flg_1 = true;
        //会話BGM開始
        Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_004);
    }

    if (m_Flow == Flow::Gameplay && m_Conversation_BGM_flg_2 == false)
    {
        m_Conversation_BGM_flg_2 = true;
        //会話BGM停止
        Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_004);
        //StageBGM開始
        Game::GetSound()->Play(SOUND_LABEL_BGM_STAGE_001);
    }

    if (m_Flow == Flow::EndTalk && m_Conversation_BGM_flg_3 == false)
    {
        m_Conversation_BGM_flg_3 = true;
        //StageBGM停止
        Game::GetSound()->Stop(SOUND_LABEL_BGM_STAGE_001);
        //StageBGM開始
        Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_005);
    }
}

void Stage5::BuildStartPages()
{
    m_Pages.clear();

    // Page0
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";        // name_miko.png
        p.textId = "stage5_start";  // text_stage1_end_***
        p.textIndex = 0;          // 000

        p.focus = FocusSide::Right;

        // Page0必須：左右の初期表情
        p.leftFaceId = "normal";
        p.rightFaceId = "normal";
        p.speakerFaceId = "";

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE5_START_000;

        m_Pages.push_back(p);
        //…(村の人達はみんな逃げれてるみたい)
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage5_start";
        p.textIndex = 1;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "glare";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE5_START_001;

        m_Pages.push_back(p);
        //…そろそろ腹が減って限界だ小娘…！
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage5_start";
        p.textIndex = 2;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE5_START_002;

        m_Pages.push_back(p);
        //３分以内に持ってこい…！でないとこの村の人間を食らい殺す…！
    }
}

void Stage5::BuildEndPages()
{
    m_Pages.clear();

    // Page0:
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";//表示名(テキスト)
        p.textId = "stage5_end";
        p.textIndex = 0;//(stage1_start_000.png)

        p.focus = FocusSide::Right;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";//蜘蛛初期表情
        p.rightFaceId = "normal";//巫女初期表情
        p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
        //今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
        //表情を変更しないときは何も書かないように

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE5_END_000;

        m_Pages.push_back(p);
        //人間は渡せないんで牛肉で我慢してください
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage5_end";
        p.textIndex = 1;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE5_END_001;

        m_Pages.push_back(p);
        //…大して美味くも無いが…飢えるよりマシか
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage5_end";
        p.textIndex = 2;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE5_END_002;

        m_Pages.push_back(p);
        //チッ…起きて最初の食事がこんな低劣なものとはな…
    }
    // Page3
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage5_end";
        p.textIndex = 3;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE5_END_003;

        m_Pages.push_back(p);
        //古書館に神託書があるんです。
    }
    // Page4
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage5_end";
        p.textIndex = 4;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE5_END_004;

        m_Pages.push_back(p);
        //壊される前に早く行かないと…！
    }
}

void Stage5::UpdateEnemySpawn()
{

}

void Stage5::StageClearCheck()
{

}

void Stage5::StageFailedCheck()
{

}