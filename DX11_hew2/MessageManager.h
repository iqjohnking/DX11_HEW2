#pragma once
#include "Object.h"

//‰ï˜b‘S‘Ì‚ÌŠÇ—
class MessageManager : public Object
{
private:
	//MessageFrameUI
	//TextUI
	//MIKO
	//KUMO

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;
};

