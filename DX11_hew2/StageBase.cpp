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

	case CUTTER:
		for (int i = 0; i < 10; ++i)
		{
			if (Enemy2List[i] != nullptr)
				continue;
			Enemy2List[i] = Game::GetInstance()->AddObject<Enemy2>();
			Enemy2List[i]->SetTarget(m_Miko);
			Enemy2List[i]->SetField(m_Field);
			Enemy2List[i]->SetPosition(pos);
			m_MySceneObjects.emplace_back(Enemy2List[i]);
			break;
		}
		break;

	case MAYU:
		for (int i = 0; i < 10; ++i)
		{
			if (Enemy3List[i] != nullptr)
				continue;
			Enemy3List[i] = Game::GetInstance()->AddObject<Enemy3>();
			Enemy3List[i]->SetTarget(m_Miko);
			Enemy3List[i]->SetField(m_Field);
			Enemy3List[i]->SetPosition(pos);
			m_MySceneObjects.emplace_back(Enemy3List[i]);
			break;
		}
		break;
		
	case TACKLE:
		for (int i = 0; i < 10; ++i)
		{
			if (Enemy4List[i] != nullptr)
				continue;
			Enemy4List[i] = Game::GetInstance()->AddObject<Enemy4>();
			Enemy4List[i]->SetTarget(m_Miko);
			Enemy4List[i]->SetField(m_Field);
			Enemy4List[i]->SetPosition(pos);
			m_MySceneObjects.emplace_back(Enemy4List[i]);
			break;
		}  
		break;

	}	
}