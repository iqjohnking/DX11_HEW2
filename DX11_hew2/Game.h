#pragma once
#include <iostream>

#include "TestPlane.h"
#include "TestCube.h"

class Game
{
private:

	// カメラ
	Camera  m_Camera;

	// テストオブジェクト
	TestPlane m_Plane;
	TestCube m_Cube;

public:
	Game(); // コンストラクタ
	~Game(); // デストラクタ

	void Init(); // 初期化
	void Update(); // 更新
	void Draw(); // 描画
	void Uninit(); // 終了処理
};
