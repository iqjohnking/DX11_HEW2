#include "Stage10.h"
#include "Game.h"
#include<iostream>

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

	//経過したフレーム数と秒数を0にリセット
	elapsedFrames = 0;
	elapsedSeconds = 0;

	// UI用の赤い糸表示
	m_UI_redSilk = Game::GetInstance()->AddObject<UI_redSilk>();
	m_MySceneObjects.emplace_back(m_UI_redSilk);
	m_UI_redSilk->SetHands(m_HandL, m_HandR);

	// UI用の赤い糸表示
	m_UI_mikoHp = Game::GetInstance()->AddObject<UI_mikoHp>();
	m_MySceneObjects.emplace_back(m_UI_mikoHp);
	m_UI_mikoHp->SetMiko(m_Miko);
	m_SilkCount = 0;

	/*
	//敵が出現するフェーズのフラグをリセット
	phase1Flag = false;
	phase2Flag = false;
	phase3Flag = false;
	phase4Flag = false;
	phase5Flag = false;
	phase6Flag = false;
	*/

	EnemySpawnFlag = false;

	StagekillCount = 0;     //倒した敵の数をリセット
	//StageEnemyCount = 11;   //ステージの敵の総数を設定（エンドレスなので不要）

	//BGM開始
	Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_000);
}

void Stage10::Uninit()
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

void Stage10::Update()
{
	MessageUpdate();
	GameUpdate();
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

void Stage10::MessageUpdate()
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
					std::cout << Score << std::endl;
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
	//StageClearCheck();
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
	/*
	//敵を全て倒したかどうか
	if (StagekillCount >= StageEnemyCount)
	{
		m_Flow = Flow::EndTalk;
	}
	*/
}

void Stage10::StageFailedCheck()
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