#pragma once
#include <iostream>

//#include "GolfBall.h"
//#include "Ground.h"

#include "Renderer.h"
#include "TitleScene.h"
#include "Stage1Scene.h"
#include "ResultScene.h"

enum SceneName {
	TITLE,
	STAGE1,
	RESULT

};


class Game
{
private:

	// シーン管理
	static Game* m_Instance;
	Scene* m_Scene;

	// カメラ
	Camera  m_Camera;

	// テストオブジェクト
	//GolfBall m_Ball;

	//Objects
	std::vector<std::unique_ptr<Object>> m_Objects;

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
	void DeleteObject(Object* ptr); // オブジェクト削除
	void DeleteAllObjects(); // オブジェクト全削除

	// オブジェクトを追加する（※テンプレート関数）
	// 引数なしバージョン
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
	template <typename T, typename... Args>
	T* AddObject(Args&&... args)
	{
		// T(args...) でオブジェクトを生成
		auto ptr = std::make_unique<T>(std::forward<Args>(args)...);

		T* rawPtr = ptr.get();								// 生ポインタを退避
		m_Instance->m_Objects.emplace_back(std::move(ptr)); // vector に格納
		rawPtr->Init();										// 初期化
		return rawPtr;
	}

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


};
