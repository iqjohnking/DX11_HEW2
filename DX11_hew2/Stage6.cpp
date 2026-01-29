#include "Stage6.h"
#include "Game.h"

void Stage6::Init()
{
	const int kStageNo = 6;
	// 次回開始モードを取得
	StageStartMode startMode = Game::GetInstance()->ConsumeNextStageStartMode(kStageNo);

	// 司令塔をObjectとして追加
	m_Message = Game::GetInstance()->AddObject<MessageManager>();
	m_MySceneObjects.emplace_back(m_Message);

	// 会話参加者の指定（キャラID）
	m_Message->SetParticipants("kumo", "miko");

	// 会話素材の指定
	m_Message->SetFramePath("assets/texture/Message/UI/MessageUI.png");

	m_Message->SetBackgroundPath("assets/texture/Message/bg/bg_stage1.jpg");

	if (startMode == StageStartMode::StartTalk)
	{
		BuildStartPages();
		m_Message->SetPages(m_Pages);
		m_Message->Play();

		m_Flow = Flow::StartTalk;

		// 会話BGM開始
		Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_003);
	}
	else // Gameplay開始
	{
		m_Flow = Flow::Gameplay;
	}

	//SoundFlg
	m_Conversation_BGM_flg_1 = false;
	m_Conversation_BGM_flg_2 = false;
	m_Conversation_BGM_flg_3 = false;

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

	StagekillCount = 0;     //倒した敵の数をリセット
	StageEnemyCount = 39;   //ステージの敵の総数を設定

	ClearImage[0, 1, 2] = { nullptr };
	GameOverImage[0, 1, 2] = { nullptr };

	m_ClearFlg = false;
	m_ClearImageFlg = false;
	m_ClearChangeImageFlg = false;
	m_GameOverFlg = false;
	m_GameOverImageFlg = false;
	m_ChangeClearCount = 60;

	m_SelectIndex = 0;

	m_GameUpdateBlock = false;
}

void Stage6::Uninit()
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

void Stage6::Update()
{
	if (m_GameUpdateBlock != true)
	{
		GameUpdate();
		UpdateEnemySpawn();
		MessageUpdate();
	}

	SoundUpdate();
	IssueUpdate();
}

void Stage6::MessageUpdate()
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

void Stage6::GameUpdate()
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

	//ステージクリアと失敗のチェック
	StageClearCheck();
	StageFailedCheck();
}

void Stage6::SoundUpdate()
{
	if (m_Flow == Flow::Gameplay && m_Conversation_BGM_flg_1 == false)
	{
		m_Conversation_BGM_flg_1 = true;
		Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_003);
		Game::GetSound()->Play(SOUND_LABEL_BGM_STAGE_001);
	}

	if (m_Flow == Flow::EndTalk && m_Conversation_BGM_flg_2 == false)
	{
		m_Conversation_BGM_flg_2 = true;
		Game::GetSound()->Stop(SOUND_LABEL_BGM_STAGE_001);
		Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_003);
	}
}

void Stage6::BuildStartPages()
{
	m_Pages.clear();

	// Page0:
	// 右=巫女,左=女郎蜘蛛
	{
		MessagePage p;

		// ★必須：このページの表示（名前＋本文）
		p.nameId = "Bookstore";//表示名(テキスト)
		p.textId = "stage6_start";
		p.textIndex = 0;//(stage1_start_000.png)

		p.focus = FocusSide::None;

		// ★Page0必須：左右の初期表情
		p.leftFaceId = "normal";//蜘蛛初期表情
		p.rightFaceId = "normal";//巫女初期表情
		p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
		//今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
		//表情を変更しないときは何も書かないように

		// このページのボイス
		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_000;

		m_Pages.push_back(p);
		//や、やめろ！！来るな！
	}
	// Page1
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage6_start";
		p.textIndex = 1;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（古書館長が襲い掛かられる直前に間一髪助け出す巫女）
	}
	// Page2
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage6_start";
		p.textIndex = 2;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "surprised";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_002;

		m_Pages.push_back(p);
		//大丈夫ですか！？
	}
	// Page3
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage6_start";
		p.textIndex = 3;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_003;

		m_Pages.push_back(p);
		//…っ！？
	}
	// Page4
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage6_start";
		p.textIndex = 4;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（助け出した隙を突かれて妖怪の軍勢に襲い掛かられそうな巫女、それを一網打尽にする女郎蜘蛛）
	}
	// Page5
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage6_start";
		p.textIndex = 5;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "smile";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_005;

		m_Pages.push_back(p);
		//…ありがーー
	}
	// Page6
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage6_start";
		p.textIndex = 6;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_START_006;

		m_Pages.push_back(p);
		//黙れ。私と貴様は運命共同体だろうが、さっさと神託書とやらを探してこい
	}
}

void Stage6::BuildEndPages()
{
	m_Pages.clear();

	// Page0
	// 右=巫女,左=女郎蜘蛛
	{
		MessagePage p;

		// ★必須：このページの表示（名前＋本文）
		p.nameId = "miko";        // name_miko.png
		p.textId = "stage6_end";  // text_stage1_end_***
		p.textIndex = 0;          // 000

		p.focus = FocusSide::Right;

		// Page0必須：左右の初期表情
		p.leftFaceId = "normal";
		p.rightFaceId = "normal";
		p.speakerFaceId = "surprised";

		// このページのボイス
		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_000;

		m_Pages.push_back(p);
		//…あった……！
	}
	// Page1
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage6_end";
		p.textIndex = 1;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_001;

		m_Pages.push_back(p);
		//のろのろと…もっと早く見つけられんものか？
	}
	// Page2
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage6_end";
		p.textIndex = 2;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "normal";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_002;

		m_Pages.push_back(p);
		//…いちいち嫌味言わないでくださいよ…
	}
	// Page3
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage6_end";
		p.textIndex = 3;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_003;

		m_Pages.push_back(p);
		//…この「大封印の書」…これさえあれば…
	}
	// Page4
	{
		MessagePage p;

		p.nameId = "villager";
		p.textId = "stage6_end";
		p.textIndex = 4;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_004;

		m_Pages.push_back(p);
		//ありがとうございます！なんとお礼を言ったらよいか…！
	}
	// Page5
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage6_end";
		p.textIndex = 5;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_005;

		m_Pages.push_back(p);
		//妖怪たちはあの人が…
	}
	// Page6
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage6_end";
		p.textIndex = 6;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//…
	}
	// Page7
	{
		MessagePage p;

		p.nameId = "villager";
		p.textId = "stage6_end";
		p.textIndex = 7;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_007;

		m_Pages.push_back(p);
		//…あの見た目、妖怪…まさか…
	}
	// Page8
	{
		MessagePage p;

		p.nameId = "villager";
		p.textId = "stage6_end";
		p.textIndex = 8;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_008;

		m_Pages.push_back(p);
		//本で見た事あるぞ…女郎蜘蛛じゃ…
	}
	// Page9
	{
		MessagePage p;

		p.nameId = "villager";
		p.textId = "stage6_end";
		p.textIndex = 9;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_009;

		m_Pages.push_back(p);
		//女郎蜘蛛…？私たちを助けたって…そんな訳…
	}
	// Page10
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage6_end";
		p.textIndex = 10;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "surprised";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_010;

		m_Pages.push_back(p);
		//あ…あの！本当なーー
	}
	// Page11
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage6_end";
		p.textIndex = 11;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "glare";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_011;

		m_Pages.push_back(p);
		//…消えろ。
	}
	// Page12
	{
		MessagePage p;

		p.nameId = "villager";
		p.textId = "stage6_end";
		p.textIndex = 12;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_012;

		m_Pages.push_back(p);
		//え？
	}
	// Page13
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage6_end";
		p.textIndex = 13;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "angry";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_013;

		m_Pages.push_back(p);
		//死にたくなければ全員消えろと言ってるんだ…！
	}
	// Page14
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage6_end";
		p.textIndex = 14;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（慌てて逃げ出す一同）
	}
	// Page15
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage6_end";
		p.textIndex = 15;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "sad";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_015;

		m_Pages.push_back(p);
		//「ちょ…ちょっと…」
	}
	// Page16
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage6_end";
		p.textIndex = 16;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "normal";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_016;

		m_Pages.push_back(p);
		//…村の者に手は出さん、さっさと壊した祠の場所へ向かえ…
	}
	// Page17
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage6_end";
		p.textIndex = 17;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE6_END_017;

		m_Pages.push_back(p);
		//ここにいるだけでも心底不愉快だ
	}
}

void Stage6::UpdateEnemySpawn()
{
	if (elapsedSeconds == 5 && phase1Flag == false)
	{
		EnemySpawn(NORMAL, Vector3(350.0f, 150.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(150.0f, 350.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-150.0f, 350.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-350.0f, 150.0f, 0.0f));
		phase1Flag = true;
	}

	if (elapsedSeconds == 11 && phase2Flag == false)
	{
		EnemySpawn(NORMAL, Vector3(-150.0f, 350.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-200.0f, 300.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-275.0f, 150.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-350.0f, 50.0f, 0.0f));
		phase2Flag = true;
	}

	if (elapsedSeconds == 18 && phase3Flag == false)
	{
		EnemySpawn(NORMAL, Vector3(350.0f, 100.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(300.0f, 200.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(2500.0f, 300.0f, 0.0f));
		phase3Flag = true;
	}

	if (elapsedSeconds == 25 && phase4Flag == false)
	{
		EnemySpawn(NORMAL, Vector3(-200.0f, 350.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-200.0f, -350.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-250.0f, 250.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-250.0f, -250.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-300.0f, 150.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-300.0f, -150.0f, 0.0f));
		phase4Flag = true;
	}

	if (elapsedSeconds == 33 && phase5Flag == false)
	{
		EnemySpawn(NORMAL, Vector3(75.0f, 375.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(0.0f, 400.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-75.0f, 375.0f, 0.0f));
		phase5Flag = true;
	}

	if (elapsedSeconds == 39 && phase6Flag == false)
	{
		EnemySpawn(NORMAL, Vector3(400.0f, 0.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(350.0f, 100.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(350.0f, -100.0f, 0.0f));
		phase6Flag = true;
	}

	if (elapsedSeconds == 45 && phase7Flag == false)
	{
		EnemySpawn(NORMAL, Vector3(75.0f, -375.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(0.0f, -400.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-75.0f, -375.0f, 0.0f));
		phase7Flag = true;
	}

	if (elapsedSeconds == 51 && phase8Flag == false)
	{
		EnemySpawn(NORMAL, Vector3(-375.0f, 100.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-375.0f, -100.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-400.0f, 0.0f, 0.0f));
		phase8Flag = true;
	}

	if (elapsedSeconds == 57 && phase9Flag == false)
	{
		EnemySpawn(NORMAL, Vector3(100.0f, 375.0f, 0.0f));
		EnemySpawn(MAYU, Vector3(0.0f, 400.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-100.0f, 375.0f, 0.0f));
		phase9Flag = true;
	}

	if (elapsedSeconds == 64 && phase10Flag == false)
	{
		EnemySpawn(MAYU, Vector3(400.0f, 0.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(250.0f, 320.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(150.0f, 360.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(0.0f, 400.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-150.0f, 460.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-250.0f, 320.0f, 0.0f));
		EnemySpawn(MAYU, Vector3(-400.0f, 0.0f, 0.0f));
		phase10Flag = true;
	}
}

void Stage6::StageClearCheck()
{
	//敵を全て倒したかどうか
	if (StagekillCount >= StageEnemyCount)
	{
		m_ChangeClearCount--;
	}
	if (m_ChangeClearCount <= 0 && m_ClearFlg == false)
	{
		m_ClearFlg = true;
	}

	Game::GetInstance()->SetMaxClearedStage(6);

}

void Stage6::StageFailedCheck()
{
	if (m_Miko->GetDYINGTimer() <= 0)
	{
		m_GameOverFlg = true;
	}
}

void Stage6::IssueUpdate()
{
	if (m_GameOverFlg == true)
	{
		m_GameUpdateBlock = true;
		Game::GetInstance()->SetWorldStopped(true);
		// 初回だけ画像生成
		if (m_GameOverImageFlg == false)
		{
			//敗北
			GameOverImage[0] = Game::GetInstance()->AddObject<Texture2D>();
			GameOverImage[0]->SetTexture("assets/texture/lose000.png");
			GameOverImage[0]->SetPosition(0.0f, 0.0f, 0.0f);
			GameOverImage[0]->SetScale(1920.0f, 1080.0f, 0.0f);
			GameOverImage[0]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(GameOverImage[0]);

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

			if (stickFree_GameOver)
			{
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
				Game::GetInstance()->ChangeScene(STAGE_SELECT);
				return;
			}
		}
		return;
	}

	if (m_ClearFlg == true)
	{
		m_GameUpdateBlock = true;
		Game::GetInstance()->SetWorldStopped(true);
		if (m_ClearImageFlg == false)
		{
			//勝利
			ClearImage[0] = Game::GetInstance()->AddObject<Texture2D>();
			ClearImage[0]->SetTexture("assets/texture/win.png");
			ClearImage[0]->SetPosition(0.0f, 0.0f, 0.0f);
			ClearImage[0]->SetScale(1920.0f, 1080.0f, 0.0f);
			ClearImage[0]->SetDrawOrder(10000);
			m_MySceneObjects.emplace_back(ClearImage[0]);

			m_ClearImageFlg = true;
		}

		if (Input::GetKeyTrigger(VK_SPACE) || Input::GetButtonTrigger(XINPUT_A))
		{
			ClearImage[0]->SetScale(0.0f, 0.0f, 0.0f);

			//終了会話スタート
			BuildEndPages();
			m_Message->SetPages(m_Pages);
			m_Message->Play();
			m_Flow = Flow::EndTalk;

			m_ClearFlg = false; // クリア演出終了
		}
		return;
	}
	if (m_Flow == Flow::EndTalk)
	{
		if (!m_Message->IsPlaying())
		{
			// 初回だけ画像生成
			if (m_ClearChangeImageFlg == false)
			{
				m_ClearChangeImageFlg = true;

				//次のステージへ
				ClearImage[1] = Game::GetInstance()->AddObject<Texture2D>();
				ClearImage[1]->SetTexture("assets/texture/win002.png");
				ClearImage[1]->SetPosition(0.0f, 0.0f, 0.0f);
				ClearImage[1]->SetScale(2048.0f, 1152.0f, 0.0f);
				ClearImage[1]->SetDrawOrder(10000);
				m_MySceneObjects.emplace_back(ClearImage[1]);

				//セレクトへ
				ClearImage[2] = Game::GetInstance()->AddObject<Texture2D>();
				ClearImage[2]->SetTexture("assets/texture/lose002.png");
				ClearImage[2]->SetPosition(0.0f, 0.0f, 0.0f);
				ClearImage[2]->SetScale(1280.0f, 720.0f, 0.0f);
				ClearImage[2]->SetDrawOrder(10000);
				m_MySceneObjects.emplace_back(ClearImage[2]);

				m_SelectIndex = 0;
			}

			// スティックで選択
			{
				DirectX::XMFLOAT2 stick = Input::GetLeftAnalogStick();
				static bool stickFree_ClearResult = true;

				if (stickFree_ClearResult)
				{
					if (stick.x < -0.5f)
					{
						m_SelectIndex = (m_SelectIndex + 1) % 2; // 0<->1
						stickFree_ClearResult = false;
					}
					else if (stick.x > 0.5f)
					{
						m_SelectIndex = (m_SelectIndex + 1) % 2; // 0<->1
						stickFree_ClearResult = false;
					}
				}
				if (fabsf(stick.x) < 0.2f) stickFree_ClearResult = true;
			}

			// 見た目反映
			if (m_SelectIndex == 0)
			{
				//次のステージへ
				ClearImage[1]->SetScale(2048.0f, 1152.0f, 0.0f);
				ClearImage[2]->SetScale(1280.0f, 720.0f, 0.0f);
			}
			else
			{
				//セレクトへ
				ClearImage[1]->SetScale(1280.0f, 720.0f, 0.0f);
				ClearImage[2]->SetScale(2048.0f, 1152.0f, 0.0f);
			}

			// 決定
			if (Input::GetKeyTrigger(VK_SPACE) || Input::GetButtonTrigger(XINPUT_A))
			{
				if (m_SelectIndex == 0)
				{
					//次のステージへ
					Game::GetInstance()->ChangeScene(STAGE7);
					return;
				}
				else
				{
					//セレクトへ
					Game::GetInstance()->ChangeScene(STAGE_SELECT);
					return;
				}
			}
		}
	}
}

void Stage6::RePlay()
{
	// 画面消す
	GameOverImage[0]->SetScale(0.0f, 0.0f, 0.0f);
	GameOverImage[1]->SetScale(0.0f, 0.0f, 0.0f);
	GameOverImage[2]->SetScale(0.0f, 0.0f, 0.0f);

	Game::GetInstance()->SetNextStageStartMode(6, StageStartMode::Gameplay);

	Game::GetInstance()->ChangeScene(STAGE6);
	return;
}