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
    // 表示用の名前(巫女、女郎蜘蛛)//デバック用
    //std::string speakerName;

    // 名前欄画像（例：assets/texture/Message/name/name_miko.png）
    //std::string nameImagePath;

    // 本文//デバック用
    //std::string text;

    // 本文画像（例：assets/texture/Message/text/text_stage1_start_000.png）
    //std::string textImagePath;

    // 表示画像の指定は「ID」でやる
    // name_{nameId}.png 例: name_miko.png / name_kumo.png
    std::string nameId;

    // text_{textId}_{index:3桁}.png 例: text_stage1_start_000.png
    std::string textId;
    int textIndex = -1; // -1なら変更しない運用にできる（必須にするなら>=0前提）

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
