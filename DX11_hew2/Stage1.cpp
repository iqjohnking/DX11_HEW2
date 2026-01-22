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
	MessageUpdate();
	GameUpdate();
}

//会話パートのUpdate
void Stage1::MessageUpdate()
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

//ゲームパートのUpdate
void Stage1::GameUpdate()
{
    if(m_Flow != Flow::Gameplay) return;

	//60フレーム経過するごとに1秒プラス
	elapsedFrames++;
	elapsedSeconds = elapsedFrames / 60;

	if (elapsedSeconds == 5 && phase1Flag == false)	//5秒経過かつフェーズ1が未実行なら
	{
		//EnemySpawn();	//関数で敵をスポーンさせるようにしたい

		for (int i = 0; i < 1; ++i)
		{
			Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
			Enemy1List[i]->SetTarget(m_Miko);
			Enemy1List[i]->SetField(m_Field);
			Enemy1List[i]->SetPosition({ 0.0f, 350.0f, 0.0f });
						
			m_MySceneObjects.emplace_back(Enemy1List[i]);
		}
		phase1Flag = true;
	}

	if (elapsedSeconds == 12 && phase2Flag == false)	//12秒経過かつフェーズ2が未実行なら
	{
		//EnemySpawn();	//関数で敵をスポーンさせるようにしたい

		for (int i = 0; i < 1; ++i)
		{
			Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
			Enemy1List[i]->SetTarget(m_Miko);
			Enemy1List[i]->SetField(m_Field);
			Enemy1List[i]->SetPosition({ 400.0f, 50.0f, 0.0f });

			m_MySceneObjects.emplace_back(Enemy1List[i]);
		}
		phase2Flag = true;
	}

	if (elapsedSeconds == 18 && phase3Flag == false)	//18秒経過かつフェーズ3が未実行なら
	{
		//EnemySpawn();	//関数で敵をスポーンさせるようにしたい

		for (int i = 0; i < 2; ++i)
		{
			Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
			Enemy1List[i]->SetTarget(m_Miko);
			Enemy1List[i]->SetField(m_Field);
			Enemy1List[0]->SetPosition({ 200.0f, 300.0f, 0.0f });
			Enemy1List[1]->SetPosition({ 350.0f, 200.0f, 0.0f });

			m_MySceneObjects.emplace_back(Enemy1List[i]);
		}
		phase3Flag = true;
	}

	if (elapsedSeconds == 25 && phase4Flag == false)	//25秒経過かつフェーズ4が未実行なら
	{
		//EnemySpawn();	//関数で敵をスポーンさせるようにしたい

		for (int i = 0; i < 2; ++i)
		{
			Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
			Enemy1List[i]->SetTarget(m_Miko);
			Enemy1List[i]->SetField(m_Field);
			Enemy1List[0]->SetPosition({ 50.0f, -400.f, 0.0f });
			Enemy1List[1]->SetPosition({ -50.0f, -400.f, 0.0f });

			m_MySceneObjects.emplace_back(Enemy1List[i]);
		}
		phase4Flag = true;
	}

	if (elapsedSeconds == 32 && phase5Flag == false)	//32秒経過かつフェーズ5が未実行なら
	{
		//EnemySpawn();	//関数で敵をスポーンさせるようにしたい

		for (int i = 0; i < 2; ++i)
		{
			Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
			Enemy1List[i]->SetTarget(m_Miko);
			Enemy1List[i]->SetField(m_Field);
			Enemy1List[0]->SetPosition({ 450.0f, -150.0f, 0.0f });
			Enemy1List[1]->SetPosition({ 300.0f, -350.0f, 0.0f });

			m_MySceneObjects.emplace_back(Enemy1List[i]);
		}
		phase5Flag = true;
	}

	if (elapsedSeconds == 39 && phase6Flag == false)	//39秒経過かつフェーズ6が未実行なら
	{
		//EnemySpawn();	//関数で敵をスポーンさせるようにしたい

		for (int i = 0; i < 2; ++i)
		{
			Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
			Enemy1List[i]->SetTarget(m_Miko);
			Enemy1List[i]->SetField(m_Field);
			Enemy1List[0]->SetPosition({ 450.0f, -130.0f, 0.0f });
			Enemy1List[1]->SetPosition({ 30.0f, -420.0f, 0.0f });

			m_MySceneObjects.emplace_back(Enemy1List[i]);
		}

		for (int i = 0; i < 1; ++i)
		{
			Enemy2List[i] = Game::GetInstance()->AddObject<Enemy2>();
			Enemy2List[i]->SetTarget(m_Miko);
			Enemy2List[i]->SetField(m_Field);
			Enemy2List[i]->SetPosition({ 300.0f, -350.0f, 0.0f });

			m_MySceneObjects.emplace_back(Enemy2List[i]);
		}
		phase6Flag = true;
	}
	
    //-----------------------------------------------------------------------------
	// 操作／INPUT
	//-----------------------------------------------------------------------------

	if (Input::GetKeyTrigger('D') || Input::GetButtonTrigger(XINPUT_LEFT_SHOULDER))   // 
	{
		silkWall* w = m_SilkWalls[m_NextSilkIndex];
		if (w && m_HandL && m_HandR)
		{
			Vector3 startPos = m_HandL->GetPosition();  // 左手
			Vector3 targetPos = m_HandR->GetPosition(); // 右手
			w->Fire(startPos, targetPos);

			// 用掉一條絲 → 往下一個 index
			++m_NextSilkIndex;
			if (m_NextSilkIndex >= 3)
			{
				m_NextSilkIndex = 0;
			}
		}
	}

	// 
	if (Input::GetKeyTrigger('J') || Input::GetKeyTrigger(VK_LEFT) || Input::GetButtonTrigger(XINPUT_RIGHT_SHOULDER))
	{
		silkWall* w = m_SilkWalls[m_NextSilkIndex];
		if (w && m_HandL && m_HandR)
		{
			Vector3 startPos = m_HandR->GetPosition();  // 右手
			Vector3 targetPos = m_HandL->GetPosition(); // 左手
			w->Fire(startPos, targetPos);

			++m_NextSilkIndex;
			if (m_NextSilkIndex >= 3)
			{
				m_NextSilkIndex = 0;
			}
		}
	}

	if (Input::GetKeyTrigger('R'))   // 
	{
		std::vector<Object*> removeList;

		//Enemyを探す
		for (auto* obj : m_MySceneObjects)
		{
			if (dynamic_cast<EnemyBase*>(obj))
			{
				removeList.push_back(obj);
			}
		}

		//　見つけたEnemy1を削除する
		for (auto* obj : removeList)
		{
			Game::GetInstance()->DeleteObject(obj);

			auto it = std::find(m_MySceneObjects.begin(), m_MySceneObjects.end(), obj);
			if (it != m_MySceneObjects.end())
			{
				m_MySceneObjects.erase(it);
			}
		}
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


	// nullptr チェックのみ（就緒判定は行わない）
	if (allReady)
	{
		// 3 本の silkWall から三角形生成を試行
		// false の場合は三角形が構成できなかった（平行／交差なし／面積ゼロなど）
		if (TriangleSilk::TryMakeTriangleFromWallsXY(walls[0], walls[1], walls[2], A, B, C))
		{
			int eliminatedCount = 0;
			//std::vector<Object*> toRemove; // 倒す予定リスト

			// 敵を調べて、三角形内にいるやつを倒す予定リストに格納する
			for (auto* obj : m_MySceneObjects)
			{
				auto* enemy = dynamic_cast<EnemyBase*>(obj);
				if (!enemy) continue;

				const auto pos = enemy->GetPosition();
				if (TriangleSilk::IsInsideTriangleXY(pos, A, B, C))
				{
					//toRemove.push_back(obj);
					Vector3 centroid = (A + B + C) / 3.0f;
					enemy->StartMayuing(centroid);
					++eliminatedCount;
				}
			}

			// Mayu を生成する
			if (eliminatedCount > 0)
			{
				// 半径の設計例：基準 25 + 1体ごとに +5
				const float baseRadius = 25.0f;
				const float perKill = 5.0f;
				const float mayuRadius = baseRadius + perKill * static_cast<float>(eliminatedCount);

				auto* mayu = Game::GetInstance()->AddObject<EnemyMayu>();
				// 生成位置は三角形の重心に配置（必要なら別ロジックに変更）
				Vector3 centroid = (A + B + C) / 3.0f;
				mayu->SetPosition(centroid);
				mayu->SetRadius(mayuRadius); // 半径設定（後述の連動対応が必要）
				m_MySceneObjects.emplace_back(mayu);
			}

			// 倒す予定の敵を削除する
			//アニメーションの追加する必要あるか？
			//for (auto* obj : toRemove)
			//{
			//	Game::GetInstance()->DeleteObject(obj);
			//	auto it = std::find(m_MySceneObjects.begin(), m_MySceneObjects.end(), obj);
			//	if (it != m_MySceneObjects.end())
			//	{
			//		m_MySceneObjects.erase(it);
			//	}
			//}

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

	//-----------------------------------------------------------------------------
	// エンターキーを押してステージ1へ
	//-----------------------------------------------------------------------------
	if (Input::GetKeyTrigger(VK_RETURN))	//クリア条件達成でm_Flowを変えるようにする
	{
		/*
		Game::GetInstance()->ChangeScene(STAGE1);

		return;
		*/
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

//実行すると敵がスポーン
void Stage1::EnemySpawn()
{
	
}

void Stage1::StageClearCheck()
{
	//クリア条件を達成しているかどうか
	//達成していたらm_Flowを変える	
}

void Stage1::StageFailedCheck()
{
	//ステージ失敗かどうか
}