#pragma once
#include "Scene.h"
#include "Object.h"

#include <vector>
#include "MessageManager.h"
#include "MessagePage.h"

// Stage1Scenクラス
class Stage1Scene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

	MessageManager* m_Message = nullptr;  // AddObjectで生成したものを保持
	std::vector<MessagePage> m_TestPages;

	void BuildTestPages();

public:
	Stage1Scene(); // コンストラクタ
	~Stage1Scene(); // デストラクタ

	void Init() override; // 初期化
	void Uninit()override; // 終了処理
	void Update()override; // 更新
};

