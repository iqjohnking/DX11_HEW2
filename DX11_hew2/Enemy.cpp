#include "Enemy.h"
#include "Game.h"
#include "Shrinemaiden.h"
using namespace DirectX::SimpleMath;

//�t���[���J�E���g��p�ӂ��āA�G�̑��x�ɍ��킹�Ă��̊֐������s����̂����肩��
//�G�̑��x���ޏ���1/3�Ȃ�A3�t���[����1����s����c�Ȃ� (�����ƓG�̓������J�N�J�N�ɂȂ肻���H)

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::Init()
{
	//�G�̃X�s�[�h�̏�����
	enemy_speed = Vector3(0.3f,0.3f,0.0f);

	//�ޏ���ǂ������邽�߂̍��W�z��
	DirectX::SimpleMath::Vector3 enemy_chase[61];

	//�G�̑��x�ɍ��킹�����W�ړ������邽�߂̌v�Z�p�ϐ�
	enemy_pos_work = Vector3(0.0f, 0.3f, 0.0f);

	//�����Ă��邩�̃t���O�̏�����
	alive_flg_enemy = true;
}

void Enemy::Update()
{
	auto shrinePositions = Game::GetInstance()->GetPositions<Shrinemaiden>();

	// �ޏ������݂��邩�`�F�b�N
	if (!shrinePositions.empty())
	{
		// 1�̂������Ȃ��O��Ȃ̂� [0]
		enemy_pos_work = shrinePositions[0];

		// ��F�ޏ��Ɍ����������x�N�g��
		DirectX::SimpleMath::Vector3 dir = enemy_pos_work - GetPosition();
	}

	//�ړ��J�n
	Enemy_move();
}

void Enemy::Enemy_move()
{	
	// �����̌��݈ʒu
	Vector3 pos = GetPosition();

	// �ޏ��̈ʒu
	Vector3 target = enemy_pos_work;

	// �ޏ������x�N�g��
	Vector3 dir = target - pos;

	// ����
	float dist = dir.Length();

	//// �߂������牽�����Ȃ��i�U���h�~�j
	//if (dist < 0.01f)
	//	return;

	// ���������ɂ���i����1�j
	dir.Normalize();

	//���g�� speed ���|���Ė��t���[���ړ�
	pos += dir * enemy_speed;

	// �ʒu�X�V
	SetPosition(pos);
	//for (int i = 60; i > 0; i--)  //���W�z��̒l�̈ړ�
	//{
	//	enemy_chase[i] = enemy_chase[i - 1];
	//}	

	////�ޏ��̈ʒu��G�̍��W�z��ɑ��
	////���ޏ��̃N���X������ěޏ��̍��W���Q�Ƃł���悤�ɂȂ�����R�����g�O��
	////enemy_chase[0] = player_pos;

	////�����x���ޏ���1/3�̓G�Ȃ�	
	////enemy_pos_work = enemy_chase[60] - enemy_chase[59];
	////enemy_pos_work = enemy_pos_work / 3;
	////enemy_pos += enemy_pos_work;
	////�ړ��ʎZ�o���ړ��ʂ�1/3�Ɂ����݂̓G���W�Ɉړ��ʂ𑫂��Ĉړ������A�Ƃ������Ƃ����Ă���

	////���ޏ��Ɠ������x�̓G�̏ꍇ�̈ړ�
	////���݂̓G�̍��W���X�V
	//enemy_pos = enemy_chase[60];
}

void Enemy::Draw(Camera* cam)
{
	//���̂Ƃ���Ȃ�
}

void Enemy::Uninit() 
{
	//���̂Ƃ���Ȃ�
}