#pragma once
#include "Enemy_base.h"

class Enemy1 : public Enemy_base
{
protected:

	//ˆê”Ô•’Ê‚¾‚©‚ç‰½‚à‚È‚¢

public:
	void Init() override;
	void Update() override;

	//›Ş—‚ğ’Ç‚¢‚©‚¯‚é‚½‚ß‚ÌŠÖ”
	void move() override;

	//“G‚ğ’Ç‰Á‚·‚éŠÖ”
	//void Add_Enemy();
};