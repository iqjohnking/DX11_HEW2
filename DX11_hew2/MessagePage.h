#pragma once
#include <string>
#include "sound.h"


// どちらを話者として強調するか
enum class FocusSide
{
	None,
	Left,
	Right
};

//台本
struct MessagePage
{
    std::string nameId;//(miko, kumo)

    std::string textId;
    int textIndex = -1; // -1なら変更しない運用にできる（必須にするなら>=0前提）

    // どちらを話者として強調するか
    FocusSide focus = FocusSide::None; //(Left / Right / None)

    // このページで「話者側」をこの表情にしたい（空なら変更しない）
    std::string speakerFaceId;

    // Page0用（左右の初期表情）
    std::string leftFaceId;
    std::string rightFaceId;

    // このページで鳴らすボイス(無効値は SOUND_LABEL_MAX)
    SOUND_LABEL voiceLabel = SOUND_LABEL_MAX;
};
