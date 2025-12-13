#pragma once
#include "Object.h"

class Enemy :public Object
{
protected:
	//�G�̑��x
	DirectX::SimpleMath::Vector3 enemy_speed;

	//�G�̑O�̍��W
	DirectX::SimpleMath::Vector3 old_enemy_pos;

	//�ޏ���ǂ������邽�߂̍��W�z��
	DirectX::SimpleMath::Vector3 enemy_chase[61];

	//�G�̑��x�ɍ��킹�����W�ړ������邽�߂̌v�Z�p�ϐ�
	DirectX::SimpleMath::Vector3 enemy_pos_work;

	//�����Ă��邩�̃t���O
	bool alive_flg_enemy;

public:
	Enemy();
	~Enemy();

	/*virtual void Init();
	virtual void Update();*/

	void Init()override;
	void Update()override;
	void Draw(Camera* cam)override;
	void Uninit()override;

	//�ޏ���ǂ������邽�߂̊֐�
	void Enemy_move();

	//�G��ǉ�����֐�
	void Add_Enemy();	
};