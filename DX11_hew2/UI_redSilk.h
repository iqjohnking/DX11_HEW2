#pragma once
#include "Texture2D.h"

class UI_redSilk : public Object
{
protected:
	Texture2D m_Texture2D;

	DirectX::SimpleMath::Vector3 m_StartPos;
	DirectX::SimpleMath::Vector3 m_EndPos;

	Object* m_LeftHand = nullptr;
	Object* m_RightHand = nullptr;

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	void SetHands(Object* left, Object* right)
	{
		m_LeftHand = left;
		m_RightHand = right;
	}
};
