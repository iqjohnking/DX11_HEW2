#pragma once
#include "StageBase.h"

class Stage0 : public StageBase
{
private:
	//敵の出現を管理するためのフラグ
	bool phase1Flag = false;
	bool phase2Flag = false;
	bool phase3Flag = false;

	//SoundFlg
	bool m_Conversation_BGM_flg_1 = false;
	bool m_Conversation_BGM_flg_2 = false;

public:
	Stage0() {};
	~Stage0() {};

	void Init() override; // 初期化
	void Uninit() override; // 終了処理
	void Update() override; // 更新

	void MessageUpdate() override;	//会話パート更新
	void GameUpdate() override;		//ゲームパート更新
	void SoundUpdate() override;	//sound更新

	//会話パート
	void BuildStartPages() override;
	void BuildEndPages() override;

	void UpdateEnemySpawn() override;
	void StageClearCheck() override;
	void StageFailedCheck() override;
};

