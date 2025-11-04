#include	"TestCube.h"

using namespace DirectX::SimpleMath;

//=======================================
//初期化処理
//=======================================
void TestCube::Init()
{
	// 頂点データ
	m_Vertices.resize(24);
	float u1;
	float u2;
	float v1;
	float v2;

	//	　　　５￣￣￣￣￣￣￣2
	//	　　／　　　　　　　／｜
	//　　／　　　　　　　／　｜
	//　６￣￣￣￣￣￣￣１　　｜
	//  ｜　　　　　　　｜　　｜
	//  ｜　　　　　　　｜　　｜
	//  ｜　　４　　　　｜　　３
	//  ｜　　　　　　　｜　／
	//  ｜　　　　　　　｜／
	//　７￣￣￣￣￣￣￣０
	//
	// 0  1  2  3  4  5  6  7 
	// 8  9 10 11 12 13 14 15 
	//16 17 18 19 20 21 22 23

	// +X 面（右）（３）ok
	m_Vertices[0].position = Vector3(10, -10, -10);
	m_Vertices[1].position = Vector3(10, 10, -10);
	m_Vertices[2].position = Vector3(10, 10, 10);
	m_Vertices[3].position = Vector3(10, -10, 10);
	m_Vertices[0].normal = m_Vertices[1].normal = m_Vertices[2].normal = m_Vertices[3].normal = Vector3(1, 0, 0);
	
	u1 = 0.66f;
	u2 = 1.00f;
	v1 = 0.00f;
	v2 = 0.50f;
	m_Vertices[1].uv = Vector2(u1, v1);
	m_Vertices[2].uv = Vector2(u2, v1);
	m_Vertices[0].uv = Vector2(u1, v2);
	m_Vertices[3].uv = Vector2(u2, v2);

	m_Vertices[0].color = Vector4(1,1,1,1);
	m_Vertices[1].color = Vector4(1,1,1,1);
	m_Vertices[2].color = Vector4(1,1,1,1);
	m_Vertices[3].color = Vector4(1,1,1,1);

	// -X 面（左）（４）ok
	m_Vertices[4].position = Vector3(-10, -10, 10);
	m_Vertices[5].position = Vector3(-10, 10, 10);
	m_Vertices[6].position = Vector3(-10, 10, -10);
	m_Vertices[7].position = Vector3(-10, -10, -10);
	m_Vertices[4].normal = m_Vertices[5].normal = m_Vertices[6].normal = m_Vertices[7].normal = Vector3(-1, 0, 0);

	u1 = 0.00f;
	u2 = 0.33f;
	v1 = 0.50f;
	v2 = 1.00f;
	m_Vertices[5].uv = Vector2(u1, v1);
	m_Vertices[6].uv = Vector2(u2, v1);
	m_Vertices[4].uv = Vector2(u1, v2);
	m_Vertices[7].uv = Vector2(u2, v2);

	m_Vertices[4].color = Vector4(1, 1, 1, 1);
	m_Vertices[5].color = Vector4(1, 1, 1, 1);
	m_Vertices[6].color = Vector4(1, 1, 1, 1);
	m_Vertices[7].color = Vector4(1, 1, 1, 1);

	// +Y 面（上）（１）ok
	m_Vertices[ 8].position = Vector3(-10, 10,  10);
	m_Vertices[ 9].position = Vector3( 10, 10,  10);
	m_Vertices[11].position = Vector3(-10, 10, -10);
	m_Vertices[10].position = Vector3( 10, 10, -10);
	m_Vertices[ 8].normal = m_Vertices[9].normal = m_Vertices[10].normal = m_Vertices[11].normal = Vector3(0, 1, 0);

	u1 = 0.00f;
	u2 = 0.33f;
	v1 = 0.00f;
	v2 = 0.50f;
	m_Vertices[ 9].uv = Vector2(u1, v1);
	m_Vertices[10].uv = Vector2(u2, v1);
	m_Vertices[ 8].uv = Vector2(u1, v2);
	m_Vertices[11].uv = Vector2(u2, v2);

	m_Vertices[ 8].color = Vector4(1, 1, 1, 1);
	m_Vertices[ 9].color = Vector4(1, 1, 1, 1);
	m_Vertices[10].color = Vector4(1, 1, 1, 1);
	m_Vertices[11].color = Vector4(1, 1, 1, 1);

	// -Y 面（下）（６）
	m_Vertices[12].position = Vector3(-10, -10, -10);
	m_Vertices[13].position = Vector3(10, -10, -10);
	m_Vertices[14].position = Vector3(10, -10, 10);
	m_Vertices[15].position = Vector3(-10, -10, 10);
	m_Vertices[12].normal = m_Vertices[13].normal = m_Vertices[14].normal = m_Vertices[15].normal = Vector3(0, -1, 0);

	u1 = 0.66f;
	u2 = 1.00f;
	v1 = 0.50f;
	v2 = 1.00f;
	m_Vertices[12].uv = Vector2(u1, v1);
	m_Vertices[13].uv = Vector2(u2, v1);
	m_Vertices[15].uv = Vector2(u1, v2);
	m_Vertices[14].uv = Vector2(u2, v2);
	m_Vertices[12].color = Vector4(1, 1, 1, 1);
	m_Vertices[13].color = Vector4(1, 1, 1, 1);
	m_Vertices[14].color = Vector4(1, 1, 1, 1);
	m_Vertices[15].color = Vector4(1, 1, 1, 1);

	// +Z 面（前）（５）
	m_Vertices[16].position = Vector3(-10, -10, 10);
	m_Vertices[17].position = Vector3(10, -10, 10);
	m_Vertices[18].position = Vector3(10, 10, 10);
	m_Vertices[19].position = Vector3(-10, 10, 10);
	m_Vertices[16].normal = m_Vertices[17].normal = m_Vertices[18].normal = m_Vertices[19].normal = Vector3(0, 0, 1);

	u1 = 0.33f;
	u2 = 0.66f;
	v1 = 0.50f;
	v2 = 1.00f;
	m_Vertices[16].uv = Vector2(u1, v1);
	m_Vertices[17].uv = Vector2(u2, v1);
	m_Vertices[19].uv = Vector2(u1, v2);
	m_Vertices[18].uv = Vector2(u2, v2);
	m_Vertices[16].color = Vector4(1, 1, 1, 1);
	m_Vertices[17].color = Vector4(1, 1, 1, 1);
	m_Vertices[18].color = Vector4(1, 1, 1, 1);
	m_Vertices[19].color = Vector4(1, 1, 1, 1);

	// -Z 面（後）（２）
	m_Vertices[20].position = Vector3(10, -10, -10);
	m_Vertices[21].position = Vector3(-10, -10, -10);
	m_Vertices[22].position = Vector3(-10, 10, -10);
	m_Vertices[23].position = Vector3( 10,  10, -10);
	m_Vertices[20].normal = m_Vertices[21].normal = m_Vertices[22].normal = m_Vertices[23].normal = Vector3(0, 0, -1);

	u1 = 0.33f;
	u2 = 0.66f;
	v1 = 0.00f;
	v2 = 0.50f;
	m_Vertices[22].uv = Vector2(u1, v1);
	m_Vertices[23].uv = Vector2(u2, v1);
	m_Vertices[21].uv = Vector2(u1, v2);
	m_Vertices[20].uv = Vector2(u2, v2);

	m_Vertices[20].color = Vector4(1, 1, 1, 1);
	m_Vertices[21].color = Vector4(1, 1, 1, 1);
	m_Vertices[22].color = Vector4(1, 1, 1, 1);
	m_Vertices[23].color = Vector4(1, 1, 1, 1);

	// 白
	//for (auto& v : m_Vertices) {
	//	v.color = Color(1, 1, 1, 1);
	//}

	// 頂点バッファ生成
	m_VertexBuffer.Create(m_Vertices);

	// インデックデータ
	
	m_Indices.resize(36);
	// インデックス設定
	m_Indices = {
		0,1,2, 0,2,3,
		4,5,6, 4,6,7,
		8,9,10, 8,10,11,
		12,13,14, 12,14,15,
		16,17,18, 16,18,19,
		20,21,22, 20,22,23
	};

	// インデックスバッファ生成
	m_IndexBuffer.Create(m_Indices);

	// シェーダオブジェクト生成
	//m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	m_Shader.Create("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");

	// テクスチャロード
	bool sts = m_Texture.Load("assets/texture/dice.png");
	assert(sts == true);

}

//=======================================
//更新処理
//=======================================
void TestCube::Update()
{
	static float oldtime = 0;
	static float degree = 0;
	oldtime += 0.01;
	degree = DirectX::XMConvertToRadians(cosf(oldtime) * 30.f);

	m_Rotation.y = oldtime;
	m_Rotation.x = oldtime;
	//m_Rotation.z = DirectX::XMConvertToRadians(90);
	//static float degreePitch = 0.f;
	//static float degreeYaw = 0.f;
	//static float degreeRoll = 0.f;
	//degreePitch += 1.0f;
	//degreeYaw += 1.0f;
	//degreeRoll += 1.0f;
	//m_Rotation.x = DirectX::XMConvertToRadians(degreePitch);
	//m_Rotation.y = DirectX::XMConvertToRadians(degreeYaw);
	//m_Rotation.z = DirectX::XMConvertToRadians(degreeRoll);

}

//=======================================
//描画処理
//=======================================
void TestCube::Draw(Camera* cam)
{
	//カメラを選択する
	cam->SetCamera();

	// SRT情報作成
	Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z); //xyz軸に沿って回転する
	Matrix t = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	Matrix s = Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix worldmtx;
	worldmtx = s * r * t;
	Renderer::SetWorldMatrix(&worldmtx); // GPUにセット

	// 描画の処理
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// トポロジーをセット（プリミティブタイプ）
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //変更します
	//devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

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
void TestCube::Uninit()
{

}
