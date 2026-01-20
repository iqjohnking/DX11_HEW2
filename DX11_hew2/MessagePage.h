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
    // 表示用の名前(巫女、女郎蜘蛛)
    std::string speakerName;

    // 本文
    std::string text;

    // ボイスID（空なら再生しない）
    std::string voiceId;

    // どちらを話者として強調するか
    FocusSide focus = FocusSide::None;

    // このページで「話者側」をこの表情にしたい（空なら変更しない）
    std::string speakerFaceId;

    // Page0用（左右の初期表情）。空なら変更しない
    std::string leftFaceId;
    std::string rightFaceId;
};
