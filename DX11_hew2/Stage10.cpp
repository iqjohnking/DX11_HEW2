#include "Stage10.h"
#include "Game.h"

void Stage10::Init()
{
	//// まず開始会話の台本を作る
	//BuildStartPages();

	//// 司令塔をObjectとして追加
	//m_Message = Game::GetInstance()->AddObject<MessageManager>();
	//m_MySceneObjects.emplace_back(m_Message);

	//// 会話参加者の指定（キャラID）
	//m_Message->SetParticipants("kumo", "miko");

	//// 会話素材の指定
	//m_Message->SetFramePath("assets/texture/Message/UI/MessageUI.png");

	//m_Message->SetBackgroundPath("assets/texture/Message/bg/bg_stage1.jpg");

	//// 台本セット
	//m_Message->SetPages(m_Pages);

	//// 開始時
	//m_Message->Play();

	//m_Flow = Flow::StartTalk;

	m_Flow = Flow::Gameplay;

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

	// UI用の赤い糸表示
	m_UI_redSilk = Game::GetInstance()->AddObject<UI_redSilk>();
	m_MySceneObjects.emplace_back(m_UI_redSilk);
	m_UI_redSilk->SetHands(m_HandL, m_HandR);

	// UI用の赤い糸表示
	m_UI_mikoHp = Game::GetInstance()->AddObject<UI_mikoHp>();
	m_MySceneObjects.emplace_back(m_UI_mikoHp);
	m_UI_mikoHp->SetMiko(m_Miko);
	m_SilkCount = 0;

	// UI 用の操作説明表示
	m_UI_control = Game::GetInstance()->AddObject<UI_control>();
	m_MySceneObjects.emplace_back(m_UI_control);

	/*
	//敵が出現するフェーズのフラグをリセット
	phase1Flag = false;
	phase2Flag = false;
	phase3Flag = false;
	phase4Flag = false;
	phase5Flag = false;
	phase6Flag = false;
	*/

	//経過したフレーム数と秒数を0にリセット
	elapsedFrames = 0;
	elapsedSeconds = 0;
	survivalTime = 0;

	//スコアをリセット
	Score = 0;

	survivalTime_100 = 0; //生存時間100の位
	survivalTime_10 = 0; //生存時間10の位
	survivalTime_1 = 0; //生存時間1の位

	Score_10000 = 0;     //スコア10000の位
	Score_1000 = 0;     //スコア1000の位
	Score_100 = 0;      //スコア100の位
	Score_10 = 0;       //スコア10の位
	Score_1 = 0;        //スコア1の位

	EnemySpawnFlag = false;

	m_GameOverFlg = false;
	m_GameOverImageFlg = false;

	m_SelectIndex = 0;

	m_GameUpdateBlock = false;

	StagekillCount = 0;     //倒した敵の数をリセット
	//StageEnemyCount = 11;   //ステージの敵の総数を設定（エンドレスなので不要）

	//BGM開始
	Game::GetSound()->Play(SOUND_LABEL_BGM_STAGE_000);
}

void Stage10::Uninit()
{
	if (m_Message)
	{
		m_Message->Stop();
	}

	Game::GetSound()->Stop(SOUND_LABEL_BGM_STAGE_000);

	m_Pages.clear();

	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}

void Stage10::Update()
{
	if (m_GameUpdateBlock != true)
	{
		MessageUpdate();
		GameUpdate();
		UpdateEnemySpawn();
	}

	IssueUpdate();

}

void Stage10::MessageUpdate()
{
	if (!m_Message) return;

	switch (m_Flow)
	{
	case Flow::StartTalk:
		// 開始会話が終わったらゲーム開始へ
		//if (!m_Message->IsPlaying())
		//{
		//	
		//}
		m_Flow = Flow::Gameplay;
		break;

	case Flow::Gameplay:
		break;

	case Flow::EndTalk:
		break;
	}
}

void Stage10::GameUpdate()
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

	//30秒経過するごとに出現する敵の数を増やす
	EnemySpawnplus = elapsedSeconds / 24;

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
			ScoreMultiplier = 0.9f; //スコア倍率リセット

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
					ScoreMultiplier += 0.1f;	//1体目は1倍、以降0.1倍ずつ倍率アップ
					Score += baseScore * ScoreMultiplier;	//100*スコア倍率
					if (Score >= 99999)
					{
						Score = 99999; //スコアカンスト
					}
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

	StageFailedCheck();

}

void Stage10::BuildStartPages()
{
	m_Pages.clear();

	//// Page0:
	//// 右=巫女,左=女郎蜘蛛
	//{
	//    MessagePage p;

	//    // ★必須：このページの表示（名前＋本文）
	//    p.nameId = "miko";//表示名(テキスト)
	//    p.textId = "stage4_start";
	//    p.textIndex = 0;//(stage1_start_000.png)

	//    p.focus = FocusSide::Right;

	//    // ★Page0必須：左右の初期表情
	//    p.leftFaceId = "normal";//蜘蛛初期表情
	//    p.rightFaceId = "normal";//巫女初期表情
	//    p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
	//    //今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
	//    //表情を変更しないときは何も書かないように

	//    // このページのボイス
	//    //p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_000;

	//    m_Pages.push_back(p);
	//    //やっぱり……何回来ても良いなぁ……
	//}
	//// Page1
	//{
	//    MessagePage p;

	//    p.nameId = "kumo";
	//    p.textId = "stage4_start";
	//    p.textIndex = 1;

	//    p.focus = FocusSide::Left;
	//    p.speakerFaceId = "";

	//    //p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_START_001;

	//    m_Pages.push_back(p);
	//    //…くだらない、さっさと要件を済ませろ
	//}
}

void Stage10::BuildEndPages()
{
	//m_Pages.clear();

	//// Page0
	//// 右=巫女,左=女郎蜘蛛
	//{
	//    MessagePage p;

	//    // ★必須：このページの表示（名前＋本文）
	//    p.nameId = "miko";        // name_miko.png
	//    p.textId = "stage4_end";  // text_stage1_end_***
	//    p.textIndex = 0;          // 000

	//    p.focus = FocusSide::Right;

	//    // Page0必須：左右の初期表情
	//    p.leftFaceId = "normal";
	//    p.rightFaceId = "normal";
	//    p.speakerFaceId = "";

	//    // このページのボイス
	//    p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_END_000;

	//    m_Pages.push_back(p);
	//    //そんなにお腹が空いてるなら、お団子でも五平餅でも食べればいいじゃないですか
	//}
	//// Page1
	//{
	//    MessagePage p;

	//    p.nameId = "kumo";
	//    p.textId = "stage4_end";
	//    p.textIndex = 1;

	//    p.focus = FocusSide::Left;
	//    p.speakerFaceId = "";

	//    p.voiceLabel = SOUND_LABEL_VOICE_STAGE4_END_001;

	//    m_Pages.push_back(p);
	//    //…理性を保つのに、生の血肉が必要でな
	//}
}

void Stage10::UpdateEnemySpawn()
{
	static int lastSpawnSecond = -1;
	if (elapsedSeconds != 0 && elapsedSeconds % 8 == 0 && elapsedSeconds != lastSpawnSecond)   //0秒以降かつ8秒ごとにスポーン
	{
		EnemySpawnFlag = true;
		lastSpawnSecond = elapsedSeconds;
	}

	if (EnemySpawnFlag == true)
	{
		if (elapsedSeconds <= 24)   //ステージ開始から24秒以内はNORMALのみ
		{
			int i = get_rand_range(1, 3);
			i += EnemySpawnplus; //24秒ごとに出現する敵の数を増やす
			for (i; i > 0; i--) //敵を1~3体まで出現
			{
				//ランダムな位置にEnemyをスポーンさせる
				float x = static_cast<float>(get_rand_range(-300, 300));    //+-300以内にしないとステージ外にスポーンする可能性あり
				float y = static_cast<float>(get_rand_range(-300, 300));
				Vector3 spawnPos = Vector3(x, y, 0.0f);
				EnemySpawn(NORMAL, spawnPos);
			}
			EnemySpawnFlag = false; //スポーンフラグをリセット
		}
		else //24秒以降
		{
			int i = get_rand_range(1, 3);
			i += EnemySpawnplus; //24秒ごとに出現する敵の数を増やす
			for (i; i > 0; i--)
			{
				//ランダムな位置にEnemyをスポーンさせる
				float x = static_cast<float>(get_rand_range(-300, 300));    //+-300以内にしないとステージ外にスポーンする可能性あり
				float y = static_cast<float>(get_rand_range(-300, 300));


				Vector3 spawnPos = Vector3(x, y, 0.0f);

				//ランダムで敵の種類を決定
				int r = get_rand_range(0, 9);

				//24秒ごとに敵の種類の出現確率を変化させる
				switch (elapsedSeconds / 24)
				{
				case 1:
					//最初はNORMALの確率高め
					if (r <= 6) EnemySpawn(NORMAL, spawnPos);
					else if (r == 7) EnemySpawn(CUTTER, spawnPos);
					else if (r == 8) EnemySpawn(MAYU, spawnPos);
					else if (r == 9) EnemySpawn(TACKLE, spawnPos);
					break;

				case 2:
					//最初はNORMALの確率高め
					if (r <= 6) EnemySpawn(NORMAL, spawnPos);
					else if (r == 7) EnemySpawn(CUTTER, spawnPos);
					else if (r == 8) EnemySpawn(MAYU, spawnPos);
					else if (r == 9) EnemySpawn(TACKLE, spawnPos);
					break;

				case 3:
					//少しずつNORMAL以外の確率を上げる
					if (r <= 5) EnemySpawn(NORMAL, spawnPos);
					else if (r == 6) EnemySpawn(CUTTER, spawnPos);
					else if (r == 7) EnemySpawn(MAYU, spawnPos);
					else if (r == 8 || r == 9) EnemySpawn(TACKLE, spawnPos);
					break;

				case 4:
					//少しずつNORMAL以外の確率を上げる
					if (r <= 4) EnemySpawn(NORMAL, spawnPos);
					else if (r == 5) EnemySpawn(CUTTER, spawnPos);
					else if (r == 6 || r == 7) EnemySpawn(MAYU, spawnPos);
					else if (r == 8 || r == 9) EnemySpawn(TACKLE, spawnPos);
					break;

				default:
					//確率上昇ストップ、以降は同じ確率で出現
					if (r <= 3) EnemySpawn(NORMAL, spawnPos);
					else if (r == 4 || r == 5) EnemySpawn(CUTTER, spawnPos);
					else if (r == 6 || r == 7) EnemySpawn(MAYU, spawnPos);
					else if (r == 8 || r == 9) EnemySpawn(TACKLE, spawnPos);
					break;
				}
			}
			EnemySpawnFlag = false; //スポーンフラグをリセット
		}
	}
}

void Stage10::StageClearCheck()
{
	//敵を全て倒したかどうか
	//エンドレスなのでなし	
}

void Stage10::StageFailedCheck()
{
	//ステージ失敗かどうか
	if (m_Miko->GetDYINGTimer() <= 0)
	{
		m_GameOverFlg = true;
		survivalTime = elapsedSeconds;
	}
}

void Stage10::IssueUpdate()
{
	if (m_GameOverFlg == true)
	{
		m_GameUpdateBlock = true;
		Game::GetInstance()->SetWorldStopped(true);
		// 初回だけ画像生成
		if (m_GameOverImageFlg == false)
		{
			ScoreCheck();
			//スコア
			ScoreImage[0] = Game::GetInstance()->AddObject<Texture2D>();
			ScoreImage[0]->SetTexture("assets/texture/ui/score.png");
			ScoreImage[0]->SetPosition(-300.0f, 250.0f, 0.0f);
			ScoreImage[0]->SetScale(426.0f, 144.0f, 0.0f);
			ScoreImage[0]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(ScoreImage[0]);

			ScoreImage[1] = Game::GetInstance()->AddObject<Texture2D>();
			switch (Score_10000)
			{
				case 0:
					ScoreImage[1]->SetTexture("assets/texture/ui/time0.png");				
					break;
				case 1:
					ScoreImage[1]->SetTexture("assets/texture/ui/time1.png");
					break;
				case 2:
					ScoreImage[1]->SetTexture("assets/texture/ui/time2.png");
					break;
				case 3:
					ScoreImage[1]->SetTexture("assets/texture/ui/time3.png");
					break;
				case 4:
					ScoreImage[1]->SetTexture("assets/texture/ui/time4.png");
					break;
				case 5:
					ScoreImage[1]->SetTexture("assets/texture/ui/time5.png");
					break;
				case 6:
					ScoreImage[1]->SetTexture("assets/texture/ui/time6.png");
					break;
				case 7:
					ScoreImage[1]->SetTexture("assets/texture/ui/time7.png");
					break;
				case 8:
					ScoreImage[1]->SetTexture("assets/texture/ui/time8.png");
					break;
				case 9:
					ScoreImage[1]->SetTexture("assets/texture/ui/time9.png");
					break;
			}
			ScoreImage[1]->SetPosition(0.0f, 250.0f, 0.0f);
			ScoreImage[1]->SetScale(144.0f, 144.0f, 0.0f);
			ScoreImage[1]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(ScoreImage[1]);

			ScoreImage[2] = Game::GetInstance()->AddObject<Texture2D>();
			switch (Score_1000)
			{
			case 0:
				ScoreImage[2]->SetTexture("assets/texture/ui/time0.png");
				break;
			case 1:
				ScoreImage[2]->SetTexture("assets/texture/ui/time1.png");
				break;
			case 2:
				ScoreImage[2]->SetTexture("assets/texture/ui/time2.png");
				break;
			case 3:
				ScoreImage[2]->SetTexture("assets/texture/ui/time3.png");
				break;
			case 4:
				ScoreImage[2]->SetTexture("assets/texture/ui/time4.png");
				break;
			case 5:
				ScoreImage[2]->SetTexture("assets/texture/ui/time5.png");
				break;
			case 6:
				ScoreImage[2]->SetTexture("assets/texture/ui/time6.png");
				break;
			case 7:
				ScoreImage[2]->SetTexture("assets/texture/ui/time7.png");
				break;
			case 8:
				ScoreImage[2]->SetTexture("assets/texture/ui/time8.png");
				break;
			case 9:
				ScoreImage[2]->SetTexture("assets/texture/ui/time9.png");
				break;
			}
			ScoreImage[2]->SetPosition(100.0f, 250.0f, 0.0f);
			ScoreImage[2]->SetScale(144.0f, 144.0f, 0.0f);
			ScoreImage[2]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(ScoreImage[2]);

			ScoreImage[3] = Game::GetInstance()->AddObject<Texture2D>();
			switch (Score_100)
			{
			case 0:
				ScoreImage[3]->SetTexture("assets/texture/ui/time0.png");
				break;
			case 1:
				ScoreImage[3]->SetTexture("assets/texture/ui/time1.png");
				break;
			case 2:
				ScoreImage[3]->SetTexture("assets/texture/ui/time2.png");
				break;
			case 3:
				ScoreImage[3]->SetTexture("assets/texture/ui/time3.png");
				break;
			case 4:
				ScoreImage[3]->SetTexture("assets/texture/ui/time4.png");
				break;
			case 5:
				ScoreImage[3]->SetTexture("assets/texture/ui/time5.png");
				break;
			case 6:
				ScoreImage[3]->SetTexture("assets/texture/ui/time6.png");
				break;
			case 7:
				ScoreImage[3]->SetTexture("assets/texture/ui/time7.png");
				break;
			case 8:
				ScoreImage[3]->SetTexture("assets/texture/ui/time8.png");
				break;
			case 9:
				ScoreImage[3]->SetTexture("assets/texture/ui/time9.png");
				break;
			}
			ScoreImage[3]->SetPosition(200.0f, 250.0f, 0.0f);
			ScoreImage[3]->SetScale(144.0f, 144.0f, 0.0f);
			ScoreImage[3]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(ScoreImage[3]);

			ScoreImage[4] = Game::GetInstance()->AddObject<Texture2D>();
			switch (Score_10)
			{
			case 0:
				ScoreImage[4]->SetTexture("assets/texture/ui/time0.png");
				break;
			case 1:
				ScoreImage[4]->SetTexture("assets/texture/ui/time1.png");
				break;
			case 2:
				ScoreImage[4]->SetTexture("assets/texture/ui/time2.png");
				break;
			case 3:
				ScoreImage[4]->SetTexture("assets/texture/ui/time3.png");
				break;
			case 4:
				ScoreImage[4]->SetTexture("assets/texture/ui/time4.png");
				break;
			case 5:
				ScoreImage[4]->SetTexture("assets/texture/ui/time5.png");
				break;
			case 6:
				ScoreImage[4]->SetTexture("assets/texture/ui/time6.png");
				break;
			case 7:
				ScoreImage[4]->SetTexture("assets/texture/ui/time7.png");
				break;
			case 8:
				ScoreImage[4]->SetTexture("assets/texture/ui/time8.png");
				break;
			case 9:
				ScoreImage[4]->SetTexture("assets/texture/ui/time9.png");
				break;
			}
			ScoreImage[4]->SetPosition(300.0f, 250.0f, 0.0f);
			ScoreImage[4]->SetScale(144.0f, 144.0f, 0.0f);
			ScoreImage[4]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(ScoreImage[4]);

			ScoreImage[5] = Game::GetInstance()->AddObject<Texture2D>();
			switch (Score_1)
			{
			case 0:
				ScoreImage[5]->SetTexture("assets/texture/ui/time0.png");
				break;
			case 1:
				ScoreImage[5]->SetTexture("assets/texture/ui/time1.png");
				break;
			case 2:
				ScoreImage[5]->SetTexture("assets/texture/ui/time2.png");
				break;
			case 3:
				ScoreImage[5]->SetTexture("assets/texture/ui/time3.png");
				break;
			case 4:
				ScoreImage[5]->SetTexture("assets/texture/ui/time4.png");
				break;
			case 5:
				ScoreImage[5]->SetTexture("assets/texture/ui/time5.png");
				break;
			case 6:
				ScoreImage[5]->SetTexture("assets/texture/ui/time6.png");
				break;
			case 7:
				ScoreImage[5]->SetTexture("assets/texture/ui/time7.png");
				break;
			case 8:
				ScoreImage[5]->SetTexture("assets/texture/ui/time8.png");
				break;
			case 9:
				ScoreImage[5]->SetTexture("assets/texture/ui/time9.png");
				break;
			}
			ScoreImage[5]->SetPosition(400.0f, 250.0f, 0.0f);
			ScoreImage[5]->SetScale(144.0f, 144.0f, 0.0f);
			ScoreImage[5]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(ScoreImage[5]);

			//生存時間
			SurvivalTimeImage[0] = Game::GetInstance()->AddObject<Texture2D>();
			SurvivalTimeImage[0]->SetTexture("assets/texture/ui/timetext.png");
			SurvivalTimeImage[0]->SetPosition(-300.0f, 50.0f, 0.0f);
			SurvivalTimeImage[0]->SetScale(426.0f, 144.0f, 0.0f);
			SurvivalTimeImage[0]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(SurvivalTimeImage[0]);

			SurvivalTimeImage[1] = Game::GetInstance()->AddObject<Texture2D>();
			switch (survivalTime_100)
			{
				case 0:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time0.png");
					break;
				case 1:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time1.png");
					break;
				case 2:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time2.png");
					break;
				case 3:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time3.png");
					break;
				case 4:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time4.png");
					break;
				case 5:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time5.png");
					break;
				case 6:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time6.png");
					break;
				case 7:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time7.png");
					break;
				case 8:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time8.png");
					break;
				case 9:
					SurvivalTimeImage[1]->SetTexture("assets/texture/ui/time9.png");
					break;
			}
			SurvivalTimeImage[1]->SetPosition(0.0f, 50.0f, 0.0f);
			SurvivalTimeImage[1]->SetScale(144.0f, 144.0f, 0.0f);
			SurvivalTimeImage[1]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(SurvivalTimeImage[1]);

			SurvivalTimeImage[2] = Game::GetInstance()->AddObject<Texture2D>();
			switch (survivalTime_10)
			{
			case 0:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time0.png");
				break;
			case 1:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time1.png");
				break;
			case 2:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time2.png");
				break;
			case 3:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time3.png");
				break;
			case 4:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time4.png");
				break;
			case 5:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time5.png");
				break;
			case 6:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time6.png");
				break;
			case 7:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time7.png");
				break;
			case 8:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time8.png");
				break;
			case 9:
				SurvivalTimeImage[2]->SetTexture("assets/texture/ui/time9.png");
				break;
			}
			SurvivalTimeImage[2]->SetPosition(100.0f, 50.0f, 0.0f);
			SurvivalTimeImage[2]->SetScale(144.0f, 144.0f, 0.0f);
			SurvivalTimeImage[2]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(SurvivalTimeImage[2]);

			SurvivalTimeImage[3] = Game::GetInstance()->AddObject<Texture2D>();
			switch (survivalTime_1)
			{
			case 0:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time0.png");
				break;
			case 1:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time1.png");
				break;
			case 2:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time2.png");
				break;
			case 3:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time3.png");
				break;
			case 4:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time4.png");
				break;
			case 5:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time5.png");
				break;
			case 6:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time6.png");
				break;
			case 7:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time7.png");
				break;
			case 8:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time8.png");
				break;
			case 9:
				SurvivalTimeImage[3]->SetTexture("assets/texture/ui/time9.png");
				break;
			}
			SurvivalTimeImage[3]->SetPosition(200.0f, 50.0f, 0.0f);
			SurvivalTimeImage[3]->SetScale(144.0f, 144.0f, 0.0f);
			SurvivalTimeImage[3]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(SurvivalTimeImage[3]);

			SurvivalTimeImage[4] = Game::GetInstance()->AddObject<Texture2D>();
			SurvivalTimeImage[4]->SetTexture("assets/texture/ui/timetext2.png");
			SurvivalTimeImage[4]->SetPosition(320.0f, 50.0f, 0.0f);
			SurvivalTimeImage[4]->SetScale(144.0f, 144.0f, 0.0f);
			SurvivalTimeImage[4]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(SurvivalTimeImage[4]);

			//もう一度遊ぶ
			GameOverImage[1] = Game::GetInstance()->AddObject<Texture2D>();
			GameOverImage[1]->SetTexture("assets/texture/lose001.png");
			GameOverImage[1]->SetPosition(0.0f, 0.0f, 0.0f);
			GameOverImage[1]->SetScale(2048.0f, 1152.0f, 0.0f);
			GameOverImage[1]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(GameOverImage[1]);

			//ステージセレクトへ
			GameOverImage[2] = Game::GetInstance()->AddObject<Texture2D>();
			GameOverImage[2]->SetTexture("assets/texture/lose002.png");
			GameOverImage[2]->SetPosition(0.0f, 0.0f, 0.0f);
			GameOverImage[2]->SetScale(1280.0f, 720.0f, 0.0f);
			GameOverImage[2]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(GameOverImage[2]);

			m_SelectIndex = 0;
			m_GameOverImageFlg = true;
		}

		// スティックで選択
		{
			DirectX::XMFLOAT2 stick = Input::GetLeftAnalogStick();
			static bool stickFree_GameOver = true;

			bool leftKey = Input::GetKeyTrigger(VK_A);
			bool rightKey = Input::GetKeyTrigger(VK_D);

			if (stickFree_GameOver)
			{
				if (leftKey || rightKey)
				{
					m_SelectIndex = (m_SelectIndex + 1) % 2; // 0 <-> 1
					stickFree_GameOver = false;
				}
				if (stick.x < -0.5f)
				{
					m_SelectIndex = (m_SelectIndex + 1) % 2; // 0<->1
					stickFree_GameOver = false;
				}
				else if (stick.x > 0.5f)
				{
					m_SelectIndex = (m_SelectIndex + 1) % 2; // 0<->1
					stickFree_GameOver = false;
				}
			}
			if (fabsf(stick.x) < 0.2f) stickFree_GameOver = true;
		}

		// 見た目反映
		//選択してる方を大きく、選択していないほうを小さく
		if (m_SelectIndex == 0)
		{
			GameOverImage[1]->SetScale(2048.0f, 1152.0f, 0.0f);
			GameOverImage[2]->SetScale(1280.0f, 720.0f, 0.0f);
		}
		else
		{
			GameOverImage[1]->SetScale(1280.0f, 720.0f, 0.0f);
			GameOverImage[2]->SetScale(2048.0f, 1152.0f, 0.0f);
		}

		// 決定
		if (Input::GetKeyTrigger(VK_SPACE) || Input::GetButtonTrigger(XINPUT_A))
		{
			//もう一度プレイ
			if (m_SelectIndex == 0)
			{
				RePlay();
				return;
			}
			else
			{
				//ステージセレクトへ
				Game::GetInstance()->ChangeScene(SceneName::STAGE_SELECT);
				return;
			}
		}
		return;
	}	
}

void Stage10::RePlay()
{
	// 画面消す
	ScoreImage[0]->SetScale(0.0f, 0.0f, 0.0f);
	ScoreImage[1]->SetScale(0.0f, 0.0f, 0.0f);
	ScoreImage[2]->SetScale(0.0f, 0.0f, 0.0f);
	ScoreImage[3]->SetScale(0.0f, 0.0f, 0.0f);
	ScoreImage[4]->SetScale(0.0f, 0.0f, 0.0f);
	ScoreImage[5]->SetScale(0.0f, 0.0f, 0.0f);
	SurvivalTimeImage[0]->SetScale(0.0f, 0.0f, 0.0f);
	SurvivalTimeImage[1]->SetScale(0.0f, 0.0f, 0.0f);
	SurvivalTimeImage[2]->SetScale(0.0f, 0.0f, 0.0f);
	SurvivalTimeImage[3]->SetScale(0.0f, 0.0f, 0.0f);
	SurvivalTimeImage[4]->SetScale(0.0f, 0.0f, 0.0f);
	GameOverImage[1]->SetScale(0.0f, 0.0f, 0.0f);
	GameOverImage[2]->SetScale(0.0f, 0.0f, 0.0f);

	// 次回のStageはGameplay開始にする
	Game::GetInstance()->SetNextStageStartMode(10, StageStartMode::Gameplay);

	// ステージを作り直す（安全）
	Game::GetInstance()->ChangeScene(SceneName::STAGE10);
	return;
}

void Stage10::ScoreCheck()
{
	//スコアと生存時間を桁ごとに分ける
	survivalTime_100 = survivalTime / 100;
	survivalTime_10 = survivalTime / 10;
	survivalTime_1 = survivalTime % 10;

	Score_10000 = Score / 10000;
	Score_1000 = (Score / 1000) % 10;
	Score_100 = (Score / 100) % 10;
	Score_10 = (Score / 10) % 10;
	Score_1 = Score % 10;
}