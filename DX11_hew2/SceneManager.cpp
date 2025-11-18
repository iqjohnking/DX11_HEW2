#include "SceneManager.h"
#include "GameScene.h"
#include <iostream>

SceneManager::SceneManager() :scene(MENU) ,currentScene(nullptr) {}

//SceneManager::SceneManager()
//{
//	scene = MENU;
//}

SceneManager::~SceneManager()
{
}

SceneManager& SceneManager::GetInstance() {
	static SceneManager instance;
	return instance;
}

void SceneManager::Init()
{
    // ˆêŠJŽnAi“ü GAMEiˆ½æ MENUj
    ChangeScene(GAME);
}

void SceneManager::ChangeScene(Scenes nextScene)
{
    // æ”cäp“I scene ´{
    if (currentScene) {
        currentScene->Uninit();
        delete currentScene;
        currentScene = nullptr;
    }

    scene = nextScene;

    switch (scene)
    {
    case MENU:
        std::cout << "TODO: new MenuScene();" << std::endl;
        currentScene = nullptr;
        break;

    case GAME:
        currentScene = new GameScene();
        break;

    case RESULT:
        std::cout << "TODO: new ResultScene();" << std::endl;
        currentScene = nullptr;
        break;

    case QUIT:
        currentScene = nullptr;
        break;
    }

    if (currentScene) {
        currentScene->Init();
    }
}

void SceneManager::Update()
{
    if (currentScene) {
        currentScene->Update();
    }
}

void SceneManager::Draw()
{
    if (currentScene) {
        currentScene->Draw();
    }
}

void SceneManager::Uninit()
{
    if (currentScene) {
        currentScene->Uninit();
        delete currentScene;
        currentScene = nullptr;
    }
}