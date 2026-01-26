#include "Stage8.h"
#include "Game.h"

void Stage8::Init()
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

    Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_007);
}

void Stage8::Uninit()
{
    if (m_Message)
    {
        m_Message->Stop();
    }

    m_Pages.clear();

    Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_007);

    // このシーンのオブジェクトを削除する
    for (auto& o : m_MySceneObjects) {
        Game::GetInstance()->DeleteObject(o);
    }
    m_MySceneObjects.clear();
}

void Stage8::Update()
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

void Stage8::MessageUpdate()
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

void Stage8::GameUpdate()
{
    if (m_Flow != Flow::Gameplay) return;

    //60フレーム経過するごとに1秒プラス
    elapsedFrames++;
    elapsedSeconds = elapsedFrames / 60;
}

void Stage8::SoundUpdate()
{
    if (m_Flow == Flow::Gameplay && m_Conversation_BGM_flg_1 == false)
    {
        m_Conversation_BGM_flg_1 = true;
        //会話BGM停止
        Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_007);
        //StageBGM開始
        Game::GetSound()->Play(SOUND_LABEL_BGM_STAGE_002);
    }

    if(m_Flow == Flow::EndTalk && m_Conversation_BGM_flg_2 == false)
    {
        m_Conversation_BGM_flg_2 = true;
        //StageBGM停止
        Game::GetSound()->Stop(SOUND_LABEL_BGM_STAGE_002);
        //会話BGM開始
        Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_007);
	}
}

void Stage8::BuildStartPages()
{
    m_Pages.clear();

    // Page0:
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";//表示名(テキスト)
        p.textId = "stage8_start";
        p.textIndex = 0;//(stage1_start_000.png)

        p.focus = FocusSide::Right;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";//蜘蛛初期表情
        p.rightFaceId = "normal";//巫女初期表情
        p.speakerFaceId = "surprised";//フォーカスしている話者のみ表情を変更
        //今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
        //表情を変更しないときは何も書かないように

        // このページのボイス
        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_START_000;

        m_Pages.push_back(p);
        //「あった…祠！」
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage8_start";
        p.textIndex = 1;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "surprised";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_START_001;

        m_Pages.push_back(p);
        //…！急げ小娘！
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage8_start";
        p.textIndex = 2;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_START_002;

        m_Pages.push_back(p);
        //…！？なんであんなに…！？
    }
    // Page3
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage8_start";
        p.textIndex = 3;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "glare";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_START_003;

        m_Pages.push_back(p);
        //お前がその本を持っている事を知られたのだろうな。封印を避けるために私のこと関係なく来るつもりか…！
    }
}

void Stage8::BuildEndPages()
{
    m_Pages.clear();

    // Page0
    // 右=巫女,左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "none";        // name_miko.png
        p.textId = "stage8_end";  // text_stage1_end_***
        p.textIndex = 0;          // 000

        p.focus = FocusSide::None;

        // Page0必須：左右の初期表情
        p.leftFaceId = "normal";
        p.rightFaceId = "normal";
        p.speakerFaceId = "";

        // このページのボイス
		//ボイスなし

        m_Pages.push_back(p);
        //（本の中身を確認する）
    }
    // Page1
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage8_end";
        p.textIndex = 1;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_001;

        m_Pages.push_back(p);
        //「…えっと…どれを使えば…！」
    }
    // Page2
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage8_end";
        p.textIndex = 2;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_002;

        m_Pages.push_back(p);
        //…大封印の書、二百三十二番
    }
    // Page3
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage8_end";
        p.textIndex = 3;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_003;

        m_Pages.push_back(p);
        //二百三十二番…
    }
    // Page4
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage8_end";
        p.textIndex = 4;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "surprised";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_004;

        m_Pages.push_back(p);
        //…！別世界への封印…
    }
    // Page5
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage8_end";
        p.textIndex = 5;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_005;

        m_Pages.push_back(p);
        //昔私を封印した時に使っていた封印術だ。それを使って妖怪共をまとめて私と同じ場所に封印しろ
    }
    // Page6
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage8_end";
        p.textIndex = 6;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "normal";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_006;

        m_Pages.push_back(p);
        //…どうしてですか？
    }
    // Page7
    {
        MessagePage p;

        p.nameId = "miko";
        p.textId = "stage8_end";
        p.textIndex = 7;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_007;

        m_Pages.push_back(p);
        //…復讐するとか言ってたじゃないですか、別に止めはしませんけど…
    }
    // Page8
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage8_end";
        p.textIndex = 8;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_008;

        m_Pages.push_back(p);
        //……どのみちこの制約を解かん限りそんなこと不可能だからな、
    }
    // Page9
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage8_end";
        p.textIndex = 9;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_009;

        m_Pages.push_back(p);
        //このまま行ったところで小娘、貴様が消されれば終わりだ
    }
    // Page10
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage8_end";
        p.textIndex = 10;

        p.focus = FocusSide::Left;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE8_END_010;

        m_Pages.push_back(p);
        //…まず裏切り者を全員殺す
    }
    // Page11
    {
        MessagePage p;

        p.nameId = "kumo";
        p.textId = "stage8_end";
        p.textIndex = 11;

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

		//ボイスなし

        m_Pages.push_back(p);
        //…
    }
}

void Stage8::UpdateEnemySpawn()
{

}

void Stage8::StageClearCheck()
{

}

void Stage8::StageFailedCheck()
{

}