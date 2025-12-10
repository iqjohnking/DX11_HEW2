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

	float m_Speed = 10.0f;     // 移動速度（1F あたりの移動量）
	float m_MoveLimitTop = 300.0f;    // 画面上方向の移動上限
	float m_MoveLimitBottom = -300.0f;   // 画面下方向の移動下限

public:

	playerHand(int side);
	~playerHand() override = default;

	void Init() override;
	void Update() override;
	//void Draw(Camera* cam);
	//void Uninit();

	void SetSide(HandSide side) { m_Side = side; }
	HandSide GetSide() const { return m_Side; }

	void SetMoveLimit(float bottom, float top)
	{
		m_MoveLimitBottom = bottom;
		m_MoveLimitTop = top;
	}
};


