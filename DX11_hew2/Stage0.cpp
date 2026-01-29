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
    //チュートリアル用に速度を0にセット
    m_Miko->SetMinSpeed(0.0f);
    m_Miko->SetAcceleration(0.0f);
    m_Miko->SetMaxSpeed(0.0f);

    // UI用の赤い糸表示
    m_UI_redSilk = Game::GetInstance()->AddObject<UI_redSilk>();
    m_MySceneObjects.emplace_back(m_UI_redSilk);
    m_UI_redSilk->SetHands(m_HandL, m_HandR);

    // UI用の赤い糸表示
    m_UI_mikoHp = Game::GetInstance()->AddObject<UI_mikoHp>();
    m_MySceneObjects.emplace_back(m_UI_mikoHp);
    m_UI_mikoHp->SetMiko(m_Miko);

    //経過したフレーム数と秒数を0にリセット
    elapsedFrames = 0;
    elapsedSeconds = 0;

    //敵が出現するフェーズのフラグをリセット
    phase1Flag = false;
    phase2Flag = false;
    phase3Flag = false;

    //画像フラグのリセット
    UI1Flag = false;
    UI2Flag = false;
    UI3Flag = false;
    UI4Flag = false;
    UI5Flag = false;
    UI6Flag = false;

    nowUI = 0;

    StagekillCount = 0;     //倒した敵の数をリセット
    StageEnemyCount = 3;   //ステージの敵の総数を設定

    m_ClearFlg = false;
    m_GameOverFlg = false;
    m_ChangeClearCount = 0;

    m_SelectIndex = 0;

    //BGM開始
    Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_000);
}

void Stage0::Uninit()
{
    if (m_Message)
    {
        m_Message->Stop();
    }

    m_Pages.clear();

    Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_010);

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
    SoundUpdate();
    UIUpdate();
    UpdateEnemySpawn();    
    // 終了会話が終わったらリザルトへ
    if (m_Flow == Flow::EndTalk)
    {
        if (!m_Message->IsPlaying())
        {
            Game::GetInstance()->ChangeScene(GAMEOVER);
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
            if (m_SilkWalls[i]->IsActive() == false)
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
            w->SetUID(m_SilkCount++);
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
            w->SetUID(m_SilkCount++);
        }
    }

    int count = 0;
    int minIndex = -1;
    int minUID = 0;
    for (int i = 0; i < 3; ++i)
    {
        auto* w = m_SilkWalls[i];
        if (!w) continue;
        //if (m_SilkWalls[i]->IsActive() == false) continue;
        if (w->IsActive())
        {
            ++count;
        }
        w->SetOldOne(false);   // 先全部清掉
        int uid = w->GetUID();
        if (minIndex == -1 || uid < minUID)
        {
            minUID = uid;
            minIndex = i;
        }
    }
    //  3 以上ときOldOneを設定 
    if (count > 2 && minIndex != -1)
    {

        m_SilkWalls[minIndex]->SetOldOne(true);
    }

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
        Game::GetInstance()->ChangeScene(START);
        //BuildEndPages();
        //m_Message->SetPages(m_Pages);
        //m_Message->Play();
        //m_Flow = Flow::EndTalk;     //一旦終了会話に飛ばす
    }

    if (Input::GetKeyTrigger(VK_SPACE) || Input::GetButtonTrigger(XINPUT_A))
    {
        if (UI1Flag == true && UI2Flag == false)
        {
            nowUI = 2;
        }
        if (UI3Flag == true && UI4Flag == false)
        {
            nowUI = 4;
        }
        if (UI5Flag == true && UI6Flag == false)
        {
            nowUI = 6;
        }
    }

    //ステージクリアと失敗のチェック
    StageClearCheck();
    StageFailedCheck();

}

void Stage0::SoundUpdate()
{
    if (m_Message->GetIndex() == 13 && m_Conversation_BGM_flg_1 == false)
    {
        Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_000);
        m_Conversation_BGM_flg_1 = true;
        //BGM開始
        Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_010);
    }
    if( m_Flow == Flow::Gameplay && m_Conversation_BGM_flg_1 == false)
    {
        Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_000);
        m_Conversation_BGM_flg_1 = true;
        //BGM開始
        Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_010);
	}
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
        p.speakerFaceId = "angry";

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
        p.speakerFaceId = "surprised";

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
        p.speakerFaceId = "normal";

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
        p.speakerFaceId = "smile";

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
        p.speakerFaceId = "normal";

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
        p.speakerFaceId = "glare";

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
        p.speakerFaceId = "angry";

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
        p.speakerFaceId = "normal";

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
        p.speakerFaceId = "surprised";

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
        p.speakerFaceId = "angry";

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
        p.speakerFaceId = "glare";

        p.voiceLabel = SOUND_LABEL_VOICE_STAGE0_START_019;

        m_Pages.push_back(p);
        //…チッ…
    }
}

void Stage0::BuildEndPages()
{
    m_Pages.clear();
}

void Stage0::UIUpdate()
{
    if (m_Flow != Flow::Gameplay) return;

    if (UI1Flag == false)
    {
        Game::GetInstance()->SetWorldStopped(true);

        m_tutorialpic1 = Game::GetInstance()->AddObject<Texture2D>();
        m_tutorialpic1->SetTexture("assets/texture/ui/tutorialhint000.png");
        m_tutorialpic1->SetPosition(0.0f, 0.0f, -1.0f);
        m_tutorialpic1->SetScale(1920.0f, 1080.0f, 0.0f);
        m_tutorialpic1->SetDrawOrder(10000);
        m_MySceneObjects.emplace_back(m_tutorialpic1);

        UI1Flag = true;
    }

    if (phase1Flag == true && UI2Flag == false)
    {
        //前のUI非表示
        m_tutorialpic1->SetScale(0.0f, 0.0f, 0.0f);

        Game::GetInstance()->SetWorldStopped(false);

        //チュートリアル用矢印
        m_tutorialarrow_sankaku = Game::GetInstance()->AddObject<Texture2D>();
        m_tutorialarrow_sankaku->SetTexture("assets/texture/ui/tutorialarrow001.png");
        m_tutorialarrow_sankaku->SetPosition(0.0f, 50.0f, -1.0f);
        m_tutorialarrow_sankaku->SetScale(1920.0f, 1080.0f, 0.0f);
        m_tutorialarrow_sankaku->SetDrawOrder(10000);
        m_MySceneObjects.emplace_back(m_tutorialarrow_sankaku);

        //テキスト
        m_tutorialstagetext000 = Game::GetInstance()->AddObject<Texture2D>();
        m_tutorialstagetext000->SetTexture("assets/texture/ui/tutorialstagetext000.png");
        m_tutorialstagetext000->SetPosition(700.0f, 400.0f, -1.0f);
        m_tutorialstagetext000->SetScale(499.0f, 196.0f, 0.0f);
        m_tutorialstagetext000->SetDrawOrder(10000);
        m_MySceneObjects.emplace_back(m_tutorialstagetext000);

        UI2Flag = true;
    }

    if (StagekillCount == 1 && UI3Flag == false)
    {
        //Scaleを0にして前のUIを非表示にする
        m_tutorialarrow_sankaku->SetScale(0.0f, 0.0f, 0.0f);
        m_tutorialstagetext000->SetScale(0.0f, 0.0f, 0.0f);

        Game::GetInstance()->SetWorldStopped(true);

        m_tutorialpic2 = Game::GetInstance()->AddObject<Texture2D>();
        m_tutorialpic2->SetTexture("assets/texture/ui/tutorialhint001.png");
        m_tutorialpic2->SetPosition(0.0f, 0.0f, -1.0f);
        m_tutorialpic2->SetScale(1920.0f, 1080.0f, 0.0f);
        m_tutorialpic2->SetDrawOrder(10000);
        m_MySceneObjects.emplace_back(m_tutorialpic2);

        UI3Flag = true;
    }

    if (phase2Flag == true && UI4Flag == false)
    {
        //前のUI非表示
        m_tutorialpic2->SetScale(0.0f, 0.0f, 0.0f);

        Game::GetInstance()->SetWorldStopped(false);

        //チュートリアル用矢印
        m_tutorialarrow = Game::GetInstance()->AddObject<Texture2D>();
        m_tutorialarrow->SetTexture("assets/texture/ui/tutorialarrow000.png");
        m_tutorialarrow->SetPosition(0.0f, 0.0f, -1.0f);
        m_tutorialarrow->SetScale(1920.0f, 1080.0f, 0.0f);
        m_tutorialarrow->SetDrawOrder(10000);
        m_MySceneObjects.emplace_back(m_tutorialarrow);

        //テキスト
        m_tutorialstagetext001 = Game::GetInstance()->AddObject<Texture2D>();
        m_tutorialstagetext001->SetTexture("assets/texture/ui/tutorialstagetext001.png");
        m_tutorialstagetext001->SetPosition(700.0f, 400.0f, -1.0f);
        m_tutorialstagetext001->SetScale(499.0f, 196.0f, 0.0f);
        m_tutorialstagetext001->SetDrawOrder(10000);
        m_MySceneObjects.emplace_back(m_tutorialstagetext001);

        UI4Flag = true;
    }

    if (StagekillCount == 2 && UI5Flag == false)
    {
        //Scaleを0にして前のUIを非表示にする
        m_tutorialarrow->SetScale(0.0f, 0.0f, 0.0f);
        m_tutorialstagetext001->SetScale(0.0f, 0.0f, 0.0f);

        Game::GetInstance()->SetWorldStopped(true);

        m_tutorialpic3 = Game::GetInstance()->AddObject<Texture2D>();
        m_tutorialpic3->SetTexture("assets/texture/ui/tutorialhint002.png");
        m_tutorialpic3->SetPosition(0.0f, 0.0f, -1.0f);
        m_tutorialpic3->SetScale(1920.0f, 1080.0f, 0.0f);
        m_tutorialpic3->SetDrawOrder(10000);
        m_MySceneObjects.emplace_back(m_tutorialpic3);

        UI5Flag = true;
    }

    if (phase3Flag == true && UI6Flag == false)
    {
		//Scaleを0にして前のUIを非表示にする
        m_tutorialpic3->SetScale(0.0f, 0.0f, 0.0f);

        Game::GetInstance()->SetWorldStopped(false);

        //テキスト
        m_tutorialstagetext002 = Game::GetInstance()->AddObject<Texture2D>();
        m_tutorialstagetext002->SetTexture("assets/texture/ui/tutorialstagetext002.png");
        m_tutorialstagetext002->SetPosition(700.0f, 400.0f, -1.0f);
        m_tutorialstagetext002->SetScale(499.0f, 196.0f, 0.0f);
        m_tutorialstagetext002->SetDrawOrder(10000);
        m_MySceneObjects.emplace_back(m_tutorialstagetext002);

        UI6Flag = true;
    }

	//最後の敵を倒したらテキストを非表示にする
    if (StagekillCount == 3)
    {
        m_tutorialstagetext002->SetScale(0.0f, 0.0f, 0.0f);
    }
}

void Stage0::UpdateEnemySpawn()
{
    if (nowUI == 2 && phase1Flag == false)
    {
        EnemySpawnSpeedzero(NORMAL, Vector3(0.0f, 150.0f, 0.0f));   //動かない敵をスポーンさせる
        phase1Flag = true;
    }

    if (nowUI == 4 && phase2Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(-30.0f, 350.0f, 0.0f));
        phase2Flag = true;
    }

    if (nowUI == 6 && phase3Flag == false)
    {
        EnemySpawn(NORMAL, Vector3(0.0f, 300.0f, 0.0f));
        //巫女の速度を元に戻す
        m_Miko->SetMinSpeed(0.0f);
        m_Miko->SetAcceleration(0.2f);
		m_Miko->SetMaxSpeed(10.0f);
        phase3Flag = true;
    }
}

void Stage0::StageClearCheck()
{
    //敵を全て倒したかどうか
    if (StagekillCount >= StageEnemyCount)
    {
        m_Flow = Flow::EndTalk;
    }

   // Game::GetInstance()->SetMaxClearedStage(1);


}

void Stage0::StageFailedCheck()
{
    //ステージ失敗かどうか
    //巫女のHPが0になったら失敗にする
    //チュートリアルでは死んだら死んだウェーブから再開とのこと
}

//チュートリアルで速度0の敵をスポーンさせる関数
void Stage0::EnemySpawnSpeedzero(EnemyType enemyType, DirectX::SimpleMath::Vector3 pos)
{
    switch (enemyType)
    {
    case NORMAL:
        for (int i = 0; i < 60; i++)
        {
            if (Enemy1List[i] != nullptr)
                continue;
            Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
            Enemy1List[i]->SetTarget(m_Miko);
            Enemy1List[i]->SetField(m_Field);
            Enemy1List[i]->SetPosition(pos);
			Enemy1List[i]->SetMinSpeed(0.0f); //速度0に設定、チュートリアル用
			Enemy1List[i]->SetAcceleration(0.0f); //加速度0に設定、チュートリアル用
			Enemy1List[i]->SetMaxSpeed(0.0f); //速度0に設定、チュートリアル用
            m_MySceneObjects.emplace_back(Enemy1List[i]);
            break;
    
        }
        break;

    default:
        break;
    }
}