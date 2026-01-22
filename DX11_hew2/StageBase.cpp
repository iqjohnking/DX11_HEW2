#include "StageBase.h"
#include"Game.h"

void StageBase::EnemySpawn(EnemyType enemyType, DirectX::SimpleMath::Vector3 pos)
{
	switch (enemyType)
	{
	case NORMAL:
		for (int i = 0; i < 60; i++)
		{
			if (Enemy1List[i] != nullptr)
				continue;
			Enemy1List[i] = Game::GetInstance()->AddObject<Enemy1>();
			Enemy1List[i]->SetTarget(m_Miko);
			Enemy1List[i]->SetField(m_Field);
			Enemy1List[i]->SetPosition(pos);
			m_MySceneObjects.emplace_back(Enemy1List[i]);
			break;
		}
		break;
	}
}