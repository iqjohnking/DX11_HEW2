#pragma once
#include "Scene.h"
#include "Object.h"
//#include"StageBase.h"
//
#include <vector>
#include "MessageManager.h"
#include "MessagePage.h"
//
// 会話の進行状態
enum class Flow { StartTalk, Gameplay, EndTalk };
//// Stage1Scenクラス
class Stage1Scene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

	MessageManager* m_Message = nullptr;  // AddObjectで生成したものを保持
	std::vector<MessagePage> m_Pages;

	void BuildStartPages();

	void BuildEndPages();

	Flow m_Flow;

public:
	Stage1Scene(); // コンストラクタ
	~Stage1Scene(); // デストラクタ

	void Init(); // 初期化
	void Uninit(); // 終了処理
	void Update(); // 更新
};
