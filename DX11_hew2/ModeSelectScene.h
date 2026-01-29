#pragma once
#include "Scene.h"
#include "Object.h"
#include <vector>

class Texture2D;

class ModeSelectScene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // オブジェクト管理用

	int m_SelectIndex = 0; // 0:ストーリー, 1:エンドレス

	// --- 表示オブジェクト ---
	Texture2D* m_mode_in_L = nullptr;
	Texture2D* m_mode_in_R = nullptr;
	Texture2D* m_mode_out_L = nullptr;
	Texture2D* m_mode_out_R = nullptr;
	Texture2D* m_modesentaku = nullptr;
	Texture2D* m_modesentakugamen = nullptr;
	Texture2D* m_modesentakugamen_waku = nullptr;
	Texture2D* m_StoryBtn = nullptr;
	Texture2D* m_EndlessBtn = nullptr;
	Texture2D* m_FadePanel = nullptr; // フェード用
	
	float m_fadeAlpha = 1.0f;       
	bool m_isStarting = false;		// 決定ボタンを押したか

	float m_curStoryScale = 400.0f;
	float m_curEndlessScale = 400.0f;

	int m_NextSceneID = 0; //1 : START, 2 : STAGE_SELECT, 3 : STAGE10）

	bool IsMouseOver(Texture2D* obj);
public:
	ModeSelectScene();
	~ModeSelectScene();

	void Init() override;
	void Uninit() override;
	void Update() override;
};