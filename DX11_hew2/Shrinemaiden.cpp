#include "Shrinemaiden.h"
using namespace DirectX::SimpleMath;

void Shrinemaiden::Init()
{
	shrinemaiden_pos = Vector3(0.0f, 0.0f, 0.0f);
	stage_center = Vector3(0.0f, 0.0f, 0.0f);
	alive_flg_Shrinemaiden = true;
}

void Shrinemaiden::Update()
{
	old_shrinemaiden_pos = shrinemaiden_pos;
	SetPosition(shrinemaiden_pos);
}

void Shrinemaiden::Shrinemaiden_move()
{
	//範囲内の敵の位置を全て算出
	//そこから中心点を算出し、移動先を算出
	//移動開始
	//もし移動先に壁があれば
}
