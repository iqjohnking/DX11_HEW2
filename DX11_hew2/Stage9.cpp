#include "Stage9.h"
#include "Game.h"

void Stage9::Init()
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
}

void Stage9::Uninit()
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

void Stage9::Update()
{
	MessageUpdate();
	GameUpdate();
	// 終了会話が終わったらリザルトへ
	if (m_Flow == Flow::EndTalk)
	{
		if (!m_Message->IsPlaying())
		{
			Game::GetInstance()->ChangeScene(RESULT);
		}
	}
}

void Stage9::MessageUpdate()
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

void Stage9::GameUpdate()
{
	if (m_Flow != Flow::Gameplay) return;

	//60フレーム経過するごとに1秒プラス
	elapsedFrames++;
	elapsedSeconds = elapsedFrames / 60;
}

void Stage9::BuildStartPages()
{
	m_Pages.clear();

	// Page0:
	// 右=巫女,左=女郎蜘蛛
	{
		MessagePage p;

		// ★必須：このページの表示（名前＋本文）
		p.nameId = "kumo";//表示名(テキスト)
		p.textId = "stage9_start";
		p.textIndex = 0;//(stage1_start_000.png)

		p.focus = FocusSide::Left;

		// ★Page0必須：左右の初期表情
		p.leftFaceId = "normal";//蜘蛛初期表情
		p.rightFaceId = "normal";//巫女初期表情
		p.speakerFaceId = "";//フォーカスしている話者のみ表情を変更
		//今ここに前と同じ表情を入れると立ち絵が表示されなくなるバグがあります
		//表情を変更しないときは何も書かないように

		// このページのボイス
		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_START_000;

		m_Pages.push_back(p);
		//…まだなのか？早くしろ…！
	}
	// Page1
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_start";
		p.textIndex = 1;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_START_001;

		m_Pages.push_back(p);
		//囮になりながらだから準備に時間がかかるんですよ！もう少し待ってください！
	}
	// Page2
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_start";
		p.textIndex = 2;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//………
	}
	// Page3
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage9_start";
		p.textIndex = 3;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//敵出現
	}
	// Page4
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_start";
		p.textIndex = 4;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_START_004;

		m_Pages.push_back(p);
		//…邪魔はさせぬぞ
	}
	// Page5
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage9_start";
		p.textIndex = 5;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（叫ぶ妖怪達）
	}
	// Page6
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_start";
		p.textIndex = 6;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_START_006;

		m_Pages.push_back(p);
		//……ほざけ…！！最初に裏切ったのは貴様らだろうが…！！
	}
}

void Stage9::BuildEndPages()
{
	m_Pages.clear();

	// Page0
	// 右=巫女,左=女郎蜘蛛
	{
		MessagePage p;

		// ★必須：このページの表示（名前＋本文）
		p.nameId = "miko";        // name_miko.png
		p.textId = "stage9_end";  // text_stage1_end_***
		p.textIndex = 0;          // 000

		p.focus = FocusSide::Right;

		// Page0必須：左右の初期表情
		p.leftFaceId = "normal";
		p.rightFaceId = "normal";
		p.speakerFaceId = "";

		// このページのボイス
		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_000;

		m_Pages.push_back(p);
		//………！来た！
	}
	// Page1
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 1;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_001;

		m_Pages.push_back(p);
		//…！（…なんだ！？小娘の雰囲気が違う…！）
	}
	// Page2
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 2;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_002;

		m_Pages.push_back(p);
		//……古より伝わる星の神よ…
	}
	// Page3
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 3;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_003;

		m_Pages.push_back(p);
		//放たれし邪悪な妖を消し去るため
	}
	// Page4
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 4;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_004;

		m_Pages.push_back(p);
		//この書に記された制約に従い、私に力を貸せ…！
	}
	// Page5
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage9_end";
		p.textIndex = 5;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（森の中にある神社が青白く光り、亜空間への入り口が開く）
	}
	// Page6
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage9_end";
		p.textIndex = 6;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（女郎蜘蛛を除き、周りにいた妖怪たちが渦を巻いて吸い込まれていく）
	}
	// Page7
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 7;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_007;

		m_Pages.push_back(p);
		//終わったか……
	}
	// Page8
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 8;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_008;

		m_Pages.push_back(p);
		//はい！終わりましたよ！
	}
	// Page9
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 9;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_009;

		m_Pages.push_back(p);
		//…ん？何故私は残っている？
	}
	// Page10
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 10;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_010;

		m_Pages.push_back(p);
		//…少しだけおしゃべりがしたくって。エネルギーは残してるので、いつでも送れますよ。
	}
	// Page11
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 11;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_011;

		m_Pages.push_back(p);
		//…渡しておきますね
	}
	// Page12
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 12;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_012;

		m_Pages.push_back(p);
		//…随分と器用なことをするものだな
	}
	// Page13
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 13;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_013;

		m_Pages.push_back(p);
		//…本当にありがとうございました
	}
	// Page14
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 14;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_014;

		m_Pages.push_back(p);
		//それと…ごめんなさい、私の尻拭いに付き合わせてしまって
	}
	// Page15
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 15;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_015;

		m_Pages.push_back(p);
		//…まったくだ
	}
	// Page16
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 16;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//…
	}
	// Page17
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 17;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_017;

		m_Pages.push_back(p);
		//…私、1000年前に生まれたかったです
	}
	// Page18
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 18;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_018;

		m_Pages.push_back(p);
		//…やはりお前といるのも少しだけ不愉快だ
	}
	// Page19
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 19;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_019;

		m_Pages.push_back(p);
		//…あの！
	}
	// Page20
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 20;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//………
	}
	// Page21
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage9_end";
		p.textIndex = 21;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//別れ(光る)
	}
	// Page22
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 22;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_022;

		m_Pages.push_back(p);
		//……やり直せると思いますよ！私は！
	}
	// Page23
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 23;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_023;

		m_Pages.push_back(p);
		//…お前はつくづく、嫌なところを突いてくるな
	}
	// Page24
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage9_end";
		p.textIndex = 24;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//暗転(目を閉じる)
	}
	// Page25
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage9_end";
		p.textIndex = 25;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（再び目覚める）
	}
	// Page26
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 26;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_026;

		m_Pages.push_back(p);
		//誰か…誰か助けて…！
	}
	// Page27
	{
		MessagePage p;

		p.nameId = "miko";
		p.textId = "stage9_end";
		p.textIndex = 27;

		p.focus = FocusSide::Right;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_027;

		m_Pages.push_back(p);
		//うわっ…！！
	}
	// Page28
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage9_end";
		p.textIndex = 28;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（襲われる直前、妖怪たちを蜘蛛の糸がぐるぐると絡めとる）
	}
	// Page29
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 29;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_029;

		m_Pages.push_back(p);
		//………随分早い再開だな？小娘…
	}
	// Page30
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 30;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_030;

		m_Pages.push_back(p);
		//お前…ん？
	}
	// Page31
	{
		MessagePage p;

		p.nameId = "none";
		p.textId = "stage9_end";
		p.textIndex = 31;

		p.focus = FocusSide::None;
		p.speakerFaceId = "";

		//ボイスなし

		m_Pages.push_back(p);
		//（派手に壊れている亜空間のゲートと結界陣を見つける）
	}
	// Page32
	{
		MessagePage p;

		p.nameId = "kumo";
		p.textId = "stage9_end";
		p.textIndex = 32;

		p.focus = FocusSide::Left;
		p.speakerFaceId = "";

		p.voiceLabel = SOUND_LABEL_VOICE_STAGE9_END_032;

		m_Pages.push_back(p);
		//………お前一体何をした…？
	}
}

void Stage9::UpdateEnemySpawn()
{

}

void Stage9::StageClearCheck()
{

}

void Stage9::StageFailedCheck()
{

}