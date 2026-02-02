#pragma once
#include "Texture2D.h"

class UI_control : public Object
{
protected:
	Texture2D m_control2D;

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;
};
