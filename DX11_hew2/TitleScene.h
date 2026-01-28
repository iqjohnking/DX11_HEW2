#pragma once
#include "Scene.h"
#include "Object.h"
#include "Game.h"
#include "Input.h"
#include "Collision.h"
#include "TriangleSilk.h"

#include "Texture2D.h"
#include "TitleBG.h"
#include "silkWall.h"
#include "playerHand.h"
#include "UI_redSilk.h"
#include "Field.h"
#include "Shrinemaiden.h"
#include "EnemyBase.h"	
#include "Enemy1.h"	
#include "Enemy2.h"
#include "Enemy3.h"
#include "Enemy4.h"
#include "Hashira.h"
#include "EnemyMayu.h"	

//#include "Enemy2.h"	
//#include "Enemy3.h"	

// TitleSceneクラス
class TitleScene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

	Field* m_Field = nullptr;      // フィールド（境界判定用）

	silkWall*   m_SilkWalls[3]{};   // 糸の壁 最大3本
	Enemy1*     Enemy1List[60]{};  // テスト用に60体
	Enemy2*		Enemy2List[10]{};  // テスト用に10体
	Enemy3*		Enemy3List[10]{};  // テスト用に10体
	Enemy4*		Enemy4List[10]{};  // テスト用に10体
	Hashira*	hashiras[10]{};  // テスト用に10体
	playerHand* m_HandL = nullptr; // 左手
	playerHand* m_HandR = nullptr; // 右手
	UI_redSilk*  m_UI_redSilk;    // UI用の赤い糸表示
	int         m_NextSilkIndex = 0; // 次に使う糸

	int m_SilkCount = 0;

	Shrinemaiden* m_Miko = nullptr;     // 巫女

public:

	TitleScene();  // コンストラクタ
	~TitleScene(); // デストラクタ

	void Init() override; // 初期化
	void Uninit()override; // 終了処理
	void Update()override; // 更新
};

