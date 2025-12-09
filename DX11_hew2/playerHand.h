#pragma once

#include "Object.h"		//基底クラス//必要なし実は
#include "Texture2D.h"	//基底クラス
#include "Input.h"		//操作用
#include "utility.h"	//文字列変換用

enum class HandSide
{
	Left,
	Right,
};

class playerHand :public Object
{
private:


public:
	void Init();
	void Update();
	void Draw(Camera* cam);
	void Uninit();

};


