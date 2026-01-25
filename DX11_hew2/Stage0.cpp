#include "Stage0.h"
#include "Game.h"

void Stage0::Init()
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

void Stage0::Uninit()
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

void Stage0::Update()
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

void Stage0::MessageUpdate()
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

void Stage0::GameUpdate()
{
    if (m_Flow != Flow::Gameplay) return;

    //60フレーム経過するごとに1秒プラス
    elapsedFrames++;
    elapsedSeconds = elapsedFrames / 60;
}

void Stage0::BuildStartPages()
{
    m_Pages.clear();

    // Page0:
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "none";//表示名(テキスト)
        p.textId = "stage0_start";
        p.textIndex = 0;//(stage1_start_000.png)

        p.focus = FocusSide::None;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";//蜘蛛初期表情
        p.rightFaceId = "normal";//巫女初期表情
        p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
        //今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
        //表情を変更しないときは何も書かないように

        // このページのボイス
        //ボイスなし

        m_Pages.push_back(p);
        //（暗転、目を開いたように景色が広がる）
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 1;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_001;

        m_Pages.push_back(p);
        //……(どこだ、ここは...)
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage0_start";
        p.textIndex = 2;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        //ボイスなし

        m_Pages.push_back(p);
        //（森・少し開けた場所）
    }
    // Page3
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage0_start";
        p.textIndex = 3;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_003;

        m_Pages.push_back(p);
        //…神様…じゃ…ないですよね…
    }
    // Page4
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 4;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_004;

        m_Pages.push_back(p);
        //……巫女服…神の使いか…！
    }
    // Page5
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage0_start";
        p.textIndex = 5;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        //ボイスなし

        m_Pages.push_back(p);
        //（妖怪出現、女郎蜘蛛に跪く妖怪たち）
    }
    // Page6
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage0_start";
        p.textIndex = 6;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        //ボイスなし

        m_Pages.push_back(p);
        //…！
    }
    // Page7
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 7;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_007;

        m_Pages.push_back(p);
        //…ほう？祠に封印されていたのをこの女が破壊したことにより復活したと？
    }
    // Page8
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 8;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_008;

        m_Pages.push_back(p);
        //…私を封印した小賢しい神への復讐の時が来たらしい…！
    }
    // Page9
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 9;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_009;

        m_Pages.push_back(p);
        //だが…
    }
    // Page10
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage0_start";
        p.textIndex = 10;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        //ボイスなし

        m_Pages.push_back(p);
        //（森を囲うほど大規模な結界が作られた直後、前にいた数体が一瞬で繭にされた後貫かれ爆散、怯えたり警戒する妖怪たち）
    }
    // Page11
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 11;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_011;

        m_Pages.push_back(p);
        //千年前にお前たちの裏切りで封印されたことを知らないと思ったか？
    }
    // Page12
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 12;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_012;

        m_Pages.push_back(p);
        //まずお前たちから殺す
    }
    // Page13
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 13;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_013;

        m_Pages.push_back(p);
        //(妖怪どもが巫女に向かっている…奴が持つ鈴の力か)
    }
    // Page14
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 14;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_014;

        m_Pages.push_back(p);
        //(…塵集めには丁度いい。全員まとめて消してやる…！)
    }
    // Page15
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage0_start";
        p.textIndex = 15;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_015;

        m_Pages.push_back(p);
        //わっ…！危な…！
    }
    // Page16
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 16;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_016;

        m_Pages.push_back(p);
        //ほう？意外と素早いな？
    }
    // Page17
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage0_start";
        p.textIndex = 17;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_017;

        m_Pages.push_back(p);
        //言っておきますけど、私を殺したらあなたも死にますよ！？
    }
    // Page18
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage0_start";
        p.textIndex = 18;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_018;

        m_Pages.push_back(p);
        //そういう「召喚」だったんで！
    }
    // Page19
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage0_start";
        p.textIndex = 19;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_019;

        m_Pages.push_back(p);
        //…チッ…
    }
}

void Stage0::BuildEndPages()
{
    m_Pages.clear();
}

void Stage0::UpdateEnemySpawn()
{

}

void Stage0::StageClearCheck()
{

}

void Stage0::StageFailedCheck()
{

}