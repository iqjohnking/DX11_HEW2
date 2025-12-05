#pragma once

#include	<SimpleMath.h>

//-----------------------------------------------------------------------------
//Cameraクラス
//-----------------------------------------------------------------------------
class Camera {
private:
	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};
	
	static constexpr float	TWO_PI = 6.283185307f;
	static constexpr float		PI = 3.1415926535;

	float m_CameraDirH = 0;
	float m_CameraDirV = 0;
	float m_CameraSpdH = 0.2f;
	float m_CameraSpdV = 0.2f;
	bool isInvertedH = 0;
	bool isInvertedV = 0;

	float m_TargetYaw = 0;

	// 入力確認用変数
	bool hasManualInput = false;

	// カメラりセット用の変数
	bool  m_IsAligningBehind = false;	// 
	float m_AlignTargetH = 0.0f;		// 
	float m_AlignTargetV = 0.0f;		// 

	//計算系関数
	static float Lerp(float current, float target, float t);
	static float LerpAngle(float current, float target, float t);


public:
	void Init();
	void Update();
	void Uninit();

	void SetCamera(int mode = 0);

	///////////////////

	DirectX::SimpleMath::Vector3 GetTarget();
	void SetTarget(DirectX::SimpleMath::Vector3 target);

	DirectX::SimpleMath::Vector3 GetPosition();

	void SetTargetYaw(float yaw);

	void ResetBehindTarget();



};