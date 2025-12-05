#include "Renderer.h"
#include "Camera.h"
#include "Application.h"
#include "Input.h"

using namespace DirectX::SimpleMath;

//=======================================
//初期化処理
//=======================================
void Camera::Init()
{
	m_Position = Vector3(0.0f, 20.0f, -50.0f);	// カメラの初期位置（X=0, Y=20, Z=-50）
	m_Target = Vector3(0.0f, 0.0f, 0.0f);		// 狙う位置（カメラが見ている位置
	m_CameraDirH = PI;							// カメラの水平回転角度（約180度の向き）
	m_CameraDirV = 0.0f;						// カメラの
}


//=======================================
//更新処理
//=======================================
void Camera::Update()
{
	m_CameraSpdH = isInvertedH ? -0.02f : 0.02f;
	m_CameraSpdV = isInvertedV ? -0.02f : 0.02f;


	//=========================
	// 1. カメラリセット/マニュアル操作
	//=========================
	if (Input::GetKeyTrigger(VK_Q)) {
		ResetBehindTarget();
	}

	if (m_IsAligningBehind)
	{
		float alignSpeed = 0.15f;   // 補間速度

		m_CameraDirH = LerpAngle(m_CameraDirH, m_AlignTargetH, alignSpeed);
		m_CameraDirV = LerpAngle(m_CameraDirV, m_AlignTargetV, alignSpeed);

		// 到着判定
		if (fabs(m_CameraDirH - m_AlignTargetH) < 0.01f &&
			fabs(m_CameraDirV - m_AlignTargetV) < 0.01f)
		{
			m_IsAligningBehind = false;
			m_CameraDirH = m_AlignTargetH;
			m_CameraDirV = m_AlignTargetV;
		}
	}
	else
	{
		// INPUTに応じて、カメラの回転角度を操作する
		if (Input::GetKeyPress(VK_LEFT)) {
			m_CameraDirH += m_CameraSpdH;
		}
		if (Input::GetKeyPress(VK_RIGHT)) {
			m_CameraDirH -= m_CameraSpdH;
		}
		if (Input::GetKeyPress(VK_UP)) {
			m_CameraDirV += m_CameraSpdV;
		}
		if (Input::GetKeyPress(VK_DOWN)) {
			m_CameraDirV -= m_CameraSpdV;
		}
	}

	if (Input::GetKeyTrigger(VK_P)) {
		isInvertedH = !isInvertedH;
		isInvertedV = !isInvertedV;
	}

	//=========================
	// 3. 角度正規化＆制限
	//=========================
	// 水平角を1周の範囲に制限する
	//const float TWO_PI = 6.283185307f;
	m_CameraDirH = fmod(m_CameraDirH, TWO_PI);
	if (m_CameraDirH < 0) {
		m_CameraDirH += TWO_PI;
	}
	// 仰角の制限（真上・真下は見せない）
	if (m_CameraDirV > 1.3f) m_CameraDirV = 1.3f;  // 約75°
	if (m_CameraDirV < -1.3f) m_CameraDirV = -1.3f;


	//=========================
	// 4. 最終角度 → 計算
	//=========================
	Vector3 pos = m_Target;
	float radius = 50.0f;
	pos.x += cos(m_CameraDirV) * sin(m_CameraDirH) * radius;
	pos.y += sin(m_CameraDirV) * radius + 20.0f;
	pos.z += cos(m_CameraDirV) * cos(m_CameraDirH) * radius;

	m_Position = pos;
}

//=======================================
//描画処理
//=======================================
void Camera::SetCamera(int mode)
{
	//3D
	if (mode == 0)
	{
		// ビュー変換後列作成
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up); //左手系//この関数は自動的にカメラをm_Target向けに

		Renderer::SetViewMatrix(&m_ViewMatrix);

		//プロジェクション行列の生成
		constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角 // ConvertToRadians() は角度をラジアンに変換する関数です。

		float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
		float nearPlane = 1.0f;			 // ニアクリップ
		float farPlane = 1000.0f;		 // ファークリップ

		Matrix projectionMatrix;
		projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	//左手系

		Renderer::SetProjectionMatrix(&projectionMatrix);
	}
	else if (mode == 1)//2D
	{
		// ビュー変換後列作成
		Vector3 pos  = Vector3(0.0f, 0.0f, -10.0f);
		Vector3 tgt  = Vector3(0.0f, 0.0f, 1.0f);
		Vector3 up   = Vector3(0.0f, 1.0f, 0.0f);
		m_ViewMatrix = DirectX::XMMatrixLookAtLH(pos, tgt, up); 

		Renderer::SetViewMatrix(&m_ViewMatrix);

		//プロジェクション行列の生成
		float nearPlane = 1.0f;			 // ニアクリップ
		float farPlane  = 1000.0f;		 // ファークリップ

		Matrix projectionMatrix;
		
		projectionMatrix =	DirectX::XMMatrixOrthographicLH(
							static_cast<float>(Application::GetWidth()),
							static_cast<float>(Application::GetHeight()),
							nearPlane, farPlane);	//左手系			// 2D 用の正射影行列を作成する
		
		projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);// シェーダーが正しく解釈できるように行列を転置する
		Renderer::SetProjectionMatrix(&projectionMatrix);
	}

}


//=======================================
//終了処理
//=======================================
void Camera::Uninit()
{

}

void Camera::SetTarget(DirectX::SimpleMath::Vector3 target)
{
	m_Target = target;
}
void Camera::SetTargetYaw(float yaw)
{
	m_TargetYaw = yaw;
}


DirectX::SimpleMath::Vector3 Camera::GetTarget()
{
	return m_Target;
}

DirectX::SimpleMath::Vector3 Camera::GetPosition()
{
	return m_Position;
}



//=======================================
/// 以下はカメラをターゲットの後ろにリセットする関数
//=======================================
void Camera::ResetBehindTarget()
{
	m_IsAligningBehind = true;  // カメラリセット中フラグON

	m_AlignTargetH = m_TargetYaw + PI;
	//m_AlignTargetH = m_TargetYaw ;
	//m_AlignTargetV = m_CameraDirV;
	m_AlignTargetV = 0.0f;
}



//=======================================
/// 計算用関数
//=======================================
float Camera::Lerp(float current, float target, float t)
{
	return current + (target - current) * t;
}
float Camera::LerpAngle(float current, float target, float t)
{
	float diff = target - current;

	while (diff > PI) diff -= TWO_PI;
	while (diff < -PI) diff += TWO_PI;

	return current + diff * t;
}

