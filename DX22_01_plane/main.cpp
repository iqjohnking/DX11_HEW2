#include    "main.h"
#include    "Application.h"

//=======================================
//エントリーポイント
//=======================================
int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//defined(DEBUG) || defined(_DEBUG)

	//FreeConsole();

	// アプリケーション実行
	Application app(SCREEN_WIDTH, SCREEN_HEIGHT);
	app.Run();

	return 0;
}