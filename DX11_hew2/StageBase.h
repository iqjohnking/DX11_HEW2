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
#include "Enemy4.h"
#include "EnemyMayu.h"

#include <vector>
#include "MessageManager.h"
#include "MessagePage.h"

//#include "Enemy2.h"	
//#include "Enemy3.h"

// 会話の進行状態
//StageBaseに移動予定
enum class Flow { StartTalk, Gameplay, EndTalk };

// Stage_Baseクラス
class StageBase : public Scene
{
protected:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

	MessageManager* m_Message = nullptr;  // AddObjectで生成したものを保持
	std::vector<MessagePage> m_Pages;
	
	Flow m_Flow;

	Field* m_Field = nullptr;      // フィールド（境界判定用）

	silkWall* m_SilkWalls[3]{};   // 糸の壁 最大3本
	Enemy1* Enemy1List[60]{};  // テスト用に60体
	Enemy4* Enemy4List[10]{};  // テスト用に10体
	playerHand* m_HandL = nullptr; // 左手
	playerHand* m_HandR = nullptr; // 右手
	int         m_NextSilkIndex = 0; // 次に使う糸

	Shrinemaiden* m_Miko = nullptr;     // 巫女

	int killCount = 0;	//倒した敵の数
	int Clearkill;		//クリアに必要な倒した数

	//敵がスポーンするかどうかのフラグ
	bool EnemySpawnFlag = false;

	//ランダム
	float rand = 0.0f;

public:
	StageBase();  // コンストラクタ
	~StageBase(); // デストラクタ

	virtual void Init() = 0; // 初期化
	virtual void Uninit() = 0; // 終了処理
	virtual void Update() = 0; // 更新

	//会話パート
	virtual void BuildStartPages() = 0;
	virtual void BuildEndPages() = 0;

	void EnemyrandomSpawn();
	void StageClearCheck();
	void StageFailedCheck();

	uint64_t get_rand_range(uint64_t min_val, uint64_t max_val) {
		// 乱数生成器
		static std::mt19937_64 mt64(0);

		// [min_val, max_val] の一様分布整数 (double) の分布生成器
		std::uniform_real_distribution<double> get_rand_uni_real(min_val, max_val);

		// 乱数を生成
		return get_rand_uni_real(mt64);
	}
};