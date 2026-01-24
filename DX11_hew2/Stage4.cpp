#include "Stage4.h"
#include "Game.h"

void Stage4::Init()
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

void Stage4::Uninit()
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

void Stage4::Update()
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

void Stage4::MessageUpdate()
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

void Stage4::GameUpdate()
{
    if (m_Flow != Flow::Gameplay) return;

    //60フレーム経過するごとに1秒プラス
    elapsedFrames++;
    elapsedSeconds = elapsedFrames / 60;
}

void Stage4::BuildStartPages()
{
    m_Pages.clear();

    // Page0:
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";//表示名(テキスト)
        p.textId = "stage4_start";
        p.textIndex = 0;//(stage1_start_000.png)

        p.focus = FocusSide::Right;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";//蜘蛛初期表情
        p.rightFaceId = "normal";//巫女初期表情
        p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
        //今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
        //表情を変更しないときは何も書かないように

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_000;

        m_Pages.push_back(p);
        //やっぱり……何回来ても良いなぁ……
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage4_start";
        p.textIndex = 1;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_001;

        m_Pages.push_back(p);
        //…くだらない、さっさと要件を済ませろ
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage4_start";
        p.textIndex = 2;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        //ボイスなし

        m_Pages.push_back(p);
        //（村人逃げる）
    }
    // Page3
    {
        MessagePage p;

        p.nameId = "villager";
        p.textId = "stage4_start";
        p.textIndex = 3;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_003;

        m_Pages.push_back(p);
        //た、助けてくれ……誰かぁ！!
    }
    // Page4
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage4_start";
        p.textIndex = 4;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_004;

        m_Pages.push_back(p);
        //どうしたんですか！？
    }
    // Page5
    {
        MessagePage p;

        p.nameId = "villager";
        p.textId = "stage4_start";
        p.textIndex = 5;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_005;

        m_Pages.push_back(p);
        //化け物が…変な化け物が！
    }
    // Page6
    {
        MessagePage p;

        p.nameId = "villager";
        p.textId = "stage4_start";
        p.textIndex = 6;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_006;

        m_Pages.push_back(p);
        //俺たちの家や古書館を襲って…！　…！？
    }
    // Page7
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage4_start";
        p.textIndex = 7;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_007;

        m_Pages.push_back(p);
        //人間か…
    }
    // Page8
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage4_start";
        p.textIndex = 8;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_008;

        m_Pages.push_back(p);
        //腹ごしらえには丁度いい
    }
    // Page9
    {
        MessagePage p;

        p.nameId = "villager";
        p.textId = "stage4_start";
        p.textIndex = 9;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_009;

        m_Pages.push_back(p);
        //う…うわああああ！
    }
    // Page10
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage4_start";
        p.textIndex = 10;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

		//ボイスなし

        m_Pages.push_back(p);
        //（逃げられる）
    }
    // Page11
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage4_start";
        p.textIndex = 11;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_011;

        m_Pages.push_back(p);
        //あの…！
    }
    // Page12
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage4_start";
        p.textIndex = 12;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_012;

        m_Pages.push_back(p);
        //村の人たちに手を出したら私死にますから…！
    }
    // Page13
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage4_start";
        p.textIndex = 13;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_013;

        m_Pages.push_back(p);
        //…！…クソガキが…！
    }
}

void Stage4::BuildEndPages()
{
    m_Pages.clear();

    // Page0
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";        // name_miko.png
        p.textId = "stage4_end";  // text_stage1_end_***
        p.textIndex = 0;          // 000

        p.focus = FocusSide::Right;

        // Page0必須：左右の初期表情
        p.leftFaceId = "normal";
        p.rightFaceId = "normal";
        p.speakerFaceId = "";

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_END_000;

        m_Pages.push_back(p);
        //そんなにお腹が空いてるなら、お団子でも五平餅でも食べればいいじゃないですか
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage4_end";
        p.textIndex = 1;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_END_001;

        m_Pages.push_back(p);
        //…理性を保つのに、生の血肉が必要でな
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage4_end";
        p.textIndex = 2;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_END_002;

        m_Pages.push_back(p);
        //なら屋台から貰ってくるんでちょっと我慢してください！
    }
}

void Stage4::UpdateEnemySpawn()
{

}

void Stage4::StageClearCheck()
{

}

void Stage4::StageFailedCheck()
{

}