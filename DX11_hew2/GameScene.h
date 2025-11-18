#pragma once
#include <iostream>

#include "SceneBase.h"
#include "TestPlane.h"
#include "TestCube.h"

class GameScene : public SceneBase
{
private:

	// カメラ
	Camera  m_Camera;

	// テストオブジェクト
	TestPlane m_Plane;
	TestCube m_Cube;

public:
	GameScene(); // コンストラクタ
	~GameScene()override; // デストラクタ

	void Init()override; // 初期化
	void Update()override; // 更新
	void Draw()override; // 描画
	void Uninit()override; // 終了処理
};
