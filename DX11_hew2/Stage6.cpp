#include "Stage6.h"
#include "Game.h"

void Stage6::Init()
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

void Stage6::Uninit()
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

void Stage6::Update()
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

void Stage6::MessageUpdate()
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

void Stage6::GameUpdate()
{
    if (m_Flow != Flow::Gameplay) return;

    //60フレーム経過するごとに1秒プラス
    elapsedFrames++;
    elapsedSeconds = elapsedFrames / 60;
}

void Stage6::BuildStartPages()
{
    m_Pages.clear();

    // Page0:
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "Bookstore";//表示名(テキスト)
        p.textId = "stage6_start";
        p.textIndex = 0;//(stage1_start_000.png)

        p.focus = FocusSide::None;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";//蜘蛛初期表情
        p.rightFaceId = "normal";//巫女初期表情
        p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
        //今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
        //表情を変更しないときは何も書かないように

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_000;

        m_Pages.push_back(p);
        //や、やめろ！！来るな！
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage6_start";
        p.textIndex = 1;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        //ボイスなし

        m_Pages.push_back(p);
        //（古書館長が襲い掛かられる直前に間一髪助け出す巫女）
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage6_start";
        p.textIndex = 2;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_002;

        m_Pages.push_back(p);
        //大丈夫ですか！？
    }
    // Page3
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage6_start";
        p.textIndex = 3;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_003;

        m_Pages.push_back(p);
        //…っ！？
    }
    // Page4
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage6_start";
        p.textIndex = 4;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

		//ボイスなし

        m_Pages.push_back(p);
        //（助け出した隙を突かれて妖怪の軍勢に襲い掛かられそうな巫女、それを一網打尽にする女郎蜘蛛）
    }
    // Page5
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage6_start";
        p.textIndex = 5;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_005;

        m_Pages.push_back(p);
        //…ありがーー
    }
    // Page6
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage6_start";
        p.textIndex = 6;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_006;

        m_Pages.push_back(p);
        //黙れ。私と貴様は運命共同体だろうが、さっさと神託書とやらを探してこい
    }
}

void Stage6::BuildEndPages()
{
    m_Pages.clear();

    // Page0
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";        // name_miko.png
        p.textId = "stage6_end";  // text_stage1_end_***
        p.textIndex = 0;          // 000

        p.focus = FocusSide::Right;

        // Page0必須：左右の初期表情
        p.leftFaceId = "normal";
        p.rightFaceId = "normal";
        p.speakerFaceId = "";

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_000;

        m_Pages.push_back(p);
        //…あった……！
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage6_end";
        p.textIndex = 1;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_001;

        m_Pages.push_back(p);
        //のろのろと…もっと早く見つけられんものか？
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage6_end";
        p.textIndex = 2;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_002;

        m_Pages.push_back(p);
        //…いちいち嫌味言わないでくださいよ…
    }
    // Page3
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage6_end";
        p.textIndex = 3;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_003;

        m_Pages.push_back(p);
        //…この「大封印の書」…これさえあれば…
    }
    // Page4
    {
        MessagePage p;

        p.nameId = "villager";
        p.textId = "stage6_end";
        p.textIndex = 4;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_004;

        m_Pages.push_back(p);
        //ありがとうございます！なんとお礼を言ったらよいか…！
    }
    // Page5
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage6_end";
        p.textIndex = 5;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_005;

        m_Pages.push_back(p);
        //妖怪たちはあの人が…
    }
    // Page6
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage6_end";
        p.textIndex = 6;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        //ボイスなし

        m_Pages.push_back(p);
        //…
    }
    // Page7
    {
        MessagePage p;

        p.nameId = "villager";
        p.textId = "stage6_end";
        p.textIndex = 7;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_007;

        m_Pages.push_back(p);
        //…あの見た目、妖怪…まさか…
    }
    // Page8
    {
        MessagePage p;

        p.nameId = "villager";
        p.textId = "stage6_end";
        p.textIndex = 8;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_008;

        m_Pages.push_back(p);
        //本で見た事あるぞ…女郎蜘蛛じゃ…
    }
    // Page9
    {
        MessagePage p;

        p.nameId = "villager";
        p.textId = "stage6_end";
        p.textIndex = 9;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_009;

        m_Pages.push_back(p);
        //女郎蜘蛛…？私たちを助けたって…そんな訳…
    }
    // Page10
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage6_end";
        p.textIndex = 10;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_010;

        m_Pages.push_back(p);
        //あ…あの！本当なーー
    }
    // Page11
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage6_end";
        p.textIndex = 11;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_011;

        m_Pages.push_back(p);
        //…消えろ。
    }
    // Page12
    {
        MessagePage p;

        p.nameId = "villager";
        p.textId = "stage6_end";
        p.textIndex = 12;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_012;

        m_Pages.push_back(p);
        //え？
    }
    // Page13
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage6_end";
        p.textIndex = 13;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_013;

        m_Pages.push_back(p);
        //死にたくなければ全員消えろと言ってるんだ…！
    }
    // Page14
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage6_end";
        p.textIndex = 14;

        p.focus = FocusSide::None;
        p.speakerFaceId = "";

		//ボイスなし

        m_Pages.push_back(p);
        //（慌てて逃げ出す一同）
    }
    // Page15
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage6_end";
        p.textIndex = 15;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_015;

        m_Pages.push_back(p);
        //「ちょ…ちょっと…」
    }
    // Page16
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage6_end";
        p.textIndex = 16;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_016;

        m_Pages.push_back(p);
        //…村の者に手は出さん、さっさと壊した祠の場所へ向かえ…
    }
    // Page17
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage6_end";
        p.textIndex = 17;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_017;

        m_Pages.push_back(p);
        //ここにいるだけでも心底不愉快だ
    }
}

void Stage6::UpdateEnemySpawn()
{

}

void Stage6::StageClearCheck()
{

}

void Stage6::StageFailedCheck()
{

}