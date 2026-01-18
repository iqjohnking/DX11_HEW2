#pragma once
#include <string>


// どちらを話者として強調するか
enum class FocusSide
{
	None,
	Left,
	Right
};

struct MessagePage
{
	// 話者の論理ID（"miko", "kumo"）
	std::string speakerCharId;

	// 表示用の名前(巫女、女郎蜘蛛)
	std::string speakerName;

	// 本文
	std::string text;

	// ボイスID（空なら再生しない）
	std::string voiceId;

	// フォーカス（話してる側を明るくするなどに使う）
	FocusSide focus = FocusSide::None;
};

