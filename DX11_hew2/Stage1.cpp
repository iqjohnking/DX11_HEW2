#include "Stage1.h"

#include "Game.h"
#include "Input.h"

void Stage1::Init()
{
	const int kStageNo = 1;
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
		Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_000);
	}
	else // Gameplay開始
	{
		m_Flow = Flow::Gameplay;
	}

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

	StagekillCount = 0;     //倒した敵の数をリセット
	StageEnemyCount = 11;   //ステージの敵の総数を設定

	m_Conversation_BGM_flg_1 = false;
	m_Conversation_BGM_flg_2 = false;
	m_Conversation_BGM_flg_3 = false;
	m_Conversation_BGM_flg_4 = false;

	m_ClearFlg = false;
	m_ClearImageFlg = false;
	m_ClearChangeImageFlg = false;
	m_GameOverFlg = false;
	m_GameOverImageFlg = false;
	m_ChangeClearCount = 60;

	m_SelectIndex = 0;

	m_GameUpdateBlock = false;
}

void Stage1::Uninit()
{
	if (m_Message)
	{
		m_Message->Stop();
	}

	m_Pages.clear();

	//BGMを停止
	if (m_Flow == Flow::Gameplay)
	{
		Game::GetSound()->Stop(SOUND_LABEL_BGM_STAGE_000);
	}
	Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_000);

	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}

void Stage1::Update()
{
	if(m_GameUpdateBlock != true)
	{
		GameUpdate();
		UpdateEnemySpawn();
		MessageUpdate();
	}
	
	SoundUpdate();
	IssueUpdate();
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
		break;

	case Flow::EndTalk:
		break;
	}
}

//ゲームパートのUpdate
void Stage1::GameUpdate()
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

//soundのUpdate
void Stage1::SoundUpdate()
{
	if (m_Message->GetIndex() == 16 && m_Conversation_BGM_flg_1 == false)
	{
		m_Conversation_BGM_flg_1 = true;
		//BGM停止
		Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_000);
	}

	if (m_Message->GetIndex() == 19 && m_Conversation_BGM_flg_2 == false)
	{
		m_Conversation_BGM_flg_2 = true;
		//BGM開始
		Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_001);
	}

	if (m_Flow == Flow::Gameplay && m_Conversation_BGM_flg_1 == false)
	{
		m_Conversation_BGM_flg_1 = true;
		Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_000);
	}

	if (m_Flow == Flow::Gameplay && m_Conversation_BGM_flg_3 == false)
	{
		m_Conversation_BGM_flg_3 = true;
		Game::GetSound()->Stop(SOUND_LABEL_BGM_CONVERSATION_001);
		Game::GetSound()->Play(SOUND_LABEL_BGM_STAGE_000);
	}

	if (m_Flow == Flow::EndTalk && m_Conversation_BGM_flg_4 == false)
	{
		m_Conversation_BGM_flg_4 = true;
		Game::GetSound()->Stop(SOUND_LABEL_BGM_STAGE_000);
		Game::GetSound()->Play(SOUND_LABEL_BGM_CONVERSATION_000);
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

		p.focus = FocusSide::Right;

		// ★Page0必須：左右の初期表情
		p.leftFaceId = "normal";//蜘蛛初期表情
		p.rightFaceId = "normal";//巫女初期表情
		p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
		//今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
		//表情を変更しないときは何も書かないように

		// このページのボイス
		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_000;

		m_Pages.push_back(p);
		//…助けて貰って、ありがとうございます
	}

	// Page1
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_start";
		p.textIndex = 1;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_001;

		m_Pages.push_back(p);
		//……あなたは…何なんですか…？
	}

	// Page2
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 2;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_002;

		m_Pages.push_back(p);
		//お前たちのいう女郎蜘蛛…とでも言えば分かるか？
	}
	// Page3
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_start";
		p.textIndex = 3;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "surprised";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_003;

		m_Pages.push_back(p);
		//…！
	}

	// Page4
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_start";
		p.textIndex = 4;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_004;

		m_Pages.push_back(p);
		//…なんで…私が呼び出したのは神様のはずじゃ…！
	}
	// Page5
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage1_start";
		p.textIndex = 5;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（結界陣を見る女郎蜘蛛）
	}
	// Page6
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 6;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_006;

		m_Pages.push_back(p);
		//(推し量るに…私が呼び出されたのは助けを呼ぼうと召喚術を行って邪魔されてこいつらの邪気が混じった結果か)
	}
	// Page7
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 7;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "glare";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_007;

		m_Pages.push_back(p);
		//……私を利用しようと召喚した代償は払ってもらうぞ？小娘…
	}
	// Page8
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_start";
		p.textIndex = 8;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "smile";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_008;

		m_Pages.push_back(p);
		//……へへ
	}
	// Page9
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 9;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_009;

		m_Pages.push_back(p);
		//舐めてるのか？
	}
	// Page10
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage1_start";
		p.textIndex = 10;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（遠くを見渡す女郎蜘蛛）
	}
	// Page11
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 11;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "normal";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_011;

		m_Pages.push_back(p);
		//(…結界から逃げたのがちらほらといるようだな…)
	}
	// Page12
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 12;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_012;

		m_Pages.push_back(p);
		//…そうだ…
	}
	// Page13
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_start";
		p.textIndex = 13;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "normal";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_013;

		m_Pages.push_back(p);
		//な…何ですか…
	}
	// Page14
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 14;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_014;

		m_Pages.push_back(p);
		//お前を死なせさえしなければいいのだったか？
	}
	// Page15
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_start";
		p.textIndex = 15;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "surprised";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_015;

		m_Pages.push_back(p);
		//えっ？そ、そうですけど…
	}
	// Page16
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 16;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_016;

		m_Pages.push_back(p);
		//囮になれ
	}
	// Page17
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_start";
		p.textIndex = 17;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "normal";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_017;

		m_Pages.push_back(p);
		//で、でも私…
	}
	// Page18
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage1_start";
		p.textIndex = 18;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（巫女の首に軽く糸を巻き付けて圧をかける）
	}
	// Page19
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 19;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "smile";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_019;

		m_Pages.push_back(p);
		//…お前を「死なせさえしなければ」いいのだったか？
	}
	// Page20
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_start";
		p.textIndex = 20;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "sad";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_020;

		m_Pages.push_back(p);
		//うっ…や…やります…！
	}
	// Page21
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 21;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "normal";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_021;

		m_Pages.push_back(p);
		//さて…復讐の第一歩だ…
	}
	// Page22
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_start";
		p.textIndex = 22;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "glare";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_START_022;

		m_Pages.push_back(p);
		//全員滅ぼしてやる…！
	}
}

void Stage1::BuildEndPages()
{
	m_Pages.clear();

	// Page0
	// 右=巫女,左=女郎蜘蛛
	{
		MessagePage p;

		// ★必須：このページの表示（名前＋本文）
		p.nameId = "kumo";        // name_miko.png
		p.textId = "stage1_end";  // text_stage1_end_***
		p.textIndex = 0;          // 000

		p.focus = FocusSide::Left;

		// Page0必須：左右の初期表情
		p.leftFaceId = "normal";
		p.rightFaceId = "normal";
		p.speakerFaceId = "";

		// このページのボイス
		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_END_000;

		m_Pages.push_back(p);
		//感覚が戻ってきた……悪くない
	}
	// Page1
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_end";
		p.textIndex = 1;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_END_001;

		m_Pages.push_back(p);
		//言い忘れてましたけど、貴方は私から20メートル以上離れられないので
	}
	// Page2
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_end";
		p.textIndex = 2;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_END_002;

		m_Pages.push_back(p);
		//…随分面倒な制約の多い召喚術だな？近代の術師は相当弱いらしい…
	}
	// Page3
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_end";
		p.textIndex = 3;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_END_003;

		m_Pages.push_back(p);
		//…それくらい色々しないと、人間に神様を呼び出すなんてことできませんから
	}
	// Page4
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage1_end";
		p.textIndex = 4;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_END_004;

		m_Pages.push_back(p);
		//それで呼び出されたのが私とは…貴様も不運なものだな？
	}
	// Page5
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage1_end";
		p.textIndex = 5;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE1_END_005;

		m_Pages.push_back(p);
		//…私一人の命で千年前からの脅威を消し去れるなら安いものですよ…！
	}
}

//実行すると敵がスポーン
void Stage1::UpdateEnemySpawn()
{
	if (elapsedSeconds == 5 && phase1Flag == false)	    //5秒経過かつフェーズ1が未実行なら
	{
		EnemySpawn(NORMAL, Vector3(0.0f, 300.0f, 0.0f));
		phase1Flag = true;
	}

	if (elapsedSeconds == 12 && phase2Flag == false)	//12秒経過かつフェーズ2が未実行なら
	{
		EnemySpawn(NORMAL, Vector3(300.0f, 50.0f, 0.0f));
		phase2Flag = true;
	}

	if (elapsedSeconds == 18 && phase3Flag == false)	//18秒経過かつフェーズ3が未実行なら
	{
		EnemySpawn(NORMAL, Vector3(250.0f, 200.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(150.0f, 300.0f, 0.0f));
		phase3Flag = true;
	}

	if (elapsedSeconds == 25 && phase4Flag == false)	//25秒経過かつフェーズ4が未実行なら
	{
		EnemySpawn(NORMAL, Vector3(50.0f, -350.f, 0.0f));
		EnemySpawn(NORMAL, Vector3(-50.0f, -350.f, 0.0f));
		phase4Flag = true;
	}

	if (elapsedSeconds == 32 && phase5Flag == false)	//32秒経過かつフェーズ5が未実行なら
	{
		EnemySpawn(NORMAL, Vector3(400.0f, -150.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(300.0f, -300.0f, 0.0f));
		phase5Flag = true;
	}

	if (elapsedSeconds == 39 && phase6Flag == false)	//39秒経過かつフェーズ6が未実行なら
	{
		EnemySpawn(NORMAL, Vector3(400.0f, -50.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(300.0f, -300.0f, 0.0f));
		EnemySpawn(NORMAL, Vector3(50.0f, -400.0f, 0.0f));
		phase6Flag = true;
	}
}

void Stage1::StageClearCheck()
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

	Game::GetInstance()->SetMaxClearedStage(1);
}

void Stage1::StageFailedCheck()
{
	if (m_Miko->GetDYINGTimer() <= 0)
	{
		m_GameOverFlg = true;
	}
}

void Stage1::IssueUpdate()
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
					Game::GetInstance()->ChangeScene(STAGE2);
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

void Stage1::RePlay()
{
	// 画面消す
	GameOverImage[0]->SetScale(0.0f, 0.0f, 0.0f);
	GameOverImage[1]->SetScale(0.0f, 0.0f, 0.0f);
	GameOverImage[2]->SetScale(0.0f, 0.0f, 0.0f);

	// 次回のStage1はGameplay開始にする
	Game::GetInstance()->SetNextStageStartMode(1, StageStartMode::Gameplay);

	// ステージを作り直す（安全）
	Game::GetInstance()->ChangeScene(STAGE1);
	return;
}