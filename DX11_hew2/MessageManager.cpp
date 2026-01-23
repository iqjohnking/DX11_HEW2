#include "MessageManager.h"
#include "input.h"
#include "Game.h"

void MessageManager::Init()
{
	m_Playing = false;
	m_Index = 0;

	CreatePartsIfNeeded();
}

void MessageManager::Uninit()
{
	StopCurrentVoice();
	m_Playing = false;
	m_Index = 0;

	m_AButtonHoldFrames = 0;
	m_AButtonWasDown = false;
	m_AButtonPendingAdvance = false;
	m_HoldSkipFired = false;

	CleanupParts();

	// Deleteはしない
	// ただしポインタは将来の事故防止で切っておく
	m_Background = nullptr;
	m_UI = nullptr;
	m_Text = nullptr;
	m_LeftChar = nullptr;
	m_RightChar = nullptr;
}

void MessageManager::Draw(Camera* cam)
{
	//各部品が勝手に描画するので何もなし
}

void MessageManager::Update()
{
	if (!m_Playing) return;

	// 次へ進む
	if (Input::GetKeyTrigger(VK_SPACE) || Input::GetButtonTrigger(XINPUT_A) || Input::GetKeyTrigger(VK_E))
	{
		Advance();
	}

	// 押してる間 true
	const bool spaceDown = Input::GetKeyPress(VK_SPACE);
	const bool AButtonDown = Input::GetButtonPress(XINPUT_A);

	// 60fps前提で2秒
	const int kHoldFramesToSkipAll = 2 * 60;

	if (spaceDown || AButtonDown)
	{
		// 押し始めた瞬間
		if (!m_AButtonWasDown)
		{
			m_AButtonHoldFrames = 0;
			m_AButtonPendingAdvance = true; // 短押しなら離した瞬間にAdvanceする
			m_HoldSkipFired = false;
		}

		m_AButtonHoldFrames++;

		// 3秒到達で会話を全部スキップ
		if (!m_HoldSkipFired && m_AButtonHoldFrames >= kHoldFramesToSkipAll)
		{
			m_HoldSkipFired = true;
			m_AButtonPendingAdvance = false; // Stopしたので短押しAdvanceはキャンセル
			Stop();
			// Stopでm_Playing=falseになるので、このフレームで終わり
			m_AButtonWasDown = true;
			return;
		}
	}
	else
	{
		// 離した瞬間、2秒未満なら1回だけ次へ
		if (m_AButtonWasDown)
		{
			if (!m_HoldSkipFired && m_AButtonPendingAdvance)
			{
				Advance();
			}
		}

		// リセット
		m_AButtonHoldFrames = 0;
		m_AButtonPendingAdvance = false;
		m_HoldSkipFired = false;
	}

	m_AButtonWasDown = spaceDown;
}

void MessageManager::SetFramePath(const std::string& path)
{
	m_FramePath = path;

	if (m_UI)
	{
		m_UI->SetFrame(m_FramePath);
	}
}

void MessageManager::SetBackgroundPath(const std::string& path)
{
	m_BackgroundPath = path;

	// 既に背景部品があるなら即反映
	if (m_Background && !m_BackgroundPath.empty())
	{
		m_Background->SetFrame(m_BackgroundPath);
	}
}

void MessageManager::Play()
{
	// 部品が無いなら作る
	CreatePartsIfNeeded();
	if (m_Pages.empty()) return;
	const MessagePage& p0 = m_Pages[0];
	if (p0.leftFaceId.empty() || p0.rightFaceId.empty())
	{
		m_Playing = false;
		m_Index = 0;
		CleanupParts();
		return;
	}
	if (!m_UI || !m_Text || !m_LeftChar || !m_RightChar || !m_Background) return;

	if (m_LeftCharId.empty() || m_RightCharId.empty()) return;

	// 台本チェック
	if (!ValidatePagesForPlay()) return;

	// 念押しで参加者反映
	m_LeftChar->SetCharacter(m_LeftCharId);
	m_RightChar->SetCharacter(m_RightCharId);

	m_Playing = true;
	m_Index = 0;

	m_Background->Show(true);
	m_UI->Show(true);
	m_Text->Show(true);
	m_LeftChar->Show(true);
	m_RightChar->Show(true);

	BeginPage(m_Index);
}

void MessageManager::Stop()
{
	if (!m_Playing)
	{
		CleanupParts();
		return;
	}

	StopCurrentVoice();

	m_Playing = false;
	m_Index = 0;

	CleanupParts();
}

void MessageManager::Advance()
{
	if (!m_Playing) return;

	StopCurrentVoice();

	m_Index++;
	if (m_Index >= (int)m_Pages.size())
	{
		Stop();
		return;
	}

	BeginPage(m_Index);
}

void MessageManager::BeginPage(int index)
{
	// 万一欠けてたら作る
	CreatePartsIfNeeded();

	if (!m_UI || !m_Text || !m_LeftChar || !m_RightChar)
	{
		m_Playing = false;
		return;
	}

	// 台本が無い / 範囲外 なら安全に停止
	if (m_Pages.empty() || index < 0 || index >= (int)m_Pages.size())
	{
		Stop();
		return;
	}

	const MessagePage& p = m_Pages[index];

	// 名前画像（空なら変更しない運用にするなら if を付ける）
	{
		const std::string namePath = BuildNameImagePath(p.nameId);
		if (!namePath.empty())
			m_Text->SetNameImage(namePath);
	}

	// 本文画像
	{
		const std::string textPath = BuildTextImagePath(p.textId, p.textIndex);
		if (!textPath.empty())
			m_Text->SetTextImage(textPath);
	}

	// Page0で左右の初期表情を決める(空なら何もしない＝TalkCharacter側の現状維持)
	if (index == 0)
	{
		m_LeftChar->SetFace(p.leftFaceId);
		m_RightChar->SetFace(p.rightFaceId);
	}

	// 話者側だけ表情を更新(空なら変更しない)
	if (!p.speakerFaceId.empty())
	{
		if (p.focus == FocusSide::Left)
		{
			m_LeftChar->SetFace(p.speakerFaceId);
		}
		else if (p.focus == FocusSide::Right)
		{
			m_RightChar->SetFace(p.speakerFaceId);
		}
		// None は誰も変えない（前の表情を維持）
	}

	// フォーカス
	if (p.focus == FocusSide::Left)
	{
		m_LeftChar->SetFocus(true);
		m_RightChar->SetFocus(false);
	}
	else if (p.focus == FocusSide::Right)
	{
		m_LeftChar->SetFocus(false);
		m_RightChar->SetFocus(true);
	}
	else
	{
		// Noneの場合：両方暗く
		//要調整
		m_LeftChar->SetFocus(false);
		m_RightChar->SetFocus(false);
	}

	// ボイス再生
	// このページがボイス無しなら今鳴ってるものを止めて終了
	if (p.voiceLabel == SOUND_LABEL_MAX)
	{
		StopCurrentVoice();
		return;
	}

	// 同じボイスなら再生し直さない
	if (m_CurrentVoice == p.voiceLabel)
		return;

	// 別ボイスなら前を止めてから鳴らす
	StopCurrentVoice();
	Game::GetSound()->Play(p.voiceLabel);
	m_CurrentVoice = p.voiceLabel;
}

void MessageManager::StopCurrentVoice()
{
	if (m_CurrentVoice == SOUND_LABEL_MAX)
		return;

	// 再生中のボイスを停止
	Game::GetSound()->Stop(m_CurrentVoice);

	m_CurrentVoice = SOUND_LABEL_MAX;
}

void MessageManager::CreatePartsIfNeeded()
{
	// すでに揃っているなら何もしない
	if (m_Background && m_UI && m_Text && m_LeftChar && m_RightChar)
		return;

	Game* g = Game::GetInstance();
	if (!g) return;

	bool created = false;

	if (!m_Background)
	{
		m_Background = g->AddObject<MessageUI>();
		created = true;
	}
	if (!m_UI)
	{	
		m_UI = g->AddObject<MessageUI>();
		created = true;
	}
	if (!m_Text)
	{
		m_Text = g->AddObject<MessageText>();
		created = true;
	}
	if (!m_LeftChar)
	{
		m_LeftChar = g->AddObject<TalkCharacter>();
		created = true;
	}
	if (!m_RightChar)
	{
		m_RightChar = g->AddObject<TalkCharacter>();
		created = true;
	}

	if (created)
	{
		SetupParts();
	}
}

void MessageManager::SetupParts()
{
	// 描画順(必要なら調整)
	m_Background->SetDrawOrder(800);
	m_LeftChar->SetDrawOrder(900);
	m_RightChar->SetDrawOrder(900);
	m_UI->SetDrawOrder(1000);
	m_Text->SetDrawOrder(1100);

	// 左右設定
	m_LeftChar->SetSide(TalkSide::Left);
	m_RightChar->SetSide(TalkSide::Right);

	// いったん非表示（Playで表示）
	m_Background->Show(false);
	m_UI->Show(false);
	m_Text->Show(false);
	m_LeftChar->Show(false);
	m_RightChar->Show(false);

	// 会話枠パスが設定済みなら反映
	if (!m_FramePath.empty())
	{
		m_UI->SetFrame(m_FramePath);
	}

	if (!m_LeftCharId.empty())
	{
		m_LeftChar->SetCharacter(m_LeftCharId);
	}
	if (!m_RightCharId.empty())
	{
		m_RightChar->SetCharacter(m_RightCharId);
	}
}

void MessageManager::SetParticipants(const std::string& leftCharId, const std::string& rightCharId)
{
	m_LeftCharId = leftCharId;
	m_RightCharId = rightCharId;

	// 既に部品があれば即反映してOK（無ければPlay時に反映）
	if (m_LeftChar && !m_LeftCharId.empty())   m_LeftChar->SetCharacter(m_LeftCharId);
	if (m_RightChar && !m_RightCharId.empty()) m_RightChar->SetCharacter(m_RightCharId);
}

void MessageManager::CleanupParts()
{
	if(m_Background) m_Background->Show(false);
	if (m_UI) m_UI->Show(false);
	if (m_Text) m_Text->Show(false);
	if (m_LeftChar) m_LeftChar->Show(false);
	if (m_RightChar) m_RightChar->Show(false);
}

std::string MessageManager::BuildNameImagePath(const std::string& nameId) const
{
	if (nameId.empty()) return std::string();
	return "assets/texture/Message/name/name_" + nameId + ".png";
	//例)assets/texture/Message/name/name_miko.png
}

// 本文画像：assets/texture/Message/text/text_{id}_{000}.png
std::string MessageManager::BuildTextImagePath(const std::string& textId, int index) const
{
	if (textId.empty()) return std::string();
	if (index < 0) return std::string();

	std::ostringstream oss;
	oss << "assets/texture/Message/text/text_" << textId << "_"
		<< std::setw(3) << std::setfill('0') << index
		<< ".png";
	return oss.str();
	//例)assets/texture/Message/text/text_stage1_start_000.png
}

bool MessageManager::ValidatePagesForPlay() const
{
	if (m_Pages.empty())
	{
		//デバッグ用
		std::cout << "[Message] pages empty\n";
		return false;
	}

	const MessagePage& p0 = m_Pages[0];

	// Page0は初期表示を確定させたいので必須
	if (p0.nameId.empty())
	{
		//デバッグ用
		std::cout << "[Message] Page0 nameId is empty\n";
		return false;
	}

	if (p0.textId.empty())
	{
		//デバッグ用
		std::cout << "[Message] Page0 textId is empty\n";
		return false;
	}

	if (p0.textIndex < 0)
	{
		//デバッグ用
		std::cout << "[Message] Page0 textIndex < 0\n";
		return false;
	}

	return true;
}