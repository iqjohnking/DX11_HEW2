#pragma once
#include "Scene.h"
#include "Object.h"

// ResultSceneクラス
class ResultScene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

public:
	ResultScene(); // コンストラクタ
	~ResultScene(); // デストラクタ

	void Init() override; // 初期化
	void Uninit()override; // 終了処理
	void Update()override; // 更新
};

