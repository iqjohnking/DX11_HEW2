#include "Game.h"
#include "Renderer.h"

// コンストラクタ
Game::Game()
{

}

// デストラクタ
Game::~Game()
{

}

// 初期化
void Game::Init()
{
	// 描画終了処理
	Renderer::Init();

	// カメラ初期化
	m_Camera.Init();

	// テストオブジェクト初期化
	m_Plane.Init();
	m_Cube.Init();
}

// 更新
void Game::Update()
{
	// カメラ更新
	m_Camera.Update();

	// テストオブジェクト更新
	m_Plane.Update();
	m_Cube.Update();
}

// 描画
void Game::Draw()
{
	// 描画前処理
	Renderer::DrawStart();

	// テストオブジェクト描画
	m_Cube.Draw(&m_Camera);
	//Renderer::SetDepthEnable(0);
	m_Plane.Draw(&m_Camera);
	//Renderer::SetDepthEnable(1);

	// 描画後処理
	Renderer::DrawEnd();
}

// 終了処理
void Game::Uninit()
{
	// カメラ終了処理
	m_Camera.Uninit();

	// テストオブジェクト終了処理
	m_Plane.Uninit();
	m_Cube.Uninit();

	// 描画終了処理
	Renderer::Uninit();
}


