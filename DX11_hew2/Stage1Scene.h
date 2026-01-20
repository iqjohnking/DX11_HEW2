#pragma once
#include "Scene.h"
#include "Object.h"

#include <vector>
#include "MessageManager.h"
#include "MessagePage.h"

// 会話の進行状態
//StageBaseに移動予定
enum class Flow { StartTalk, Gameplay, EndTalk };

// Stage1Scenクラス
class Stage1Scene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

	MessageManager* m_Message = nullptr;  // AddObjectで生成したものを保持
	std::vector<MessagePage> m_Pages;

	void BuildStartPages();

	void BuildEndPages();

	Flow m_Flow = Flow::StartTalk;

public:
	Stage1Scene(); // コンストラクタ
	~Stage1Scene(); // デストラクタ

	void Init() override; // 初期化
	void Uninit()override; // 終了処理
	void Update()override; // 更新
};

