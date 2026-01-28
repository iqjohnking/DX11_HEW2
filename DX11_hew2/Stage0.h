#pragma once
#include "StageBase.h"

class Stage0 : public StageBase
{
private:
	//敵の出現を管理するためのフラグ
	bool phase1Flag = false;
	bool phase2Flag = false;
	bool phase3Flag = false;

	//フェーズごとのUIフラグ
	bool phase1UIFlag = false;
	bool phase2UIFlag = false;
	bool phase3UIFlag = false;

	//SoundFlg
	bool m_Conversation_BGM_flg_1 = false;
	bool m_Conversation_BGM_flg_2 = false;

	//チュートリアル用UI
	Texture2D* m_tutorialarrow = nullptr;
	Texture2D* m_tutorialarrow_sankaku = nullptr;
	Texture2D* m_tutorialstagetext000 = nullptr;
	Texture2D* m_tutorialstagetext001 = nullptr;
	Texture2D* m_tutorialstagetext002 = nullptr;

public:
	Stage0() {};
	~Stage0() {};

	void Init() override; // 初期化
	void Uninit() override; // 終了処理
	void Update() override; // 更新

	void MessageUpdate() override;	//会話パート更新
	void GameUpdate() override;		//ゲームパート更新
	void SoundUpdate() override;	//sound更新
	void UIUpdate();				//チュートリアルUI更新

	//会話パート
	void BuildStartPages() override;
	void BuildEndPages() override;

	void UpdateEnemySpawn() override;
	void EnemySpawnSpeedzero(EnemyType enemyType, DirectX::SimpleMath::Vector3 pos);
	void StageClearCheck() override;
	void StageFailedCheck() override;
};

