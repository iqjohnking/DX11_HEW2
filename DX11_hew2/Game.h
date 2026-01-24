#pragma once
#include <iostream>
//#include "GolfBall.h"
//#include "Ground.h"
#include "Renderer.h"
#include "TitleScene.h"
#include "Stage1Scene.h"
#include "ResultScene.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Stage5.h"
#include "Stage6.h"
#include "Stage7.h"
#include "Stage8.h"
#include "Stage9.h"
#include "Stage10.h"

enum SceneName {
	TITLE,
	STAGE0,
	STAGE1,
	STAGE2,
	STAGE3,
	STAGE4,
	STAGE5,
	STAGE6,
	STAGE7,
	STAGE8,
	STAGE9,
	STAGE10,
	RESULT
};

class Game
{
private:

	// シーン管理
	static Game* m_Instance;
	Scene* m_Scene;

	SceneName m_OldScene;
	SceneName m_CurrentScene;	

	// カメラ
	Camera  m_Camera;

	//サウンド
	static Sound m_Sound;

	// デバッグ用衝突判定描画	

	// テストオブジェクト
	//GolfBall m_Ball;

	//Objects
	std::vector<std::unique_ptr<Object>> m_Objects;

	//追加
	// 生成キュー
	std::vector<std::unique_ptr<Object>> m_SpawnQueue;
	//削除キュー
	std::vector<Object*> m_DeleteQueue;
	// オブジェクト更新中フラグ
	bool m_IsUpdatingObjects = false;

public:
	Game(); // コンストラクタ
	~Game(); // デストラクタ

	static void Init(); // 初期化
	static void Update(); // 更新
	static void Draw(); // 描画
	static void Uninit(); // 終了処理

	static Game* GetInstance();

	//Camera* GetCamera() { return &m_Instance->m_Camera; }
	Camera* GetCamera() { return &m_Camera; }

	void ChangeScene(SceneName sceneName); // シーン変更
	void ChangeOldScene();	//前のシーンに戻る

	void DeleteObject(Object* ptr); // オブジェクト削除
	void DeleteAllObjects(); // オブジェクト全削除

	//追加
	//Spawnキューを反映
	void FlushSpawnQueue();
	//deleteキューを反映
	void ApplyDeleteQueue();

	//サウンドを取得
	static Sound* GetSound() { return &m_Sound; }

	template <typename T, typename... Args>
	T* AddObject(Args&&... args)
	{
		// T(args...) でオブジェクトを生成
		auto ptr = std::make_unique<T>(std::forward<Args>(args)...);

		T* rawPtr = ptr.get();								// 生ポインタを退避

		// Update中は「登録＋Init」を後回し（＝Flushでやる）
		if (m_Instance->m_IsUpdatingObjects)
		{
			m_Instance->m_SpawnQueue.emplace_back(std::move(ptr));
			return rawPtr; // ※この時点ではまだInitされてない
		}

		m_Instance->m_Objects.emplace_back(std::move(ptr)); // vector に格納
		rawPtr->Init();										// 初期化
		return rawPtr;
	}

	// オブジェクトを追加する（※テンプレート関数）// 引数なしバージョン
	/*
	template <typename T> 
	T* AddObject()
	{
		T* ptr = new T;
		m_Instance->m_Objects.emplace_back(ptr);
		ptr->Init();
		return ptr;
	}
	//オブジェクトを取得する（※テンプレート関数）
	template<typename T> 
	std::vector<T*> GetObjects()
	{
		std::vector<T*> result;
		for (auto& obj : m_Instance->m_Objects)
		{
			//dynamic_castで型変換をcheck
			if (T* deriveObj = dynamic_cast<T*>(obj.get())) {
				result.emplace_back(deriveObj);
			}
		}
		return result;
	}

	*/

	// オブジェクトを追加する（※テンプレート関数）
	// 任意個数の引数 Args... を取り、そのまま T のコンストラクタに渡す
	/*template <typename T, typename... Args>
	T* AddObject(Args&&... args)
	{
		// T(args...) でオブジェクトを生成
		auto ptr = std::make_unique<T>(std::forward<Args>(args)...);

		T* rawPtr = ptr.get();								// 生ポインタを退避
		m_Instance->m_Objects.emplace_back(std::move(ptr)); // vector に格納
		rawPtr->Init();										// 初期化
		return rawPtr;
	}*/

	// オブジェクトを取得する（※テンプレート関数）
	template<typename T>
	std::vector<T*> GetObjects()
	{
		std::vector<T*> result;
		for (auto& obj : m_Instance->m_Objects)
		{
			// dynamic_castで型変換をcheck
			if (T* deriveObj = dynamic_cast<T*>(obj.get())) {
				result.emplace_back(deriveObj);
			}
		}
		return result;
	}

	//オブジェクトの位置を取得
	template<typename T>
	std::vector<DirectX::SimpleMath::Vector3> GetPositions()
	{
		std::vector<DirectX::SimpleMath::Vector3> result;
		for (auto& obj : m_Objects)
		{
			if (T* d = dynamic_cast<T*>(obj.get()))
				result.emplace_back(d->GetPosition());
		}
		return result;
	}
};
