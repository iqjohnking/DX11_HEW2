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

	//// オブジェクトを作成
	//m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<GolfBall>(cam));
	//m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Ground>());

    BuildTestPages();

    // 司令塔をObjectとして追加
    m_Message = Game::GetInstance()->AddObject<MessageManager>();
    m_MySceneObjects.emplace_back(m_Message);

    // 会話素材の指定
    m_Message->SetFramePath("assets/texture/Message/UI/field.jpg");                 // 下枠
    m_Message->SetTextDummyPath("assets/texture/Message/text/Test_Text.png");       // 仮テキスト
    m_Message->SetCharaDummyPath("assets/texture/message/character/MessageCharaTest.png"); // 立ち絵両方

    // 台本セット
    m_Message->SetPages(m_TestPages);

    // 開始時
    m_Message->Play();

}

//更新
void Stage1Scene::Update()
{
	//// エンターキーを押してリザルトへ
	//if (Input::GetKeyTrigger(VK_RETURN))
	//{
	//	Game::GetInstance()->ChangeScene(RESULT);
	//}
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

void Stage1Scene::BuildTestPages()
{
    m_TestPages.clear();

    // Page0: テスト1（左）
    {
        MessagePage p;
        p.speakerCharId = "miko";
        p.speakerName = "巫女";
        p.text = "テスト1";
        p.voiceId = "";
        p.focus = FocusSide::Left;
        m_TestPages.push_back(p);
    }

    // Page1: テスト2（右）
    {
        MessagePage p;
        p.speakerCharId = "kumo";
        p.speakerName = "女郎蜘蛛";
        p.text = "テスト2";
        p.voiceId = "";
        p.focus = FocusSide::Right;
        m_TestPages.push_back(p);
    }

    // Page2: テスト3（左）
    {
        MessagePage p;
        p.speakerCharId = "miko";
        p.speakerName = "巫女";
        p.text = "テスト3";
        p.voiceId = "";
        p.focus = FocusSide::Left;
        m_TestPages.push_back(p);
    }
}
