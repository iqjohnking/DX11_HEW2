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

    StagekillCount = 0;     //倒した敵の数をリセット
    StageEnemyCount = 30;   //ステージの敵の総数を設定

    //SoundFlg
    Conversation_BGM_flg_1 = false;
    Conversation_BGM_flg_2 = false;

    //BGM開始
    Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_002);
}

void Stage2::Uninit()
{
    if (m_Message)
    {
        m_Message->Stop();
    }

    m_Pages.clear();

    Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_002);

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
    UpdateEnemySpawn();
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
        break;
    }
}

void Stage2::GameUpdate()
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

    if (Input::GetKeyTrigger('D') || Input::GetButtonTrigger(XINPUT_LEFT_SHOULDER))   // 
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
    if (Input::GetKeyTrigger('J') || Input::GetKeyTrigger(VK_LEFT) || Input::GetButtonTrigger(XINPUT_RIGHT_SHOULDER))
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
                    StagekillCount++;
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
        m_Flow = Flow::EndTalk;     //一旦終了会話に飛ばす
    }

    //ステージクリアと失敗のチェック
    StageClearCheck();
    StageFailedCheck();

}

void Stage2::SoundUpdate()
{
    if (m_Flow == Flow::Gameplay && Conversation_BGM_flg_1 == false)
    {
        Conversation_BGM_flg_1 = true;
        //会話パートBGM停止
        Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_002);
        Game::GetSound()->Play(SOUND_LABEL_BGM_STAGE_000);
    }

    if (m_Flow == Flow::EndTalk && Conversation_BGM_flg_2 == false)
    {
        //StageBGM停止
        Game::GetSound()->Stop(SOUND_LABEL_BGM_STAGE_000);
        Conversation_BGM_flg_2 = true;
        Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_002);
    }
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

        p.focus = FocusSide::Right;

        // ★Page0必須：左右の初期表情
        p.leftFaceId = "normal";//蜘蛛初期表情
        p.rightFaceId = "normal";//巫女初期表情
        p.speakerFaceId = "surprised";//フォーカスしている話者のみ表情を変更
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

        p.focus = FocusSide::Left;
        p.speakerFaceId = "smile";

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

        p.focus = FocusSide::Right;

        // Page0必須：左右の初期表情
        p.leftFaceId = "normal";
        p.rightFaceId = "normal";
        p.speakerFaceId = "sad";

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

        p.focus = FocusSide::Left;
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

        p.focus = FocusSide::Right;
        p.speakerFaceId = "";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE2_END_002;

        m_Pages.push_back(p);
        //この程度で疲れるな、まだ２割も倒していない
    }
}


void Stage2::UpdateEnemySpawn()
{
    if (elapsedSeconds == 5 && phase1Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(100.0f, 420.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-100.0f, 420.0f, 0.0f));
        phase1Flag = true;
    }

    if (elapsedSeconds == 11 && phase2Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(350.0f, 200.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(250.0f, 300.0f, 0.0f));
        phase2Flag = true;
    }

    if (elapsedSeconds == 17 && phase3Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(200.0f, 400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(0.0f, 410.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-200.0f, 400.0f, 0.0f));
        phase3Flag = true;
    }

    if (elapsedSeconds == 22 && phase4Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(420.0f, -20.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(400.0f, -200.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(320.0f, -300.0f, 0.0f));
        phase4Flag = true;
    }

    if (elapsedSeconds == 27 && phase5Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(60.0f, -430.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-60.0f, -430.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-250.0f, 400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-350.0f, 320.0f, 0.0f));
        phase5Flag = true;
    }

    if (elapsedSeconds == 35 && phase6Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(350.0f, 330.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(250.0f, 400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-250.0f, -400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-350.0f, -300.0f, 0.0f));
        phase6Flag = true;
    }

    if (elapsedSeconds == 42 && phase7Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(420.0f, -50.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(350.0f, -250.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(200.0f, -350.0f, 0.0f));
        phase7Flag = true;
    }

    if (elapsedSeconds == 48 && phase8Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(450.0f, 100.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(380.0f, 250.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(250.0f, 370.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(50.0f, 420.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-150.0f, 420.0f, 0.0f));
        phase8Flag = true;
    }

    if (elapsedSeconds == 54 && phase9Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(200.0f, -400.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(50.0f, -410.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-370.0f, -100.0f, 0.0f));
        EnemySpawn(NORMAL, Vector3(-400.0f, 30.0f, 0.0f));
        phase9Flag = true;
    }
}

void Stage2::StageClearCheck()
{
    //敵を全て倒したかどうか
    if (StagekillCount >= StageEnemyCount)
    {
        m_Flow = Flow::EndTalk;
    }
}

void Stage2::StageFailedCheck()
{
    //ステージ失敗かどうか
    //巫女のHPが0になったら失敗にする
}
