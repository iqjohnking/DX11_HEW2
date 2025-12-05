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



	//オブジェクトを追加
	/*
	m_Objects.emplace_back(new GolfBall(&m_Camera));
	m_Objects.emplace_back(new Ground);

	//オブジェクト初期化
	for (auto& o: m_Objects) {
		o->Init();
	}
	//ゴルフボールにGroundのポインタをセット
	dynamic_cast<GolfBall*>(m_Objects[0].get())->SetGround(dynamic_cast<Ground*>(m_Objects[1].get()));
	*/
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

	for (auto& o : m_Instance->m_Objects) {
		o->Draw(&m_Instance->m_Camera);
	}

	// 描画後処理
	Renderer::DrawEnd();
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


