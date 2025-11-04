#pragma once

#include    <Windows.h>
#include    <cstdint>

//-----------------------------------------------------------------------------
// Applicationクラス
//-----------------------------------------------------------------------------
class Application
{
public:
	Application(uint32_t width, uint32_t height);
	~Application();
	void Run();

	static uint32_t GetWidth() { return m_Width; }// 幅を取得
	static uint32_t GetHeight() { return m_Height; }// 高さを取得
	static HWND GetWindow() { return m_hWnd; }// ウインドウハンドルを返す

private:
	static HINSTANCE   m_hInst;        // インスタンスハンドル
	static HWND        m_hWnd;         // ウィンドウハンドル
	static uint32_t    m_Width;        // ウィンドウの横幅
	static uint32_t    m_Height;       // ウィンドウの縦幅

	static bool InitApp(); //初期化
	static void UninitApp(); //終了処理
	static void MainLoop(); //メインループ

	//プロシージャ
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};