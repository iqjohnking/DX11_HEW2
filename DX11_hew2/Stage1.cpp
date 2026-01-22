#include "Stage1.h"

#include "Game.h"
#include "Input.h"

void Stage1::Init()
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

void Stage1::Uninit()
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

void Stage1::Update()
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
        if (Input::GetKeyTrigger(VK_RETURN))
        {
            BuildEndPages();
            m_Message->SetPages(m_Pages);
            m_Message->Play();
            m_Flow = Flow::EndTalk;
        }
        break;

    case Flow::EndTalk:
        // 終了会話が終わったらリザルトへ
        if (!m_Message->IsPlaying())
        {
            Game::GetInstance()->ChangeScene(RESULT);
        }
        break;
    }
}


//会話パート
void Stage1::BuildStartPages()
{
    m_Pages.clear();

    // Page0:
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";//表示名(テキスト)
        p.textId = "stage1_start";
        p.textIndex = 0;//(stage1_start_000.png)

        p.voiceId = "";
        p.focus = FocusSide::Right;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";//蜘蛛初期表情
        p.rightFaceId = "normal";//巫女初期表情
        p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
        //今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
        //表情を変更しないときは何も書かないように

        m_Pages.push_back(p);
        //…助けて貰って、ありがとうございます
    }

    // Page1
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage1_start";
        p.textIndex = 1;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //……あなたは…何なんですか…？
    }

    // Page2
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 2;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //お前たちのいう女郎蜘蛛…とでも言えば分かるか？
    }
    // Page3
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage1_start";
        p.textIndex = 3;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //…！
    }

    // Page4
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage1_start";
        p.textIndex = 4;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //…なんで…私が呼び出したのは神様のはずじゃ…！
    }
    // Page5
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage1_start";
        p.textIndex = 5;

        p.voiceId = "";
        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //（結界陣を見る女郎蜘蛛）
    }
    // Page6
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 6;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //(推し量るに…私が呼び出されたのは助けを呼ぼうと召喚術を行って邪魔されてこいつらの邪気が混じった結果か)
    }
    // Page7
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 7;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //……私を利用しようと召喚した代償は払ってもらうぞ？小娘…
    }
    // Page8
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage1_start";
        p.textIndex = 8;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //……へへ
    }
    // Page9
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 9;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //舐めてるのか？
    }
    // Page10
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage1_start";
        p.textIndex = 10;

        p.voiceId = "";
        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //（遠くを見渡す女郎蜘蛛）
    }
    // Page11
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 11;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //(…結界から逃げたのがちらほらといるようだな…)
    }
    // Page12
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 12;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //…そうだ…
    }
    // Page13
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage1_start";
        p.textIndex = 13;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //な…何ですか…
    }
    // Page14
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 14;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //お前を死なせさえしなければいいのだったか？
    }
    // Page15
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage1_start";
        p.textIndex = 15;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //えっ？そ、そうですけど…
    }
    // Page16
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 16;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //囮になれ
    }
    // Page17
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage1_start";
        p.textIndex = 17;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //で、でも私…
    }
    // Page18
    {
        MessagePage p;

        p.nameId = "none";
        p.textId = "stage1_start";
        p.textIndex = 18;

        p.voiceId = "";
        p.focus = FocusSide::None;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //（巫女の首に軽く糸を巻き付けて圧をかける）
    }
    // Page19
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 19;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //…お前を「死なせさえしなければ」いいのだったか？
    }
    // Page20
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage1_start";
        p.textIndex = 20;

        p.voiceId = "";
        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //うっ…や…やります…！
    }
    // Page21
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 21;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //さて…復讐の第一歩だ…
    }
    // Page22
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 22;

        p.voiceId = "";
        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        m_Pages.push_back(p);
        //全員滅ぼしてやる…！
    }
}

void Stage1::BuildEndPages()
{
    m_Pages.clear();

    // Page0: 終了（右=巫女が話す）
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";        // name_miko.png
        p.textId = "stage1_end";  // text_stage1_end_***
        p.textIndex = 0;          // 000

        p.voiceId = "";
        p.focus = FocusSide::Right;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";
        p.rightFaceId = "normal";

        // 話者（右）表情
        p.speakerFaceId = "normal";

        m_Pages.push_back(p);
    }

    // Page1: 左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "kumo";
        p.textId = "stage1_end";
        p.textIndex = 1;          // 001

        p.voiceId = "";
        p.focus = FocusSide::Left;

        p.speakerFaceId = "angry";

        m_Pages.push_back(p);
    }
}
