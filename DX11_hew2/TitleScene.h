#pragma once
#include "Scene.h"
#include "Object.h"
#include "Game.h"
#include "Input.h"
#include "Collision.h"

#include "Texture2D.h"
#include "TitleBG.h"
#include "silkWall.h"
#include "playerHand.h"
#include "field.h"
#include "Shrinemaiden.h"
#include "Enemy_base.h"	
#include "Enemy1.h"	
//#include "Enemy2.h"	
//#include "Enemy3.h"	

//class silkWall;   // 前方宣言 //何のために？

// TitleSceneクラス
class TitleScene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

	silkWall*   m_SilkWalls[3]{};   // 糸の壁 最大3本
	Enemy1*     Enemy1List[60]{};  // テスト用に60体
	playerHand* m_HandL = nullptr; // 左手
	playerHand* m_HandR = nullptr; // 右手
	int         m_NextSilkIndex = 0; // 次に使う糸

	Field*        m_Field  = nullptr;      // フィールド（境界判定用）
	Shrinemaiden* m_Shrine = nullptr;     // 巫女

	void Init();   // 初期化
	void Uninit(); // 終了処理

public:
	TitleScene();  // コンストラクタ
	~TitleScene(); // デストラクタ

	void Update() override; // 更新
};

