#pragma once
#include "StageBase.h"

class Stage10 : public StageBase
{
private:
	/*
	//敵の出現を管理するためのフラグ
	bool phase1Flag = false;
	bool phase2Flag = false;
	bool phase3Flag = false;
	bool phase4Flag = false;
	bool phase5Flag = false;
	bool phase6Flag = false;
	*/

	//30秒経過するごとに出現する敵の数を増やすための変数
	int EnemySpawnplus = 0;
	//敵の出現を管理するフラグ
	bool EnemySpawnFlag = false;

	int survivalTime_100 = 0; //生存時間100の位
	int survivalTime_10 = 0; //生存時間10の位
	int survivalTime_1 = 0; //生存時間1の位

	int Score_10000 = 0;     //スコア10000の位
	int Score_1000 = 0;     //スコア1000の位
	int Score_100 = 0;      //スコア100の位
	int Score_10 = 0;       //スコア10の位
	int Score_1 = 0;        //スコア1の位

public:
	Stage10() {};
	~Stage10() {};

	void Init() override; // 初期化
	void Uninit() override; // 終了処理
	void Update() override; // 更新

	void MessageUpdate() override;	//会話パート更新
	void GameUpdate() override;		//ゲームパート更新
	void SoundUpdate() override {};	//sound更新
	void IssueUpdate();
	void RePlay();
	void ScoreCheck();	//スコアと生存時間を桁ごとに分ける

	//会話パート
	void BuildStartPages() override;
	void BuildEndPages() override;

	void UpdateEnemySpawn() override;
	void StageClearCheck() override;
	void StageFailedCheck() override;
};

