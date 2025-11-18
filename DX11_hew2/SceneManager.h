#pragma once
#include "SceneBase.h"

enum Scenes
{
	MENU,
	GAME,
	RESULT,
	QUIT
};

class SceneManager
{
private:
	Scenes scene;
	SceneBase* currentScene = nullptr;  //現在のシーン

	//(1)コンストラクタ、デストラクタをprivate実装にする
	SceneManager();
	~SceneManager();

	//コピー禁止
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	//移動禁止
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

public:
	static SceneManager& GetInstance();

	void Init();
	void Update();
	void Draw();
	void ChangeScene(Scenes nextScene);
	void Uninit();

};