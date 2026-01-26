#pragma once
#include "StageBase.h"
#include<iostream>

class Stage2 : public StageBase
{
private:
	//敵の出現を管理するためのフラグ
	bool phase1Flag = false;
	bool phase2Flag = false;
	bool phase3Flag = false;
	bool phase4Flag = false;
	bool phase5Flag = false;
	bool phase6Flag = false;
	bool phase7Flag = false;
	bool phase8Flag = false;
	bool phase9Flag = false;

	//SoundFlg
	bool Conversation_BGM_flg_1 = false;
	bool Conversation_BGM_flg_2 = false;

public:
	Stage2() {};
	~Stage2() {};

	void Init() override; // 初期化
	void Uninit() override; // 終了処理
	void Update() override; // 更新

	void MessageUpdate() override;	//会話パート更新
	void GameUpdate() override;		//ゲームパート更新
	void SoundUpdate()override;//sound更新

	//会話パート
	void BuildStartPages() override;
	void BuildEndPages() override;

	void UpdateEnemySpawn() override;		//引数でスポーンする敵の種類と座標を指定できるようにしたい
	void StageClearCheck() override;
	void StageFailedCheck() override;
};

