#pragma once
#include "Scene.h"
#include "Object.h"
#include "Input.h"
#include "Collision.h"
#include "TriangleSilk.h"

#include "Texture2D.h"
#include "TitleBG.h"
#include "silkWall.h"
#include "playerHand.h"
#include "Field.h"
#include "Shrinemaiden.h"
#include "EnemyBase.h"
#include "Enemy1.h"	
#include "Enemy2.h"
#include "Enemy4.h"
#include "EnemyMayu.h"
#include "EnemyType.h"

#include <vector>
#include "MessageManager.h"
#include "MessagePage.h"

//#include "Enemy3.h"

// StageBaseクラス
class StageBase : public Scene
{
protected:
	// 会話の進行状態
	enum class Flow { StartTalk, Gameplay, EndTalk };

	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

	MessageManager* m_Message = nullptr;  // AddObjectで生成したものを保持
	std::vector<MessagePage> m_Pages;
	
	Flow m_Flow = Flow::StartTalk;

	Field* m_Field = nullptr;      // フィールド（境界判定用）

	silkWall* m_SilkWalls[3]{};   // 糸の壁 最大3本
	Enemy1* Enemy1List[60]{};  // テスト用に60体
	Enemy2* Enemy2List[10]{};  // テスト用に10体
	Enemy4* Enemy4List[10]{};  // テスト用に10体
	playerHand* m_HandL = nullptr; // 左手
	playerHand* m_HandR = nullptr; // 右手
	int         m_NextSilkIndex = 0; // 次に使う糸

	Shrinemaiden* m_Miko = nullptr;     // 巫女

	int StagekillCount = 0;	//倒した敵の数
	int StageEnemyCount = 0; //ステージの敵の総数

	//経過したフレーム数
	int elapsedFrames = 0;
	//経過した秒数
	int elapsedSeconds = 0;

	//敵がスポーンするかどうかのフラグ
	bool EnemySpawnFlag = false;

	//ランダム
	float rand = 0.0f;
	
public:
	StageBase() {};  // コンストラクタ
	~StageBase() {}; // デストラクタ

	virtual void Init() = 0; // 初期化
	virtual void Uninit() = 0; // 終了処理
	virtual void Update() = 0; // 更新

	virtual void MessageUpdate() = 0;	//会話パート更新
	virtual void GameUpdate() = 0;		//ゲームパート更新

	virtual void BuildStartPages() = 0;
	virtual void BuildEndPages() = 0;


	virtual void EnemySpawn(EnemyType enemyType,DirectX::SimpleMath::Vector3 pos);
	virtual void StageClearCheck() = 0;
	virtual void StageFailedCheck() = 0;

	uint64_t get_rand_range(uint64_t min_val, uint64_t max_val)
	{
		// 確実に min_val <= max_val にする
		if (min_val > max_val) std::swap(min_val, max_val);

		// 乱数生成エンジン（初期化は一度だけ）
		static std::mt19937_64 mt64{ 0 };

		// 一様分布生成器
		std::uniform_int_distribution<uint64_t> dist(min_val, max_val);

		return dist(mt64);
	}
};