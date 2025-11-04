#include	"TestPlane.h"

using namespace DirectX::SimpleMath;

//=======================================
//初期化処理
//=======================================
void TestPlane::Init()
{
	// 頂点データ
	m_Vertices.resize(4);

	m_Vertices[0].position = Vector3(-10,  0,  10);
	m_Vertices[1].position = Vector3( 10,  0,  10);
	m_Vertices[2].position = Vector3(-10,  0, -10);
	m_Vertices[3].position = Vector3( 10,  0, -10);

	m_Vertices[0].color = Color(1, 1, 1, 1);
	m_Vertices[1].color = Color(1, 1, 1, 1);
	m_Vertices[2].color = Color(1, 1, 1, 1);
	m_Vertices[3].color = Color(1, 1, 1, 1);


	//[0](0, 0)[1](1, 0)
	//[2](0, 1)[3](1, 1)
	m_Vertices[0].uv = Vector2(0, 0);
	m_Vertices[1].uv = Vector2(1, 0);
	m_Vertices[2].uv = Vector2(0, 1);
	m_Vertices[3].uv = Vector2(1, 1);

	// 頂点バッファ生成
	m_VertexBuffer.Create(m_Vertices);

	// インデックデータ
	m_Indices.resize(4);

	m_Indices[0] = 0;
	m_Indices[1] = 1;
	m_Indices[2] = 2;
	m_Indices[3] = 3;

	// インデックスバッファ生成
	m_IndexBuffer.Create(m_Indices);

	// シェーダオブジェクト生成
	m_Shader.Create("shader/unlitTextureVS.hlsl","shader/unlitTexturePS.hlsl");

	// テクスチャロード
	bool sts = m_Texture.Load("assets/texture/field.jpg");
	assert(sts == true);

	m_Position.y = -40;
	m_Scale.x = 4;
	m_Scale.z = 4;


}

//=======================================
//更新処理
//=======================================
void TestPlane::Update()
{
	//static float t = 0;
	//t += 0.01f;
	//static float oldtime = 0;
	//oldtime = GetTickCount64()/1000.f;
	//static float degree = 0;
	//degree = DirectX::XMConvertToRadians(cosf(oldtime) * 90.f);

	//m_Rotation.x += 0.01f;
	//m_Rotation.x = degree;
	//m_Rotation.x = 1.5f ;

	//m_Position.z += 0.1f;

}

//=======================================
//描画処理
//=======================================
void TestPlane::Draw(Camera* cam)
{
	//カメラを選択する
	cam->SetCamera();

	// SRT情報作成
	//Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.x, m_Rotation.y, m_Rotation.z); //???変な感じ、xyz軸に沿って回転するではない???
	Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z); 
	Matrix t = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	Matrix s = Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix worldmtx;
	worldmtx = s * r * t;
	Renderer::SetWorldMatrix(&worldmtx); // GPUにセット

	// 描画の処理
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// トポロジーをセット（プリミティブタイプ）
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();
	m_Texture.SetGPU();

	devicecontext->DrawIndexed(
		m_Indices.size(),	// 描画するインデックス数
		0,					// 最初のインデックスバッファの位置
		0);
}

//=======================================
//終了処理
//=======================================
void TestPlane::Uninit()
{

}
