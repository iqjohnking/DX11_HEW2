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

class silkWall;   // 前方宣言

// TitleSceneクラス
class TitleScene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

	silkWall* m_SilkWalls[3]{};   // 場に出せる糸の壁は最大3つ
	playerHand* m_HandL = nullptr; // 左手
	playerHand* m_HandR = nullptr; // 右手
	int       m_NextSilkIndex = 0; // 次に使うインデックス

	void Init(); // 初期化
	void Uninit(); // 終了処理

public:
	TitleScene(); // コンストラクタ
	~TitleScene(); // デストラクタ

	void Update(); // 更新
};

