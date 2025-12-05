#include "GolfBall.h"
#include "Input.h"
#include "Collision.h"
#include "Game.h"
#include "Ground.h"

using namespace std;
using namespace DirectX::SimpleMath;

GolfBall::GolfBall(Camera* cam)
{
	SetCamera(cam);
}

GolfBall::~GolfBall()
{
}

//=======================================
//初期化処理
//=======================================
void GolfBall::Init()
{

	// メッシュ読み込み
	StaticMesh staticmesh;

	//3Dモデルデータ
	//u8string modelFile = u8"assets/model/cylinder/cylinder.obj";
	u8string modelFile = u8"assets/model/golfball/golf_ball.obj";

	//テクスチャディレクトリ
	//string texDirectory = "assets/model/cylinder";
	string texDirectory = "assets/model/golfball";

	//Meshを読み込む
	string tmpStr1(reinterpret_cast<const char*>(modelFile.c_str()), modelFile.size());
	staticmesh.Load(tmpStr1, texDirectory);

	m_MeshRenderer.Init(staticmesh);

	// シェーダオブジェクト生成
	m_Shader.Create("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");

	// サブセット情報取得
	m_subsets = staticmesh.GetSubsets();

	// テクスチャ情報取得
	m_Textures = staticmesh.GetTextures();

	// マテリアル情報取得	
	vector<MATERIAL> materials = staticmesh.GetMaterials();

	// マテリアル数分ループ
	for (int i = 0; i < materials.size(); i++)
	{
		// マテリアルオブジェクト生成
		unique_ptr<Material> m = make_unique<Material>();

		// マテリアル情報をセット
		m->Create(materials[i]);

		// マテリアルオブジェクトを配列に追加
		m_Materials.push_back(move(m));
	}

	//モデルによってスケールを調整
	m_Scale.x = 5;
	m_Scale.y = 5;
	m_Scale.z = 5;

	//初速度
	m_Velocity.x = 0.00f;
	m_Position.x = -10.00f;
}

//=======================================
//更新処理
//=======================================
void GolfBall::Update()
{
	Vector3 oldPos = m_Position;
	// 物理パラメータ
	const float dt = 1.0f / 60.0f;		// Δt（1フレームあたりの時間）
	const float accelPerFrame = 0.35f;  // 毎frameの加速度
	const float maxSpeed = 1.80f;		// 最大速度
	const float decelPower = 0.05f;		// 減速度
	const float stopEpsilon = 0.001f;	// 
	const float gravityAccel = 9.8f;          // 重力加速度
	const float gravityPerFrame = gravityAccel * dt;


	//ボールを操作する処理

	// ====== 1) 入力方向（カメラ基準） ======
	Vector3 dir(0, 0, 0);
	if (m_Cam)
	{
		Vector3 camFwd = GetPosition() - m_Cam->GetPosition();
		camFwd.y = 0.0f;								//y軸成分を無視して水平ベクトルにする
		if (camFwd.LengthSquared() > 0) camFwd.Normalize();

		Vector3 up(0.0f, 1.0f, 0.0f);
		//「上向き × 前向き = 右向き」的外積（クロス積）
		Vector3 camRight(
			up.y * camFwd.z - up.z * camFwd.y,
			up.z * camFwd.x - up.x * camFwd.z,
			up.x * camFwd.y - up.y * camFwd.x
		);
		if (camRight.LengthSquared() > 0.0f) { camRight.Normalize(); }

		if (Input::GetKeyPress(VK_A)) dir -= camRight; // 左 = 反向右
		if (Input::GetKeyPress(VK_D)) dir += camRight; // 右
		if (Input::GetKeyPress(VK_W)) dir += camFwd;   // 前
		if (Input::GetKeyPress(VK_S)) dir -= camFwd;   // 後
	}


	// ====== 2) 速度更新（入力による加速） ======

	if (dir.LengthSquared() > 0.0f)
	{
		dir.Normalize();
		m_Velocity += dir * accelPerFrame; // 1フレームぶんの加速

		// 水平速度の上限
		Vector3 velXZ(m_Velocity.x, 0.0f, m_Velocity.z);
		float spd2 = velXZ.LengthSquared();
		if (spd2 > maxSpeed * maxSpeed)
		{
			float spd = sqrt(spd2);
			velXZ /= spd;
			velXZ *= maxSpeed;
			m_Velocity.x = velXZ.x;
			m_Velocity.z = velXZ.z;
		}
	}
	// 3) velocity更新/正規化/速度制限/減速
	else {
		float spd2 = m_Velocity.LengthSquared();

		if (spd2 < stopEpsilon)
		{
			// ほぼ止まっているなら完全にゼロにする
			m_Velocity = Vector3::Zero;
		}
		else
		{
			Vector3 decel = -m_Velocity; // 速度の逆方向
			decel.Normalize();           // 方向だけ取り出す
			decel *= decelPower;         // 減速度（1フレームぶん）

			m_Velocity += decel;         // 逆向き加速度を足して減速
		}
	}

	// ====== 4) 重力 ======
	m_Velocity.y -= gravityPerFrame;

	// ====== 5) 座標更新 ======
	m_Position += m_Velocity;

	//float radius = 2.0f * m_Scale.x; // 球の半径
	float radius = m_Scale.x; // 球の半径

	// Groundのverticesデータを取得
	//vector<VERTEX_3D> vertices = m_Ground->GetVertices();
	vector<Ground*> grounds = Game::GetInstance()->GetObjects<Ground>();
	vector<VERTEX_3D> vertices;
	for (auto g : grounds) {
		vector<VERTEX_3D> vecs = g->GetVertices();
		for (auto v : vecs) {
			vertices.emplace_back(v);
		}
	}

	float moveDistance = 9999.0f;
	Vector3 contactPoint;
	Vector3 normal;
	// 地面との当たり判定
	for (int i = 0; i < vertices.size(); i += 3)
	{
		Collision::Polygon collisionPolygon = {
			vertices[i + 0].position,
			vertices[i + 1].position,
			vertices[i + 2].position
		};
		Vector3 cp;
		Collision::Segment collisionSegment = { oldPos, m_Position };
		Collision::Sphere collisionSphere = { m_Position,radius };

		bool isSegmentHit = Collision::CheckHit(collisionSegment, collisionPolygon, cp);
		bool isHit = Collision::CheckHit(collisionSphere, collisionPolygon, cp);

		if (isSegmentHit) {
			float md = 0;
			Vector3 np = Collision::moveSphere(collisionSegment, radius, collisionPolygon, cp, md);
			if (moveDistance > md)
			{
				moveDistance = md;
				m_Position = np;
				contactPoint = cp;
				normal = Collision::GetNormal(collisionPolygon);
			}
		}
		else if (isHit)
		{
			Vector3 np = Collision::moveSphere(collisionSphere, collisionPolygon, cp);
			float md = (np - oldPos).Length();
			if (moveDistance > md)
			{
				moveDistance = md;
				m_Position = np;
				contactPoint = cp;
				normal = Collision::GetNormal(collisionPolygon);
			}
		}
	}
	if (moveDistance != 9999.0f)
	{
		//MessageBoxA(NULL, "当たった", "当たり判定確認", MB_OK);
		// ballの速度ベクトル法線方向成分と接線方向成分に分解
		float velocityNormal = Collision::Dot(m_Velocity, normal);
		Vector3 v1 = velocityNormal * normal; // 法線方向成分
		Vector3 v2 = m_Velocity - v1;          // 接線方向成分
		// 反射ベクトルを計算
		const float restitution = 0.8f; // 反発係数
		const float friction = 0.9f;    // 摩擦係数
		Vector3 reflectedVelocity = v2 * friction - v1 * restitution;
		m_Velocity = reflectedVelocity;
	}

	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	// ---- face to move direction (XZ) ----
	const float turnSpeedPerFrame = 0.314f;        // 
	const float moveEpsilon2 = 1e-6f;        // 
	const float steepDotThreshold = 0.95f;     // 斜面の「急さ」判定用（cosθ）※調整用

	// 速度がほぼゼロなら何もしない
	if (m_Velocity.LengthSquared() > moveEpsilon2)
	{
		bool hasInput = (dir.LengthSquared() > 0.0f);

		bool shouldTurn = false;

		if (hasInput)
		{
			// 入力があるときは常に向きを合わせる
			shouldTurn = true;
		}
		else
		{
			// 入力がないときは「急な斜面で接地しているときだけ」向きを合わせる
			if (moveDistance != 9999.0f) // 今フレームで地面と当たっている
			{
				Vector3 up(0.0f, 1.0f, 0.0f);
				Vector3 n = normal;
				n.Normalize(); // 念のため正規化

				float dotNU = n.x * up.x + n.y * up.y + n.z * up.z;
				// dotNU = cos(斜面の傾き角度)
				//   1.0  : 完全に水平
				//   0.0  : 完全に垂直
				//   0.8  : 約36.9度以上傾いている

				if (dotNU < steepDotThreshold)
				{
					// 一定以上に傾いている斜面 → 急な坂とみなす
					shouldTurn = true;
				}
			}
		}

		if (shouldTurn)
		{
			float targetYaw = atan2(m_Velocity.x, m_Velocity.z);
			float currentYaw = m_Rotation.y;

			float delta = targetYaw - currentYaw;

			while (delta > PI)  delta -= TWO_PI;
			while (delta < -PI)  delta += TWO_PI;

			if (delta > turnSpeedPerFrame)  delta = turnSpeedPerFrame;
			if (delta < -turnSpeedPerFrame)  delta = -turnSpeedPerFrame;

			currentYaw += delta;

			if (currentYaw > PI)  currentYaw -= TWO_PI;
			if (currentYaw < -PI)  currentYaw += TWO_PI;


			m_Rotation.y = currentYaw;
		}
	}
	//camera追従
	if (m_Cam) {
		m_Cam->SetTarget(m_Position);    // 目標の座標をセットする
		m_Cam->SetTargetYaw(GetYaw());   // 目標の向きをセットする
	}
}

//=======================================
//描画処理
//=======================================
void GolfBall::Draw(Camera* cam)
{
	//カメラを選択する
	cam->SetCamera();
	//cam->SetTarget(m_Position);    // 目標の座標をセットする
	//cam->SetTargetYaw(GetYaw());   // 目標の向きをセットする

	// SRT情報作成
	Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	Matrix t = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	Matrix s = Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix worldmtx;
	worldmtx = s * r * t;
	Renderer::SetWorldMatrix(&worldmtx); // GPUにセット

	m_Shader.SetGPU();

	// インデックスバッファ・頂点バッファをセット
	m_MeshRenderer.BeforeDraw();

	//マテリアル数分ループ 
	for (int i = 0; i < m_subsets.size(); i++)
	{
		// マテリアルをセット(サブセット情報の中にあるマテリアルインデックスを使用)
		m_Materials[m_subsets[i].MaterialIdx]->SetGPU();

		if (m_Materials[m_subsets[i].MaterialIdx]->isTextureEnable())
		{
			m_Textures[m_subsets[i].MaterialIdx]->SetGPU();
		}

		m_MeshRenderer.DrawSubset(
			m_subsets[i].IndexNum,		// 描画するインデックス数
			m_subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
			m_subsets[i].VertexBase);	// 頂点バッファの最初から使用
	}
}

//=======================================
//終了処理
//=======================================
void GolfBall::Uninit()
{

}

// セットの必要はもういない
// GameクラスのインスタンスからGround情報を取得するようにしたため
//void GolfBall::SetGround(Ground* ground)
//{
//	m_Ground = ground;
//}

float GolfBall::GetYaw()
{
	return m_Rotation.y;
}

float GolfBall::GetRoll()
{
	return m_Rotation.z;
}

float GolfBall::GetPitch()
{
	return m_Rotation.x;
}

