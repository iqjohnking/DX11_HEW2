#include "TitleBG.h"
#include "Game.h"

using namespace DirectX::SimpleMath;

void TitleBG::Init()
{
	// 親クラスの初期化処理を呼び出す
	Texture2D::Init();
	// テクスチャを指定
	SceneName currentScene = Game::GetInstance()->GetScenebg();

	switch (currentScene) 
	{
	case SceneName::START:
		{
			SetTexture("assets/texture/Message/bg/background_000.jpg");
			break;
		}
		case SceneName::TITLE:
		{
			SetTexture("assets/texture/Message/bg/background_000.jpg");
			break;
		}
		case SceneName::STAGE0:
		{
			SetTexture("assets/texture/Message/bg/background_003.jpg");
			break;
		}
		case SceneName::MODE_SELECT:
		{
			SetTexture("assets/texture/Message/bg/background_000.jpg");
			break;
		}
		case SceneName::STAGE_SELECT:
		{
			SetTexture("assets/texture/Message/bg/background_000.jpg");
			break;
		}
		case SceneName::STAGE1:
		{
			SetTexture("assets/texture/Message/bg/background_000.jpg");
			break;
		}
		case SceneName::STAGE2:
		{
			SetTexture("assets/texture/Message/bg/background_000.jpg");
			break;
		}
		case SceneName::STAGE3:
		{
			SetTexture("assets/texture/Message/bg/background_000.jpg");
			break;
		}
		case SceneName::STAGE4:
		{
			SetTexture("assets/texture/Message/bg/background_001.jpg");
			break;
		}
		case SceneName::STAGE5:
		{
			SetTexture("assets/texture/Message/bg/background_001.jpg");
			break;
		}
		case SceneName::STAGE6:
		{
			SetTexture("assets/texture/Message/bg/background_002.jpg");
			break;
		}
		case SceneName::STAGE7:
		{
			SetTexture("assets/texture/Message/bg/background_003.jpg");
			break;
		}
		case SceneName::STAGE8:
		{
			SetTexture("assets/texture/Message/bg/background_004.jpg");
			break;
		}
		case SceneName::STAGE9:
		{
			SetTexture("assets/texture/Message/bg/background_005.jpg");
			break;
		}
		case SceneName::STAGE10:
		{
			SetTexture("assets/texture/Message/bg/background_003.jpg");
			break;
		}
		case SceneName::GAMEOVER:
		{
			SetTexture("assets/texture/Message/bg/background_003.jpg");
			break;
		}
		default:
		{
			SetTexture("assets/texture/Message/bg/background_003.jpg");
			break;
		}
	}


	// 位置を指定
	SetPosition(0.0f, 0.0f, 0.0f);
	// 角度を指定
	SetRotation(0.0f, 0.0f, 0.0f);
	// 大きさを指定
	SetScale(1920, 1080, 0.0f);

	SetDrawOrder(0);
}


void TitleBG::Update()
{
	//Vector3 oldPos = m_Position;
	//oldPos.x += 0.1f;
	//m_Position = oldPos;
}