#pragma once
#include "Scene.h"
#include "Object.h"
#include <vector>

class Texture2D;

class StartScene : public Scene
{
private:
    std::vector<Object*> m_MySceneObjects;
   
    Texture2D* m_Title = nullptr;
    Texture2D* m_Title_hurigana = nullptr;
    Texture2D* m_PressEnterwakuImg = nullptr;
    Texture2D* m_PressEnterImg = nullptr;

    float m_curWakuScale = 500.0f;

    bool IsMouseOver(Texture2D* obj);
public:
    StartScene();
    ~StartScene();

    void Init() override;
    void Update() override;
    void Uninit() override;
};