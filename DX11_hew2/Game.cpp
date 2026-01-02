#include "Game.h"
#include "Renderer.h"
#include "Input.h"

Game* Game::m_Instance;

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

	// 初期シーンを設定（必須）
	m_Instance->ChangeScene(TITLE);

}

// 更新
void Game::Update()
{
	Input::Update();

	//シーン更新
	m_Instance->m_Scene->Update();

	// カメラ更新
	m_Instance->m_Camera.Update();

	//オブジェクト更新
	for (auto& o : m_Instance->m_Objects) {
		o->Update();
	}

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
		delete m_Instance->m_Scene;
		m_Instance->m_Scene = nullptr;
	}
	switch (sceneName) {
	case TITLE:
		m_Instance->m_Scene = new TitleScene();
		break;
	case STAGE1:
		m_Instance->m_Scene = new Stage1Scene();
		break;
	case RESULT:
		m_Instance->m_Scene = new ResultScene();
		break;
	default:
		break;
	}

}

void Game::DeleteObject(Object* ptr)
{
	if (ptr == nullptr) {
		return;
	}

	ptr->Uninit();

	//要素の削除
	erase_if(m_Instance->m_Objects, 
		[ptr](const std::unique_ptr<Object>& element) {
		return element.get() == ptr;
		});
	m_Instance->m_Objects.shrink_to_fit();
}

void Game::DeleteAllObjects()
{
	for (auto& o : m_Instance->m_Objects) {
		o->Uninit();
	}
	m_Instance->m_Objects.clear();
	m_Instance->m_Objects.shrink_to_fit();

}


