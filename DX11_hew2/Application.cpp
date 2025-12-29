#include <chrono>
#include <thread>
#include "Application.h"
#include "Game.h"

const auto ClassName = TEXT("2025 framework ひな型");     //ウィンドウクラス名
const auto WindowName = TEXT("2025 framework ひな型");    //ウィンドウ名

HINSTANCE  Application::m_hInst;   // インスタンスハンドル
HWND       Application::m_hWnd;    // ウィンドウハンドル
uint32_t   Application::m_Width;   // ウィンドウの横幅
uint32_t   Application::m_Height;  // ウィンドウの縦幅

int			Application::ogPosX;
int			Application::ogPosY;
uint32_t	Application::og_Width;
uint32_t	Application::og_Height;

//
static int gFrameX = -1;  // ウィンドウの左右の枠の合計
static int gFrameY = -1;  // ウィンドウの上下の枠の合計
static double gAspect = 0.0; // アスペクト比（16:9）

// 枠の太さを測るための関数（計算用）
static void UpdateFrameSize(HWND hWnd) {
	RECT rw, rc;
	GetWindowRect(hWnd, &rw);
	GetClientRect(hWnd, &rc);
	gFrameX = (rw.right - rw.left) - (rc.right - rc.left);
	gFrameY = (rw.bottom - rw.top) - (rc.bottom - rc.top);
}
//

// NVIDIA Optimus 対応

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int   AmdPowerXpressRequestHighPerformance = 0x00000001;
}

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Application::Application(uint32_t width, uint32_t height)
{
	m_Height = height;
	m_Width = width;

	// アスペクト比を保存
	gAspect = (double)width / (double)height;

	timeBeginPeriod(1); //タイマー精度を1ミリ秒に設定
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Application::~Application()
{
	timeEndPeriod(1); // タイマー精度を元に戻す
}

//-----------------------------------------------------------------------------
// 実行
//-----------------------------------------------------------------------------
void Application::Run()
{
	//初期化
	bool okfg = InitApp();
	if (okfg) { MainLoop(); }

	UninitApp(); // 終了処理
}
//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
bool Application::InitApp()
{

	// インスタンスハンドルを取得
	auto hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr)
	{
		return false;
	}

	// ウィンドウの設定
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = ClassName;
	wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

	// ウィンドウの登録
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	// インスタンスハンドル設定
	m_hInst = hInst;

	// ウィンドウのサイズを設定
	RECT rc = {};
	rc.right = static_cast<LONG>(m_Width);
	rc.bottom = static_cast<LONG>(m_Height);

	// ウィンドウサイズを調整
	auto style = WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, FALSE);

	// ここでモニターの中央座標を計算して CreateWindowEx に渡す
	LONG winWidth = rc.right - rc.left;
	LONG winHeight = rc.bottom - rc.top;

	// 画面解像度（プライマリモニタ）を取得
	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);

	// 中央座標を計算
	int posX = (screenW - winWidth) / 2;
	int posY = (screenH - winHeight) / 2;

	// CreateWindowEx に位置とサイズを渡す
	m_hWnd = CreateWindowEx(
		0,
		//        WS_EX_TOPMOST,
		ClassName,
		WindowName,
		style,
		CW_USEDEFAULT,					//	  posX, // 中央 X		
		CW_USEDEFAULT,					//	  posY, // 中央 Y		
		rc.right - rc.left,				//	  winWidth,				
		rc.bottom - rc.top,				//	  winHeight,			
		nullptr,
		nullptr,
		m_hInst,
		nullptr);

	ogPosX = posX;
	ogPosY = posY;
	og_Width = winWidth;
	og_Height = winHeight;



	if (m_hWnd == nullptr)
	{
		return false;
	}
	
	// ウィンドウ枠のサイズを計測
	UpdateFrameSize(m_hWnd);
	og_Width = rc.right - rc.left;
	og_Height = rc.bottom - rc.top;

	// 作業領域（タスクバーを除いた範囲）の中心を計算
	HMONITOR hMon = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = { sizeof(MONITORINFO) };
	GetMonitorInfo(hMon, &mi);

	ogPosX = mi.rcWork.left + (mi.rcWork.right - mi.rcWork.left - (int)og_Width) / 2;
	ogPosY = mi.rcWork.top + (mi.rcWork.bottom - mi.rcWork.top - (int)og_Height) / 2;

	// 計算した中央位置へウィンドウを移動
	SetWindowPos(m_hWnd, NULL, ogPosX, ogPosY, og_Width, og_Height, SWP_NOZORDER | SWP_FRAMECHANGED);
	
	// ウィンドウを表示
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	// ウィンドウを更新
	UpdateWindow(m_hWnd);

	// ウィンドウにフォーカスを設定
	SetFocus(m_hWnd);

	//SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP | WS_MINIMIZEBOX);
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, m_Width, m_Height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	// 正常終了
	return true;

}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Application::UninitApp()
{
	// ウィンドウの登録を解除
	if (m_hInst != nullptr)
	{
		UnregisterClass(ClassName, m_hInst);
	}

	m_hInst = nullptr;
	m_hWnd = nullptr;
}

//-----------------------------------------------------------------------------
// メインループ
//-----------------------------------------------------------------------------
void Application::MainLoop()
{
	MSG msg = {};

	//// ゲームオブジェクト
	//Game game;
	// ゲーム初期化処理
	//game.Init();

	Game::Init(); // // namespace Game 中の Init()を呼ぶ


	// FPS計測用変数
	int fpsCounter = 0;
	long long oldTick = GetTickCount64(); // 前回計測時の時間
	long long nowTick = oldTick; // 今回計測時の時間

	// FPS固定用変数
	LARGE_INTEGER liWork; // workがつく変数は作業用変数
	long long frequency;// どれくらい細かく時間をカウントできるか
	QueryPerformanceFrequency(&liWork);
	frequency = liWork.QuadPart;
	// 時間（単位：カウント）取得
	QueryPerformanceCounter(&liWork);
	long long oldCount = liWork.QuadPart;// 前回計測時の時間
	long long nowCount = oldCount;// 今回計測時の時間


	// ゲームループ
	while (1)
	{
		// 新たにメッセージがあれば
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ウィンドウプロシージャにメッセージを送る
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// 「WM_QUIT」メッセージを受け取ったらループを抜ける
			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else
		{
			QueryPerformanceCounter(&liWork);// 現在時間を取得
			nowCount = liWork.QuadPart;
			// 1/60秒が経過したか？
			if (nowCount >= oldCount + frequency / 60) {

				// ゲーム更新
				Game::Update();

				// ゲーム描画
				Game::Draw();

				fpsCounter++; // ゲーム処理を実行したら＋１する
				oldCount = nowCount;
			}
		}
	}

	// ゲーム終了処理
	Game::Uninit();
}

//-----------------------------------------------------------------------------
// ウィンドウプロシージャ
//-----------------------------------------------------------------------------
LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool isFullscreen = false;
	static bool isMessageBoxShowed = false;
	switch (uMsg)
	{
	// マウスでリサイズ中に比率を強制する
	case WM_SIZING:
	{
		if (isFullscreen) return FALSE;
		RECT* r = (RECT*)lParam;
		if (gFrameX < 0 || gFrameY < 0) UpdateFrameSize(hWnd);

		int clientW = (r->right - r->left) - gFrameX;
		int clientH = (r->bottom - r->top) - gFrameY;

		switch (wParam) {
		case WMSZ_LEFT: case WMSZ_RIGHT: case WMSZ_TOPLEFT: case WMSZ_TOPRIGHT:
		case WMSZ_BOTTOMLEFT: case WMSZ_BOTTOMRIGHT:
			// 横幅に合わせて高さを変える
			r->bottom = r->top + (int)(clientW / gAspect + 0.5) + gFrameY;
			break;
		case WMSZ_TOP: case WMSZ_BOTTOM:
			// 高さに合わせて横幅を変える
			r->right = r->left + (int)(clientH * gAspect + 0.5) + gFrameX;
			break;
		}
		return TRUE;
	}

	case WM_DESTROY:// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);// 「WM_QUIT」メッセージを送る　→　アプリ終了
		break;

	case WM_CLOSE:  // 「x」ボタンが押されたら
	{
		int res = MessageBoxA(NULL, "終了しますか？", "確認", MB_OKCANCEL);
		if (res == IDOK) {
			DestroyWindow(hWnd);  // 「WM_DESTROY」メッセージを送る
		}
	}
	break;

	case WM_KEYDOWN: //キー入力があったメッセージ
		if (LOWORD(wParam) == VK_ESCAPE)
		{ //入力されたキーがESCAPEなら
			PostMessage(hWnd, WM_CLOSE, wParam, lParam);//「WM_CLOSE」を送る
		}

		else if (LOWORD(wParam) == VK_F11)
		{
			isFullscreen = !isFullscreen;
			if (isFullscreen) {
				//フルスクリーンに切り替え
				//g_pSwapChain->SetFullscreenState(TRUE, NULL);
				//ShowWindow(hWnd, SW_MAXIMIZE);

				// 疑似フルスクリーンモードに変更
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_MINIMIZEBOX); // ウィンドウ枠を削除
				// ディスプレイ解像度を取得
				int screenWidth = GetSystemMetrics(SM_CXSCREEN);
				int screenHeight = GetSystemMetrics(SM_CYSCREEN);
				SetWindowPos(hWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
			}
			else {
				//ウィンドウモードに戻す
				//g_pSwapChain->SetFullscreenState(FALSE, NULL);
				//ShowWindow(hWnd, SW_RESTORE);

				// 通常ウィンドウに戻す
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW); // ウィンドウ枠を戻す
				
				
				//SetWindowPos(hWnd, HWND_TOP, ogPosX, ogPosY, og_Width, og_Height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
				SetWindowPos(hWnd, HWND_TOP, 0, 0, 1280, 720, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
			}
		}
		break;

	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE) {
			// フルスクリーン表示かつメッセージボックス非表示なら
			if (isFullscreen && !isMessageBoxShowed)
			{
				// ウインドウを最小化する（タスク切替時に背後に残る問題対策）
				ShowWindow(hWnd, SW_MINIMIZE);
			}
		}
		// 標準挙動を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	case WM_SIZE: //ウィンドウサイズに変更があったメッセージ

		if (wParam != SIZE_MINIMIZED)
		{
			int width = LOWORD(lParam); //横幅
			int height = HIWORD(lParam); //縦幅
			Renderer::ResizeWindow(width, height);
		}
		break;

	default:
		// 受け取ったメッセージに対してデフォルトの処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

