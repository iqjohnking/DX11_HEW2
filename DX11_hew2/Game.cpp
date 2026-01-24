#include "Game.h"
#include "Renderer.h"
#include "Input.h"

Game* Game::m_Instance;
Sound Game::m_Sound;

// コンストラクタ
Game::Game()
{
	m_Scene = nullptr;
}

// デストラクタ
Game::~Game()
{
	delete m_Scene;
	DeleteAllObjects();
}

// 初期化
void Game::Init()
{
	// インスタンス生成
	m_Instance = new Game;

	// 描画初期化処理
	Renderer::Init();
	Input::Create();

	// カメラ初期化
	m_Instance->m_Camera.Init();

	//サウンド初期化
	m_Instance->m_Sound.Init();

	// 初期シーンを設定（必須）
	m_Instance->ChangeScene(TITLE);

	//会話シーンテスト用
	//m_Instance->ChangeScene(STAGE4);
}

// 更新
void Game::Update()
{
	Input::Update();

	//シーン更新
	m_Instance->m_Scene->Update();

	// カメラ更新
	m_Instance->m_Camera.Update();

	//オブジェクト更新中
	m_Instance->m_IsUpdatingObjects = true;

	//オブジェクト更新
	for (auto& o : m_Instance->m_Objects) {
		o->Update();
	}
	//オブジェクト更新終了
	m_Instance->m_IsUpdatingObjects = false;

	// ここで削除キューを反映
	m_Instance->ApplyDeleteQueue();

	// 最後に生成キューを反映
	m_Instance->FlushSpawnQueue();
}

// 描画
void Game::Draw()
{
	// 描画前処理
	Renderer::DrawStart();

	// 1) m_Objects から生描画リストを作成
	std::vector<Object*> drawList;
	drawList.reserve(m_Instance->m_Objects.size());
	for (auto& up : m_Instance->m_Objects) {
		drawList.push_back(up.get());
	}
	// 2) 手動描画順で安定ソート（同値は生成順を保持）
	// 小さい順に描画される（背面→前面）

	std::stable_sort(drawList.begin(), drawList.end(),
		[](const Object* a, const Object* b) {
			return a->GetDrawOrder() < b->GetDrawOrder();
		});
	// 3) 並び替えた順で描画
	for (auto* o : drawList) {
		o->Draw(&m_Instance->m_Camera);

		// デバッグ描画は必要に応じてここで
		// if (o->HasCollider()) { ... }
	}


	// 描画後処理
	Renderer::DrawEnd();

	/*
	for (auto& o : m_Instance->m_Objects) {
		o->Draw(&m_Instance->m_Camera);
		//todo: デバッグ描画のON/OFF
		//if (o->HasCollider()) {
		//	//auto box = o->GetCollider();
		//	//m_Instance->m_DebugDrawer.DrawAABB(box, &m_Instance->m_Camera);
		//	//todo: 球体とかも描画
		//}
	}*/

}

// 終了処理
void Game::Uninit()
{
	// カメラ終了処理
	m_Instance->m_Camera.Uninit();

	for (auto& o : m_Instance->m_Objects) {
		o->Uninit();
	}
	Input::Release();
	// 描画終了処理
	Renderer::Uninit();

	//サウンド終了処理
	m_Sound.Uninit();

	//m_Instance->m_DebugDrawer.Uninit();

	// インスタンス削除
	delete m_Instance;
}

Game* Game::GetInstance()
{
	return m_Instance;
}

void Game::ChangeScene(SceneName sceneName)
{
	// 読み込み済みシーンの削除
	if (m_Instance->m_Scene != nullptr) {
		m_Instance->m_Scene->Uninit();
		delete m_Instance->m_Scene;
		m_Instance->m_Scene = nullptr;
	}
	m_Instance->ApplyDeleteQueue();
	m_Instance->DeleteAllObjects();
	switch (sceneName) {
	case TITLE:
		m_Instance->m_Scene = new TitleScene();
		break;
	case STAGE1:
		m_Instance->m_Scene = new Stage1();
		break;
	case STAGE2:
		m_Instance->m_Scene = new Stage2();
		break;
	case STAGE3:
		m_Instance->m_Scene = new Stage3();
		break;
	case STAGE4:
		m_Instance->m_Scene = new Stage4();
		break;
	case RESULT:
		m_Instance->m_Scene = new ResultScene();
		break;
	default:
		break;
	}
	if (m_Instance->m_Scene)
	{
		m_Instance->m_Scene->Init();
		// シーン初期化でAddObjectされたものを全て反映＆Initする
		m_Instance->FlushSpawnQueue();
	}
}

void Game::ChangeOldScene()
{
	// 読み込み済みシーンの削除
	if (m_Instance->m_Scene != nullptr) {
		m_Instance->m_Scene->Uninit();
		delete m_Instance->m_Scene;
		m_Instance->m_Scene = nullptr;
	}
	m_Instance->ApplyDeleteQueue();
	m_Instance->DeleteAllObjects();
	switch (m_OldScene) {
	case TITLE:
		m_Instance->m_Scene = new TitleScene();
		break;
	case STAGE1:
		m_Instance->m_Scene = new Stage1();
		break;
	case STAGE2:
		m_Instance->m_Scene = new Stage2();
		break;
	case STAGE3:
		m_Instance->m_Scene = new Stage3();
		break;
	case RESULT:
		m_Instance->m_Scene = new ResultScene();
		break;
	default:
		break;
	}
	if (m_Instance->m_Scene)
	{
		m_Instance->m_Scene->Init();
		// シーン初期化でAddObjectされたものを全て反映＆Initする
		m_Instance->FlushSpawnQueue();
	}
	//現在のシーンを更新
	m_CurrentScene = m_OldScene;
}

void Game::DeleteObject(Object* ptr)
{
	m_Instance->m_DeleteQueue.push_back(ptr);
	ptr->SetToBeDeleted();
	//if (ptr == nullptr) {
	//	return;
	//}

	//ptr->Uninit();

	////要素の削除
	//erase_if(m_Instance->m_Objects, 
	//	[ptr](const std::unique_ptr<Object>& element) {
	//	return element.get() == ptr;
	//	});
	//m_Instance->m_Objects.shrink_to_fit();
}

void Game::DeleteAllObjects()
{
	for (auto& o : m_Instance->m_Objects) {
		o->Uninit();
	}
	m_Instance->m_Objects.clear();
	m_Instance->m_Objects.shrink_to_fit();
	m_SpawnQueue.clear();
}

void Game::FlushSpawnQueue()
{
	// Init中にAddObjectされても安全にするため、
	// 「今溜まっている分」をローカルへ退避してから処理する。
	// Init中に増えた分は m_SpawnQueue に新しく溜まるので、空になるまで繰り返す。

	while (!m_Instance->m_SpawnQueue.empty())
	{
		// いま溜まっている分をローカルに退避（これ以降 m_SpawnQueue は空）
		std::vector<std::unique_ptr<Object>> batch;
		batch.swap(m_Instance->m_SpawnQueue);

		// batch を m_Objects に移しつつ Init
		for (auto& up : batch)
		{
			if (!up) continue;

			m_Instance->m_Objects.emplace_back(std::move(up));

			Object* raw = m_Instance->m_Objects.back().get();
			if (!raw) continue;

			raw->Init();
		}
		// batch はここで破棄される（中身は全て move 済み）
	}
}

void Game::ApplyDeleteQueue()
{
	/*
	begin / end	配列の範囲指定
	remove_if	消す候補を後ろに集める（まだ消さない）
	erase	実際に消す
	erase + remove_if	vector 削除の安全な唯一の正解*/
	if (m_DeleteQueue.empty()) return;

	// 1つずつ消す
	for (Object* del : m_DeleteQueue)
	{
		if (!del) continue;

		// オブジェクト配列から削除
		{
			auto& objs = m_Objects;
			objs.erase(
				std::remove_if(
					objs.begin(),
					objs.end(),
					[del](const std::unique_ptr<Object>& o)
					{
						if (!o) return true;
						if (o.get() != del) return false;

						// 消す直前に終了処理
						o->Uninit();
						return true;
					}
				),
				objs.end()
			);
		}
	}

	// 削除依頼を処理し終わったのでクリア
	m_DeleteQueue.clear();
}