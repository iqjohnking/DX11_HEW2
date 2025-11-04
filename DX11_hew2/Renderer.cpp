
#include "Renderer.h"
#include "Application.h"


using namespace DirectX::SimpleMath;

//Direct3Dのバージョン
D3D_FEATURE_LEVEL Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

// デバイス＝DirectXの各種機能を作る ※ID3D11で始まるポインタ型の変数は、解放する必要がある
ID3D11Device* Renderer::m_pDevice{};
// コンテキスト＝描画関連を司る機能
ID3D11DeviceContext* Renderer::m_pDeviceContext{};
// スワップチェイン＝ダブルバッファ機能
IDXGISwapChain* Renderer::m_pSwapChain{};
// レンダーターゲット＝描画先を表す機能
ID3D11RenderTargetView* Renderer::m_pRenderTargetView{};
// デプスバッファ
ID3D11DepthStencilView* Renderer::m_pDepthStencilView{};

ID3D11Buffer* Renderer::m_pWorldBuffer{}; // ワールド行列
ID3D11Buffer* Renderer::m_pViewBuffer{}; // ビュー行列
ID3D11Buffer* Renderer::m_pProjectionBuffer{}; // プロジェクション行列
ID3D11Buffer* Renderer::m_pLightBuffer{}; // ライト設定（平行光源

// デプスステンシルステート
ID3D11DepthStencilState* Renderer::m_pDepthStateEnable{};
ID3D11DepthStencilState* Renderer::m_pDepthStateDisable{};

ID3D11BlendState* Renderer::m_pBlendState[MAX_BLENDSTATE]; // ブレンドステート配列
ID3D11BlendState* Renderer::m_pBlendStateATC{}; // 特定のアルファテストとカバレッジ（ATC）用のブレンドステート



//--------------------------------------------------------------------------------------
//初期化処理
//--------------------------------------------------------------------------------------
HRESULT Renderer::Init()
{
	HRESULT hr = S_OK;

	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1; // バックバッファの数を1に設定（ダブルバッファリング）
	swapChainDesc.BufferDesc.Width = Application::GetWidth(); // バッファの幅をウィンドウサイズに合わせる
	swapChainDesc.BufferDesc.Height = Application::GetHeight(); // バッファの高さをウィンドウサイズに合わせる
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // バッファのピクセルフォーマットを設定
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // リフレッシュレートを設定（Hz）
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バッファの使用用途を設定
	swapChainDesc.OutputWindow = Application::GetWindow(); // スワップチェーンのターゲットウィンドウを設定
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプリングの設定（アンチエイリアスのサンプル数とクオリティ）
	swapChainDesc.SampleDesc.Quality = 0; //同上
	swapChainDesc.Windowed = TRUE; // ウィンドウモード（フルスクリーンではなく、ウィンドウモードで実行）

	// デバイスとスワップチェインを同時に作成する関数の呼び出し
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE, // ドライバータイプ(ハードウェアGPUを使用)
		NULL,               // ソフトウェアラスタライザを指定しないのでNULL
		0,                  // フラグ（D3D11_CREATE_DEVICE_DEBUGなど） 0は何も指定しない
		NULL,               // 機能レベルの配列。NULLならデフォルトの機能レベルセットが使われる
		0,                  // 機能レベルの配列の要素数(NULLなら0でOK)
		D3D11_SDK_VERSION,  // SDKのバージョン 常に「D3D11_SDK_VERSION」を指定
		&swapChainDesc,     // スワップチェーンの設定構造体へのポインタ
		&m_pSwapChain,      // 作成されたスワップチェーンを受け取るポインタ
		&m_pDevice,	        // 作成されたデバイスを受け取るポインタ
		&m_FeatureLevel,    // 作成されたデバイスの機能レベルを受け取る変数へのポインタ
		&m_pDeviceContext); // 作成されたデバイスコンテキストを受け取るポインタ
	if (FAILED(hr)) return hr;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, 
		D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, &m_FeatureLevel, &m_pDeviceContext);
	if (FAILED(hr)) return hr;

	// レンダーターゲットビュー・デプスステンシルバッファ・デプスステンシルビュー作成
	hr = CreateRenderAndDepthResources();
	if (FAILED(hr)) return hr;

	// ビューポート設定
	D3D11_VIEWPORT viewport{};
	viewport.Width = (FLOAT)Application::GetWidth();   // ビューポートの幅
	viewport.Height = (FLOAT)Application::GetHeight(); // ビューポートの高さ
	viewport.MinDepth = 0.0f;                          // 深度範囲の最小値
	viewport.MaxDepth = 1.0f;                          // 深度範囲の最大値
	viewport.TopLeftX = 0;                             // ビューポートの左上隅のX座標
	viewport.TopLeftY = 0;                             // ビューポートの左上隅のY座標）
	m_pDeviceContext->RSSetViewports(1, &viewport);


	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; //ソリッド
	//rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME; //ワイヤーフレーム
	rasterizerDesc.CullMode = D3D11_CULL_BACK; //ポリゴン裏をカリング
	//rasterizerDesc.CullMode = D3D11_CULL_FRONT; //ポリゴン表をカリング
	//rasterizerDesc.CullMode = D3D11_CULL_NONE; //カリングしない(裏も表も表示される)
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	ID3D11RasterizerState* rs{};
	hr = m_pDevice->CreateRasterizerState(&rasterizerDesc, &rs);
	if (FAILED(hr)) return hr;
	m_pDeviceContext->RSSetState(rs);

	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc{};
	BlendDesc.AlphaToCoverageEnable = FALSE;                     // アルファ・トゥ・カバレッジを無効化（透明度をカバレッジとして利用しない）
	BlendDesc.IndependentBlendEnable = TRUE;                     // 各レンダーターゲットに対して個別のブレンド設定を有効化
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;               // ブレンドを無効に設定（不透明な描画）
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;  // ソース（描画するピクセル）のアルファ値を使用
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // デスティネーション（既存のピクセル）の逆アルファ値を使用
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;      // ソースとデスティネーションを加算する操作
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;   // ソースのアルファ値をそのまま使用
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // デスティネーションのアルファ値を無視
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // アルファ値に対して加算操作を行う
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // レンダーターゲットのカラーチャンネル書き込みマスク
	hr = m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[0]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	hr = m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[1]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	hr = m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[2]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	hr = m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[3]);
	if (FAILED(hr)) return hr;

	SetBlendState(BS_ALPHABLEND);

	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStateEnable); //深度有効ステート
	if (FAILED(hr)) return hr;

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStateDisable); //深度無効ステート
	if (FAILED(hr)) return hr;

	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStateEnable, NULL);

	// サンプラーステート設定
	D3D11_SAMPLER_DESC smpDesc{};
	smpDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.MaxAnisotropy = 4;
	smpDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	hr = m_pDevice->CreateSamplerState(&smpDesc, &samplerState);
	if (FAILED(hr)) return hr;

	m_pDeviceContext->PSSetSamplers(0, 1, &samplerState);

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(Matrix);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	//０番目の定数バッファにworld行列
	hr = m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pWorldBuffer);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pWorldBuffer);   
	if (FAILED(hr)) return hr;

	//１番目の定数バッファにview行列
	hr = m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pViewBuffer);
	m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pViewBuffer);
	if (FAILED(hr)) return hr;

	//２番目の定数バッファにProjection行列
	hr = m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pProjectionBuffer);
	m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pProjectionBuffer);
	if (FAILED(hr)) return hr;

	//３番目の定数バッファにLight行列

	bufferDesc.ByteWidth = sizeof(LIGHT);

	hr = m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pLightBuffer);
	m_pDeviceContext->VSSetConstantBuffers(3, 1, &m_pLightBuffer);
	if (FAILED(hr)) return hr;

	//Lightの初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = Vector4(0.0f, -1.0f, 0.8f, 0.0f);//方向
	light.Direction.Normalize();
	light.Diffuse = Color(1.5f, 1.5, 1.5f, 1.0f);//平行光源の強さといろ
	light.Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);//環境光の強さといろ
	SetLight(light);

	return S_OK;
}

//--------------------------------------------------------------------------------------
// レンダーターゲットビュー・デプスステンシルバッファ・デプスステンシルビュー作成
//--------------------------------------------------------------------------------------
HRESULT Renderer::CreateRenderAndDepthResources()
{
	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget{};
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	if (FAILED(hr)) return hr;
	if (renderTarget != nullptr) hr = m_pDevice->CreateRenderTargetView(renderTarget, NULL, &m_pRenderTargetView);
	renderTarget->Release();
	if (FAILED(hr)) return hr;

	// デプスステンシルバッファ作成
	// ※（デプスバッファ = 深度バッファ = Zバッファ）→奥行を判定して前後関係を正しく描画できる
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = Application::GetWidth();   // バッファの幅をスワップチェーンに合わせる
	textureDesc.Height = Application::GetHeight(); // バッファの高さをスワップチェーンに合わせる
	textureDesc.MipLevels = 1;                            // ミップレベルは1（ミップマップは使用しない）
	textureDesc.ArraySize = 1;                            // テクスチャの配列サイズ（通常1）
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;           // フォーマットは16ビットの深度バッファを使用
	textureDesc.SampleDesc.Count = 1;                     // スワップチェーンと同じサンプル設定
	textureDesc.SampleDesc.Quality = 0;                   // 同上
	textureDesc.Usage = D3D11_USAGE_DEFAULT;              // 使用方法はデフォルト（GPUで使用）
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;     // 深度ステンシルバッファとして使用
	textureDesc.CPUAccessFlags = 0;                       // CPUからのアクセスは不要
	textureDesc.MiscFlags = 0;                            // その他のフラグは設定なし
	hr = m_pDevice->CreateTexture2D(&textureDesc, NULL, &depthStencile);
	if (FAILED(hr)) return hr;

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format; // デプスステンシルバッファのフォーマットを設定
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // ビューの次元を2Dテクスチャとして設定（2Dテクスチャ用のデプスステンシルビュー）
	depthStencilViewDesc.Flags = 0; // 特別なフラグは設定しない（デフォルトの動作）
	if (depthStencile != nullptr)m_pDevice->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(hr)) return hr;
	depthStencile->Release();

	return S_OK;
}

//--------------------------------------------------------------------------------------
//終了処理
//--------------------------------------------------------------------------------------
void Renderer::Uninit()
{
	m_pDeviceContext->ClearState();

	SAFE_RELEASE(m_pLightBuffer);

	SAFE_RELEASE(m_pWorldBuffer);
	SAFE_RELEASE(m_pViewBuffer);
	SAFE_RELEASE(m_pProjectionBuffer);

	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

//--------------------------------------------------------------------------------------
//描画開始
//--------------------------------------------------------------------------------------
void Renderer::DrawStart()
{
	// 画面塗りつぶし色
	float clearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha

	// 描画先のキャンバスと使用する深度バッファを指定する
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	// 描画先キャンバスを塗りつぶす
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	// 深度バッファをリセットする
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//--------------------------------------------------------------------------------------
//描画終了
//--------------------------------------------------------------------------------------
void Renderer::DrawEnd()
{
	// ダブルバッファの切り替えを行い画面を更新する
	m_pSwapChain->Present(1, 0);
}

void Renderer::SetLight(LIGHT Light)
{
	// LIGHTの設定をGPU側へ送る
	m_pDeviceContext->UpdateSubresource(m_pLightBuffer, 0, NULL, &Light, 0, 0);
}

//--------------------------------------------------------------------------------------
// 深度ステンシルの有効・無効を設定
//--------------------------------------------------------------------------------------
void Renderer::SetDepthEnable(bool Enable)
{
	if (Enable) 
	{
		// 深度テストを有効にするステンシルステートをセット
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStateEnable, NULL);
	}
	else
	{
		// 深度テストを無効にするステンシルステートをセット
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStateDisable, NULL);
	}
}

//--------------------------------------------------------------------------------------
// アルファテストとカバレッジ（ATC）の有効・無効を設定
//--------------------------------------------------------------------------------------
void Renderer::SetATCEnable(bool Enable)
{
	// ブレンドファクター（透明度などの調整に使用）
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
	{
		// アルファテストとカバレッジ (ATC) を有効にするブレンドステートをセット
		m_pDeviceContext->OMSetBlendState(m_pBlendStateATC, blendFactor, 0xffffffff);
	}
	else 
	{
		// 通常のブレンドステートをセット
		m_pDeviceContext->OMSetBlendState(m_pBlendState[0], blendFactor, 0xffffffff);
	}
}

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void Renderer::SetWorldViewProjection2D()
{
	Matrix world = Matrix::Identity;			// 単位行列にする
	world = world.Transpose();			// 転置
	m_pDeviceContext->UpdateSubresource(m_pWorldBuffer, 0, NULL, &world, 0, 0);

	Matrix view = Matrix::Identity;			// 単位行列にする
	view = view.Transpose();			// 転置
	m_pDeviceContext->UpdateSubresource(m_pViewBuffer, 0, NULL, &view, 0, 0);

	// 2D描画を左上原点にする
	Matrix projection = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(Application::GetWidth()),	// ビューボリュームの最小Ｘ
		static_cast<float>(Application::GetHeight()),	// ビューボリュームの最小Ｙ
		0.0f,											// ビューボリュームの最大Ｙ
		0.0f,
		1.0f);

	projection = projection.Transpose();

	m_pDeviceContext->UpdateSubresource(m_pProjectionBuffer, 0, NULL, &projection, 0, 0);
}

//--------------------------------------------------------------------------------------
// ワールド行列を設定
//--------------------------------------------------------------------------------------
void Renderer::SetWorldMatrix(Matrix* WorldMatrix)
{
	Matrix world;
	world = WorldMatrix->Transpose(); // 転置

	// ワールド行列をGPU側へ送る
	m_pDeviceContext->UpdateSubresource(m_pWorldBuffer, 0, NULL, &world, 0, 0);
}

//--------------------------------------------------------------------------------------
// ビュー行列を設定
//--------------------------------------------------------------------------------------
void Renderer::SetViewMatrix(Matrix* ViewMatrix)
{
	Matrix view;
	view = ViewMatrix->Transpose(); // 転置

	// ビュー行列をGPU側へ送る
	m_pDeviceContext->UpdateSubresource(m_pViewBuffer, 0, NULL, &view, 0, 0);
}

//--------------------------------------------------------------------------------------
// プロジェクション行列を設定
//--------------------------------------------------------------------------------------
void Renderer::SetProjectionMatrix(Matrix* ProjectionMatrix)
{
	Matrix projection;
	projection = ProjectionMatrix->Transpose(); // 転置

	// プロジェクション行列をGPU側へ送る
	m_pDeviceContext->UpdateSubresource(m_pProjectionBuffer, 0, NULL, &projection, 0, 0);
}

//--------------------------------------------------------------------------------------
// ウィンドウをリサイズして画面の縦横比を維持する
//--------------------------------------------------------------------------------------
HRESULT Renderer::ResizeWindow(int width, int height)
{
	// スワップチェインが存在しない場合は処理しない
	if (!m_pSwapChain)return S_FALSE;

	// 既存のレンダーターゲットビューを解放
	if (m_pRenderTargetView) {
		m_pRenderTargetView->Release();
		m_pRenderTargetView = nullptr;
	}

	// 既存のデプスステンシルビューを解放
	if (m_pDepthStencilView) {
		m_pDepthStencilView->Release();
		m_pDepthStencilView = nullptr;
	}

	// スワップチェインのバッファサイズを新しいウィンドウサイズに合わせて変更
	m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

	// レンダーターゲットビュー・デプスステンシルバッファ・デプスステンシルビュー作成
	HRESULT hr = CreateRenderAndDepthResources();
	if (FAILED(hr)) return hr;

	// ウィンドウとターゲットのアスペクト比を比較してビューポートを調整
	float windowAspect = (float)width / (float)height;
	float targetAspect = (float)Application::GetWidth() / (float)Application::GetHeight();

	D3D11_VIEWPORT vi = {};

	if (windowAspect > targetAspect) {
		// ウィンドウが横長の場合は高さに合わせて幅を調整
		vi.Height = (float)height;
		vi.Width = height * targetAspect;
		vi.TopLeftX = (width - vi.Width) / 2.0f;
		vi.TopLeftY = 0.0f;
	}
	else {
		// ウィンドウが縦長の場合は幅に合わせて高さを調整
		vi.Width = (float)width;
		vi.Height = width / targetAspect;
		vi.TopLeftX = 0.0f;
		vi.TopLeftY = (height - vi.Height) / 2.0f;
	}
	vi.MinDepth = 0.0f;
	vi.MaxDepth = 1.0f;

	// ビューポートを設定
	m_pDeviceContext->RSSetViewports(1, &vi);

	return S_OK;
}

//--------------------------------------------------------------------------------------
// シェーダーをファイル拡張子に合わせてコンパイル
//--------------------------------------------------------------------------------------
HRESULT Renderer::CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, void** ppShaderObject, int* pShaderObjectSize)
{
	//拡張子csoのファイル名を作成
	char csoFileName[256];
	const char* dot = strrchr(szFileName, '.');  // 最後の '.' を探す
	if (dot) {
		int basenameLen = dot - szFileName;
		strncpy(csoFileName, szFileName, basenameLen); // 拡張子がある場合は拡張子を除いたファイル名をコピー
		csoFileName[basenameLen] = '\0';   // 終端文字を追加
	}
	else {
		strcpy(csoFileName, szFileName);   // 拡張子がない場合はそのままコピー
	}
	strcat(csoFileName, ".cso");// ".cso" 拡張子を付加

	//csoファイルがあれば開く
	FILE* fp;
	int ret = fopen_s(&fp, csoFileName, "rb");
	if (ret == 0)
	{
		// ファイルサイズを取得
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// バイナリデータを読み込み用にメモリ確保
		unsigned char* byteArray = new unsigned char[size];
		fread(byteArray, size, 1, fp);
		fclose(fp);

		// 呼び出し元にポインタとサイズを渡す
		*ppShaderObject = byteArray;
		*pShaderObjectSize = size;
	}
	//csoファイルがなければhlslファイルをコンパイルする
	else
	{
		HRESULT hr = S_OK;
		WCHAR	filename[512];
		size_t 	wLen = 0;
		int err = 0;

		// 文字コードを Shift-JIS → UTF-16 に変換
		setlocale(LC_ALL, "japanese");  // ロケールを設定（Windows特有）
		err = mbstowcs_s(&wLen, filename, 512, szFileName, _TRUNCATE);

		// シェーダーコンパイルオプションを設定
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3DCOMPILE_DEBUG; // デバッグビルドの場合はデバッグ情報も含める
#endif

		// コンパイル結果およびエラー情報格納用のBlob
		ID3DBlob* pErrorBlob = nullptr;
		ID3DBlob* pBlob = nullptr;

		// HLSLファイルをコンパイル
		hr = D3DCompileFromFile(
			filename,							// ファイル名
			nullptr,							// マクロ定義なし 
			D3D_COMPILE_STANDARD_FILE_INCLUDE,	// #include 対応 
			szEntryPoint,						// エントリーポイント名
			szShaderModel,						// シェーダーモデル
			dwShaderFlags,						// コンパイルフラグ
			0,									// エフェクトフラグ
			&pBlob,								// 成功時のコンパイル結果
			&pErrorBlob);						// コンパイルエラー出力

		// コンパイル失敗時のエラーメッセージを表示
		if (FAILED(hr))
		{
			if (pErrorBlob != nullptr) {
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "Error", MB_OK);
			}
			if (pErrorBlob) pErrorBlob->Release();
			if (pBlob)(pBlob)->Release();
			return E_FAIL;
		}

		// エラーブロブがあれば解放
		if (pErrorBlob) pErrorBlob->Release();

		// コンパイル成功時のバイナリデータを呼び出し元に渡す
		*ppShaderObject = (pBlob)->GetBufferPointer();
		*pShaderObjectSize = (pBlob)->GetBufferSize();
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// 頂点シェーダーオブジェクトを生成する
//--------------------------------------------------------------------------------------
HRESULT Renderer::CreateVertexShader(ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout, D3D11_INPUT_ELEMENT_DESC* pLayout, unsigned int numElements, const char* szFileName)
{
	void* ShaderObject;
	int	ShaderObjectSize;

	// ファイルの拡張子に合わせてコンパイル
	HRESULT hr = CompileShader(szFileName, "main", "vs_5_0", &ShaderObject, &ShaderObjectSize);
	if (FAILED(hr)) return E_FAIL;

	// デバイスを使って頂点シェーダーを作成
	hr = m_pDevice->CreateVertexShader(ShaderObject, ShaderObjectSize, NULL, ppVertexShader);

	// デバイスを使って頂点レイアウトを作成
	m_pDevice->CreateInputLayout(pLayout, numElements, ShaderObject, ShaderObjectSize, ppVertexLayout);

	return S_OK;
}

//--------------------------------------------------------------------------------------
// ピクセルシェーダーオブジェクトを生成する
//--------------------------------------------------------------------------------------
HRESULT Renderer::CreatePixelShader(ID3D11PixelShader** ppPixelShader, const char* szFileName)
{
	void* ShaderObject;
	int	ShaderObjectSize;

	// ファイルの拡張子に合わせてコンパイル
	HRESULT hr = CompileShader(szFileName, "main", "ps_5_0", &ShaderObject, &ShaderObjectSize);
	if (FAILED(hr)) return hr;

	// ピクセルシェーダーを生成
	hr = m_pDevice->CreatePixelShader(ShaderObject, ShaderObjectSize, nullptr, ppPixelShader);
	if (FAILED(hr)) return hr;

	return S_OK;
}

//--------------------------------------------------------------------------------------
//インデックスバッファを作成
//--------------------------------------------------------------------------------------
bool Renderer::CreateIndexBuffer(
	unsigned int indexnum,						// インデックス数
	void* indexdata,							// インデックスデータ格納メモリ先頭アドレス
	ID3D11Buffer** pIndexBuffer) {				// インデックスバッファ

	// インデックスバッファ生成
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;								// バッファ使用方
	bd.ByteWidth = sizeof(unsigned int) * indexnum;				// バッファの大き
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;						// インデックスバッファ
	bd.CPUAccessFlags = 0;										// CPUアクセス不要

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indexdata;

	HRESULT hr = m_pDevice->CreateBuffer(&bd, &InitData, pIndexBuffer);
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
//頂点バッファを作成
//--------------------------------------------------------------------------------------
bool Renderer::CreateVertexBuffer(
	unsigned int stride,				// １頂点当たりバイト数
	unsigned int vertexnum,				// 頂点数
	void* vertexdata,					// 頂点データ格納メモリ先頭アドレス
	ID3D11Buffer** pVertexBuffer) {		// 頂点バッファ

	HRESULT hr;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				// バッファ使用方法
	bd.ByteWidth = stride * vertexnum;			// バッファの大きさ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファ
	bd.CPUAccessFlags = 0;						// CPUアクセス不要

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertexdata;				// バッファの初期値

	hr = m_pDevice->CreateBuffer(&bd, &InitData, pVertexBuffer);		// バッファ生成
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
//頂点バッファを作成(CPU書き込み可能)
//--------------------------------------------------------------------------------------
bool Renderer::CreateVertexBufferWrite(
	unsigned int stride,				// １頂点当たりバイト数
	unsigned int vertexnum,				// 頂点数
	void* vertexdata,					// 頂点データ格納メモリ先頭アドレス
	ID3D11Buffer** pVertexBuffer) {		// 頂点バッファ

	HRESULT hr;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;							// バッファ使用方法
	bd.ByteWidth = stride * vertexnum;						// バッファの大きさ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// 頂点バッファ
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				// CPUアクセス可能

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertexdata;							// バッファの初期値

	hr = m_pDevice->CreateBuffer(&bd, &InitData, pVertexBuffer);		// バッファ生成
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
//コンスタントバッファを作成
//--------------------------------------------------------------------------------------
bool Renderer::CreateConstantBuffer(
	unsigned int bytesize,					// コンスタントバッファサイズ
	ID3D11Buffer** pConstantBuffer) {			// コンスタントバッファ

	// コンスタントバッファ生成
	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;								// バッファ使用方法
	bd.ByteWidth = bytesize;									// バッファの大き
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;					// コンスタントバッファ
	bd.CPUAccessFlags = 0;										// CPUアクセス不要

	HRESULT hr = m_pDevice->CreateBuffer(&bd, nullptr, pConstantBuffer);
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
//コンスタントバッファを作成(CPU書き込み可能)
//--------------------------------------------------------------------------------------
bool Renderer::CreateConstantBufferWrite(
	unsigned int bytesize,					// コンスタントバッファサイズ
	ID3D11Buffer** pConstantBuffer) {			// コンスタントバッファ

	// コンスタントバッファ生成
	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;							// バッファ使用方法
	bd.ByteWidth = bytesize;									// バッファの大き
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;					// コンスタントバッファ
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					// CPUアクセス可能

	HRESULT hr = m_pDevice->CreateBuffer(&bd, nullptr, pConstantBuffer);
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}
