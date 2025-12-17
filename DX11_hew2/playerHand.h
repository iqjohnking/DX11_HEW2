#pragma once

#include "Object.h"		//基底クラス//実は必要なし
#include "Texture2D.h"	//基底クラス
#include "Input.h"		//操作用
//#include "utility.h"	//文字列変換用
#include "silkWall.h"

enum class HandSide
{
	Left,
	Right,
	none
};

class playerHand :public Texture2D
{
private:
	HandSide m_Side = HandSide::none;   // 左手 or 右手

	DirectX::SimpleMath::Vector3 m_Position{ 0.0f, 0.0f, 0.0f };

	float m_Speed           =   10.0f;  // 移動速度（1F あたりの移動量）
	float m_MoveLimitTop    =  300.0f;  // 画面上方向の移動上限
	float m_MoveLimitBottom = -300.0f;  // 画面下方向の移動下限

	DirectX::SimpleMath::Vector3 m_FiledCenter{ 0.0f, 0.0f, 0.0f }; // フィールド中心座標
	float m_Radius			= 400.0f;   // フィールド中心との距離
	float m_FiledAngleRad   =	0.0f;		// 角度（ラジアン）
	float m_AngleSpeed		=	0.05f;  // 毎フレイムラジアン変化量

	playerHand* m_AnotherHand = nullptr;;

public:

	playerHand(int side);
	~playerHand() override = default;

	void Init() override;
	void Update() override;
	//void Draw(Camera* cam);
	//void Uninit();

	void Move();

	void SetSide(HandSide side) { m_Side = side; }
	HandSide GetSide() const { return m_Side; }
	void SetFieldCenter(const DirectX::SimpleMath::Vector3& center) { m_FiledCenter = center; }
	void SetRadius(float radius) { m_Radius = radius; }

	void SetAnotherHand(playerHand* anotherHand)
	{
		m_AnotherHand = anotherHand;
	}



	void SetMoveLimit(float bottom, float top)
	{
		m_MoveLimitBottom = bottom;
		m_MoveLimitTop = top;
	}
};


