#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<SimpleMath.h>
#include	<io.h>
#include	<string>
#include	<vector>
#include	<d3dcompiler.h>
#include	<locale.h>

//外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

// Direct3D解放の簡略化マクロ
#define SAFE_RELEASE(p) { if( NULL != p ) { p->Release(); p = NULL; } }

// ３Ｄ頂点データ
struct VERTEX_3D
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 normal;
	DirectX::SimpleMath::Color color;
	DirectX::SimpleMath::Vector2 uv;
};

// ブレンドステート
enum EBlendState {
	BS_NONE = 0,							// 半透明合成無し
	BS_ALPHABLEND,							// 半透明合成
	BS_ADDITIVE,							// 加算合成
	BS_SUBTRACTION,							// 減算合成
	MAX_BLENDSTATE
};

//平行光源
struct LIGHT
{
	BOOL Enable;			//光を使うか否かのフラグ
	BOOL Dummy[3];			
	DirectX::SimpleMath::Vector4	Direction;		//平行光源の方向
	DirectX::SimpleMath::Color		Diffuse;		//平行光源の強さと色
	DirectX::SimpleMath::Color		Ambient;		//環境光の強さと色
	
};

//サブセット//CPU側
struct SUBSET {
	std::string MtrlName;			//マテリアル名 
	unsigned int IndexNum	= 0;	//インデックス数
	unsigned int VertexNum	= 0;	//頂点数
	unsigned int IndexBase	= 0;	//開始インデックス
	unsigned int VertexBase = 0;	//頂点ベース
	unsigned int MaterialIdx = 0;	//マテリアルの番号
};

//マテリアル//GPU側
struct MATERIAL
{
	DirectX::SimpleMath::Color		Ambient;		//環境反射
	DirectX::SimpleMath::Color		Diffuse;		//拡散反射（≒カラー
	DirectX::SimpleMath::Color		Specular;		//鏡面反射
	DirectX::SimpleMath::Color		Emission;		//発光
	float Shiness;									//光沢の滑らかさ
	BOOL TextureEnable;								//テクスチャの使うか否かのフラグ
	BOOL Dummy[2];									//alignment padding用
};


//-----------------------------------------------------------------------------
//Rendererクラス
//-----------------------------------------------------------------------------
class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_pDevice;
	static ID3D11DeviceContext*    m_pDeviceContext;
	static IDXGISwapChain*         m_pSwapChain;
	static ID3D11RenderTargetView* m_pRenderTargetView;
	static ID3D11DepthStencilView* m_pDepthStencilView;

	static ID3D11Buffer*			m_pWorldBuffer;
	static ID3D11Buffer*			m_pViewBuffer;
	static ID3D11Buffer*			m_pProjectionBuffer;

	static ID3D11Buffer*			m_pLightBuffer;		//
	static ID3D11Buffer*			m_pMaterialBuffer;	//マテリアルバファー
	static ID3D11Buffer*			m_pTextureBuffer;	//UV情報

	static ID3D11DepthStencilState* m_pDepthStateEnable;
	static ID3D11DepthStencilState* m_pDepthStateDisable;

	static ID3D11BlendState*		m_pBlendState[MAX_BLENDSTATE]; // ブレンド ステート;
	static ID3D11BlendState*		m_pBlendStateATC;

	static HRESULT CreateRenderAndDepthResources();

public:

	static HRESULT Init();
	static void Uninit();
	static void DrawStart();
	static void DrawEnd();

	static HRESULT ResizeWindow(int width, int height);
		
	static void SetDepthEnable(bool Enable);

	static void SetATCEnable(bool Enable);

	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(DirectX::SimpleMath::Matrix* WorldMatrix);
	static void SetViewMatrix(DirectX::SimpleMath::Matrix* ViewMatrix);
	static void SetProjectionMatrix(DirectX::SimpleMath::Matrix* ProjectionMatrix);

	static ID3D11Device* GetDevice( void ){ return m_pDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_pDeviceContext; }

	static HRESULT CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, void** ppShaderObject, int* pShaderObjectSize);
	static HRESULT CreateVertexShader(ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout, D3D11_INPUT_ELEMENT_DESC* pLayout, unsigned int numElements, const char* szFileName);
	static HRESULT CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	static bool CreateIndexBuffer(unsigned int indexnum, void* indexdata, ID3D11Buffer** pIndexBuffer);
	static bool CreateVertexBuffer(unsigned int stride, unsigned int vertexnum, void* vertexdata, ID3D11Buffer** pVertexBuffer);
	static bool CreateVertexBufferWrite(unsigned int stride, unsigned int vertexnum, void* vertexdata, ID3D11Buffer** pVertexBuffer);

	static bool CreateConstantBuffer(unsigned int bytesize, ID3D11Buffer** pConstantBuffer);
	static bool CreateConstantBufferWrite(unsigned int bytesize, ID3D11Buffer** pConstantBuffer);
	
	static void SetLight(LIGHT Light);
	static void SetMaterial(MATERIAL Material);
	static void SetUV(float u, float v, float uw, float vh);

	//=============================================================================
	// ブレンド ステート設定
	//=============================================================================
	static void SetBlendState(int nBlendState)
	{
		if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			m_pDeviceContext->OMSetBlendState(m_pBlendState[nBlendState], blendFactor, 0xffffffff);
		}
	}
};