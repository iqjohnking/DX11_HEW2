#pragma once
#include "Scene.h"
#include "Object.h"
#include <vector>

class Texture2D;

class StageSelectScene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // オブジェクト管理用

	// --- 表示オブジェクト ---
	Texture2D* m_stagesentakugamen = nullptr;
	Texture2D* m_stagesentakugamen_waku = nullptr;
	Texture2D* m_daiissyou= nullptr;
	Texture2D* m_daiissyou_waku = nullptr;
	Texture2D* m_migiyazirusi = nullptr;
	Texture2D* m_storysentaku = nullptr;
	
	//ステージ選択(仮)
	Texture2D* m_mode_in_L = nullptr;
	Texture2D* m_mode_in_M = nullptr;
	Texture2D* m_mode_in_R = nullptr;

	Texture2D* m_FadePanel = nullptr;

	int m_NextSceneID = 0; // 1~9:各ステージ 10:ModeSelect
	float m_fadeAlpha = 1.0f;
	bool m_isStarting = false;

	float m_curScaleL = 400.0f;
	float m_curScaleM = 400.0f;
	float m_curScaleR = 400.0f;

	bool IsMouseOver(Texture2D* obj);

	int m_SelectIndex = 0; // 0?2	ボタンの選択
	int m_Chapter = 1;     // 1:一章, 2:二章 3:三章

public:
	StageSelectScene();
	~StageSelectScene();

	void Init() override;
	void Uninit() override;
	void Update() override;
};