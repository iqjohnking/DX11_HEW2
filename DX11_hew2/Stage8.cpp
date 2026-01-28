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

    // 背景
    auto* bg = Game::GetInstance()->AddObject<TitleBG>();
    m_MySceneObjects.emplace_back(bg);
    bg->Texture2D::SetPosition(0.0f, 0.0f, 2.0f); // Z順序を最後に

    // FIELD
    m_Field = Game::GetInstance()->AddObject<Field>();
    m_MySceneObjects.emplace_back(m_Field);

    // silkWall*3
    for (int i = 0; i < 3; ++i)
    {
        m_SilkWalls[i] = Game::GetInstance()->AddObject<silkWall>();
        m_MySceneObjects.emplace_back(m_SilkWalls[i]);
    }

    // 左手（W / S ）
    m_HandL = Game::GetInstance()->AddObject<playerHand>(0);
    m_MySceneObjects.emplace_back(m_HandL);

    // 右手（↑ / ↓ ）
    m_HandR = Game::GetInstance()->AddObject<playerHand>(1);
    m_MySceneObjects.emplace_back(m_HandR);

    m_HandL->SetAnotherHand(m_HandR);
    //m_HandL->SetField(m_Field);
    m_HandR->SetAnotherHand(m_HandL);
    //m_HandR->SetField(m_Field);

    ////巫女
    m_Miko = Game::GetInstance()->AddObject<Shrinemaiden>();
    m_MySceneObjects.emplace_back(m_Miko);
    m_Miko->SetField(m_Field);

    //経過したフレーム数と秒数を0にリセット
    elapsedFrames = 0;
    elapsedSeconds = 0;

    //敵が出現するフェーズのフラグをリセット
    phase1Flag = false;
    phase2Flag = false;
    phase3Flag = false;
    phase4Flag = false;
    phase5Flag = false;
    phase6Flag = false;
    phase7Flag = false;
    phase8Flag = false;
    phase9Flag = false;
    phase10Flag = false;
    phase11Flag = false;
    phase12Flag = false;
    phase13Flag = false;

    StagekillCount = 0;     //倒した敵の数をリセット
    StageEnemyCount = 56;   //ステージの敵の総数を設定

    //BGM開始
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
    UpdateEnemySpawn();
    SoundUpdate();
    // 終了会話が終わったらリザルトへ
    if (m_Flow == Flow::EndTalk)
    {
        if (!m_Message->IsPlaying())
        {
            Game::GetInstance()->ChangeScene(GAMEOVER);
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

    //m_MySceneObjects中の空間オブジェクトを削除する（erase）
    for (auto it = m_MySceneObjects.begin(); it != m_MySceneObjects.end(); )
    {
        Object* o = *it; // オブジェクト取得
        if (!o)
        {
            it = m_MySceneObjects.erase(it);
            continue;
        }

        if (o->ToBeDeleted())
        {
            Game::GetInstance()->DeleteObject(o);   // ★実体も破棄依頼
            it = m_MySceneObjects.erase(it);        // ★リストからも除去
            continue;
        }

        ++it;
    }

    //60フレーム経過するごとに1秒プラス
    elapsedFrames++;
    elapsedSeconds = elapsedFrames / 60;

    //-----------------------------------------------------------------------------
    // 操作／INPUT
    //-----------------------------------------------------------------------------

    float rt = Input::GetRightTrigger();
    float lt = Input::GetLeftTrigger();
    static bool prevLT = false;
    static bool prevRT = false;
    bool nowLT = (lt >= 0.5f);
    bool nowRT = (rt >= 0.5f);
    bool ltTriggerOnce = (nowLT && !prevLT);
    bool rtTriggerOnce = (nowRT && !prevRT);
    prevLT = nowLT;
    prevRT = nowRT;

    if (Input::GetKeyTrigger('D') || Input::GetButtonTrigger(XINPUT_LEFT_SHOULDER) || ltTriggerOnce)   // 
    {
        silkWall* w = nullptr;

        // 1. まず、非アクティブ（消えている）スロットを探す
        for (int i = 0; i < 3; ++i)
        {
            if (!m_SilkWalls[i]->IsActive())
            {
                w = m_SilkWalls[i];
                break;
            }
        }

        // 2. もし全部埋まっていたら、一番古いもの(m_NextSilkIndex)を上書きする
        if (w == nullptr)
        {
            w = m_SilkWalls[m_NextSilkIndex];
            m_NextSilkIndex = (m_NextSilkIndex + 1) % 3;
        }

        // 3. 発射実行
        if (w && m_HandL && m_HandR)
        {
            w->Fire(m_HandL->GetPosition(), m_HandR->GetPosition());
        }
    }

    // 
    if (Input::GetKeyTrigger('J') || Input::GetKeyTrigger(VK_LEFT) || Input::GetButtonTrigger(XINPUT_RIGHT_SHOULDER) || rtTriggerOnce)
    {
        silkWall* w = nullptr;

        // 1. まず、非アクティブ（消えている）スロットを探す
        for (int i = 0; i < 3; ++i)
        {
            if (!m_SilkWalls[i]->IsActive())
            {
                w = m_SilkWalls[i];
                break;
            }
        }

        // 2. もし全部埋まっていたら、一番古いもの(m_NextSilkIndex)を上書きする
        if (w == nullptr)
        {
            w = m_SilkWalls[m_NextSilkIndex];
            m_NextSilkIndex = (m_NextSilkIndex + 1) % 3;
        }

        // 3. 発射実行
        if (w && m_HandL && m_HandR)
        {
            Vector3 startPos = m_HandR->GetPosition();  // 右手
            Vector3 targetPos = m_HandL->GetPosition(); // 左手

            w->Fire(startPos, targetPos);
        }
    }

    //if (Input::GetKeyTrigger('R'))   // 
    //{
    //	std::vector<Object*> removeList;

    //	//Enemyを探す
    //	for (auto* obj : m_MySceneObjects)
    //	{
    //		if (dynamic_cast<EnemyBase*>(obj))
    //		{
    //			removeList.push_back(obj);
    //		}
    //	}

    //	//　見つけたEnemy1を削除する
    //	for (auto* obj : removeList)
    //	{
    //		Game::GetInstance()->DeleteObject(obj);

    //		auto it = std::find(m_MySceneObjects.begin(), m_MySceneObjects.end(), obj);
    //		if (it != m_MySceneObjects.end())
    //		{
    //			m_MySceneObjects.erase(it);
    //		}
    //	}
    //}

    for (int i = 0; i < 3; ++i)
    {
        silkWall* wall = m_SilkWalls[i];
        if (!wall) continue;              // 防禦性チェック
    }

    //-----------------------------------------------------------------------------
    // silkWallの三角形判定
    //-----------------------------------------------------------------------------
    Vector3 A, B, C; // 三角形頂点

    silkWall* walls[3] = { m_SilkWalls[0], m_SilkWalls[1], m_SilkWalls[2] };

    // 3 本とも準備完了しているか？
    const bool allReady = std::all_of(std::begin(walls), std::end(walls),
        [](const silkWall* w) { return w && !w->IsGrowing(); });


    if (allReady)
    {
        // 3 本の silkWall から三角形生成を試行
        if (TriangleSilk::TryMakeTriangleFromWallsXY(walls[0], walls[1], walls[2], A, B, C))
        {
            int eliminatedCount = 0;
            bool mikoMayuCount = false;

            // 敵を調べる
            for (auto* obj : m_MySceneObjects)
            {
                if (!obj) continue;
                if (obj->ToBeDeleted()) continue;

                auto* enemy = dynamic_cast<EnemyBase*>(obj);
                if (!enemy) continue;

                const auto pos = enemy->GetPosition();
                if (TriangleSilk::IsInsideTriangleXY(pos, A, B, C))
                {
                    Vector3 centroid = (A + B + C) / 3.0f;
                    enemy->StartMayuing(centroid);
                    ++eliminatedCount;
                }
            }

            // 巫女も調べる
            if (m_Miko)
            {
                const auto mikoPos = m_Miko->GetPosition();
                if (TriangleSilk::IsInsideTriangleXY(mikoPos, A, B, C))
                {
                    mikoMayuCount = true;
                }
            }

            // Mayu を生成する
            if (eliminatedCount > 0)
            {
                const float baseRadius = 25.0f;
                const float perKill = 5.0f;
                const float mayuRadius = baseRadius + perKill * static_cast<float>(eliminatedCount);

                auto* mayu = Game::GetInstance()->AddObject<EnemyMayu>();
                Vector3 centroid = (A + B + C) / 3.0f;
                mayu->SetPosition(centroid);
                mayu->SetRadius(mayuRadius);
                m_MySceneObjects.emplace_back(mayu);
            }

            // 巫女が三角形に入ったときの処理
            if (mikoMayuCount && m_Miko)
            {
                Vector3 centroid = (A + B + C) / 3.0f;
                m_Miko->SetStartMayuing(centroid);
            }

            for (int i = 0; i < 3; ++i)
            {
                m_SilkWalls[i]->reInit();
            }
        }
    }


    //-----------------------------------------------------------------------------
    // silkWall と　mayu の当たり判定
    //-----------------------------------------------------------------------------

    //MAYUのなかにいるから、こちはなにもしない

    for (auto it = m_MySceneObjects.begin(); it != m_MySceneObjects.end(); )
    {
        Object* o = *it; // オブジェクト取得
        if (!o || o->ToBeDeleted())
        {
            Game::GetInstance()->DeleteObject(o); // オブジェクト削除
            it = m_MySceneObjects.erase(it); // イテレータを更新
        }
        else
        {
            ++it; // 次へ
        }
    }

    if (m_Miko->GetDYINGTimer() <= 0) {
        BuildEndPages();
        m_Message->SetPages(m_Pages);
        m_Message->Play();
        m_Flow = Flow::EndTalk;     //一旦終了会話に飛ばす
    }

    //ステージクリアと失敗のチェック
    StageClearCheck();
    StageFailedCheck();

}

void Stage8::SoundUpdate()
{
    if (m_Flow == Flow::Gameplay && m_Conversation_BGM_flg_1 == false)
    {
        m_Conversation_BGM_flg_1 = true;
        Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_007);
        Game::GetSound()->Play(SOUND_LABEL_BGM_STAGE_002);
    }

    if (m_Flow == Flow::EndTalk && m_Conversation_BGM_flg_2 == false)
    {
        m_Conversation_BGM_flg_2 = true;
        Game::GetSound()->Stop(SOUND_LABEL_BGM_STAGE_002);
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
    if (elapsedSeconds == 5 && phase1Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(150.0f, 400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(0.0f, 450.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-150.0f, 400.0f, 0.0f));
        phase1Flag = true;
    }

    if (elapsedSeconds == 9 && phase2Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(-200.0f, 350.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-300.0f, 300.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-350.0f, 200.0f, 0.0f));
        phase2Flag = true;
    }

    if (elapsedSeconds == 13 && phase3Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(350.0f, 200.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(300.0f, 300.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(200.0f, 350.0f, 0.0f));
        phase3Flag = true;
    }

    if (elapsedSeconds == 16 && phase4Flag == false)
    {
        EnemySpawn(TACKLE, Vector3(350.0f, -300.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(150.0f, 420.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(0.0f, 450.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-150.0f, 420.0f, 0.0f));
        EnemySpawn(TACKLE, Vector3(-350.0f, -300.0f, 0.0f));
        phase4Flag = true;
    }

    if (elapsedSeconds == 20 && phase5Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(400.0f, 150.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(400.0f, -150.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(350.0f, 250.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(350.0f, -250.0f, 0.0f));
        phase5Flag = true;
    }

    if (elapsedSeconds == 24 && phase6Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(400.0f, -150.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(350.0f, -300.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(200.0f, -400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-200.0f, 400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-300.0f, 300.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-400.0f, 150.0f, 0.0f));
        phase6Flag = true;
    }

    if (elapsedSeconds == 28 && phase7Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(-200.0f, -400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-260.0f, -320.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-320.0f, -240.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-380.0f, -160.0f, 0.0f));
        phase7Flag = true;
    }

    if (elapsedSeconds == 32 && phase8Flag == false)
    {
        EnemySpawn(TACKLE, Vector3(300.0f, 200.0f, 0.0f));
        EnemySpawn(TACKLE, Vector3(200.0f, 300.0f, 0.0f));
        EnemySpawn(TACKLE, Vector3(-200.0f, 300.0f, 0.0f));
        EnemySpawn(TACKLE, Vector3(-300.0f, 200.0f, 0.0f));
        phase8Flag = true;
    }

    if (elapsedSeconds == 36 && phase9Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(350.0f, -150.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(300.0f, -250.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(250.0f, -350.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-250.0f, -350.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-300.0f, -250.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-350.0f, -150.0f, 0.0f));
        phase9Flag = true;
    }

    if (elapsedSeconds == 40 && phase10Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(400.0f, 50.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(350.0f, 200.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(300.0f, 300.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(250.0f, 400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(130.0f, 420.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(20.0f, 450.0f, 0.0f));
        phase10Flag = true;
    }

    if (elapsedSeconds == 43 && phase11Flag == false)
    {
        EnemySpawn(TACKLE, Vector3(-250.0f, -350.0f, 0.0f));
        EnemySpawn(TACKLE, Vector3(-400.0f, -150.0f, 0.0f));
        phase11Flag = true;
    }

    if (elapsedSeconds == 47 && phase12Flag == false)
    {
        EnemySpawn(TACKLE, Vector3(250.0f, 300.0f, 0.0f));
        EnemySpawn(TACKLE, Vector3(-250.0f, 300.0f, 0.0f));
        phase12Flag = true;
    }

    if (elapsedSeconds == 52 && phase13Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(-150.0f, 400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-200.0f, -350.0f, 0.0f));
        EnemySpawn(TACKLE, Vector3(-250.0f, 280.0f, 0.0f));
        EnemySpawn(TACKLE, Vector3(-250.0f, -250.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-300.0f, 350.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-300.0f, -300.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-400.0f, 150.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-400.0f, -150.0f, 0.0f));
        phase13Flag = true;
    }
}

void Stage8::StageClearCheck()
{
    //敵を全て倒したかどうか
    if (StagekillCount >= StageEnemyCount)
    {
        BuildEndPages();
        m_Message->SetPages(m_Pages);
        m_Message->Play();
        m_Flow = Flow::EndTalk;
    }

    Game::GetInstance()->SetMaxClearedStage(8);

}

void Stage8::StageFailedCheck()
{
    //ステージ失敗かどうか
    //巫女のHPが0になったら失敗にする
    /*
    BuildEndPages();
    m_Message->SetPages(m_Pages);
    m_Message->Play();
    m_Flow = Flow::EndTalk;
    */
}