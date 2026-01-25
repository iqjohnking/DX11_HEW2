#pragma once
#include "Scene.h"
#include "Object.h"
#include <vector>

class Texture2D;

class ModeSelectScene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // オブジェクト管理用

	// --- 状態管理 ---
	bool m_IsWaitingInput = true;   // 最初は Press Enter 待ち
	int  m_SelectedIndex = 0;       // 0:ストーリー, 1:エンドレス

	// --- 表示オブジェクト ---
	Texture2D* m_mode_in_L = nullptr;
	Texture2D* m_mode_in_R = nullptr;
	Texture2D* m_modesentaku = nullptr;
	Texture2D* m_modesentakugamen = nullptr;
	Texture2D* m_modesentakugamen_waku = nullptr;
	Texture2D* m_StoryBtn = nullptr;
	Texture2D* m_EndlessBtn = nullptr;

	bool IsMouseOver(Texture2D* obj);
public:
	ModeSelectScene();
	~ModeSelectScene();

	void Init() override;
	void Uninit() override;
	void Update() override;
};