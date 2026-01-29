#pragma once
#include "Texture2D.h"

class UI_mikoHp : public Object
{
protected:
	Texture2D m_Texture2D;
	Object* m_miko = nullptr;

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	void SetMiko(Object* miko)
	{
		m_miko = miko;
	}
};
