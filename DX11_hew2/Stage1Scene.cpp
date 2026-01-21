#include "Stage1Scene.h"
#include "Game.h"
#include "Input.h"
#include "GolfBall.h"
#include "Ground.h"
#include"Shrinemaiden.h"
#include"Enemy_base.h"
#include"Enemy1.h"


// コンストラクタ
Stage1Scene::Stage1Scene()
{
	
}

// デストラクタ
Stage1Scene::~Stage1Scene()
{
	
}

// 初期化
void Stage1Scene::Init()
{
	//カメラのポインタを取得
	Camera* cam = Game::GetInstance()->GetCamera();

    // まず開始会話の台本を作る
    BuildStartPages();

    // 司令塔をObjectとして追加
    m_Message = Game::GetInstance()->AddObject<MessageManager>();
    m_MySceneObjects.emplace_back(m_Message);

	// 会話参加者の指定（キャラID）
    m_Message->SetParticipants("kumo", "miko");

    // 会話素材の指定
    m_Message->SetFramePath("assets/texture/Message/UI/field.jpg");

    // 台本セット
    m_Message->SetPages(m_Pages);

    // 開始時
    m_Message->Play();

    m_Flow = Flow::StartTalk;
}

//更新
void Stage1Scene::Update()
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

// 終了処理
void Stage1Scene::Uninit()
{
    if (m_Message)
    {
        m_Message->Stop();
    }

	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}

void Stage1Scene::BuildStartPages()
{
    m_Pages.clear();

    // Page0: 開始（左=女郎蜘蛛が話す）
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "kumo";          // name_kumo.png
        p.textId = "stage1_start";  // text_stage1_start_***
        p.textIndex = 0;            // 000

        p.voiceId = "";
        p.focus = FocusSide::Left;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";
        p.rightFaceId = "angry";

        // 話者（左）表情
        p.speakerFaceId = "normal";

        m_Pages.push_back(p);
    }

    // Page1: 右=巫女
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "miko";          // name_miko.png
        p.textId = "stage1_start";
        p.textIndex = 1;            // 001

        p.voiceId = "";
        p.focus = FocusSide::Right;

        // 話者（右）だけ表情変更
        p.speakerFaceId = "smile";

        m_Pages.push_back(p);
    }

    // Page2: 左=女郎蜘蛛
    {
        MessagePage p;

        // ★必須：このページの表示（名前＋本文）
        p.nameId = "kumo";
        p.textId = "stage1_start";
        p.textIndex = 2;            // 002

        p.voiceId = "";
        p.focus = FocusSide::Left;

        p.speakerFaceId = "surprised";

        m_Pages.push_back(p);
    }
}
void Stage1Scene::BuildEndPages()
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