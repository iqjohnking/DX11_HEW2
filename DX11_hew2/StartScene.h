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
    Texture2D* m_FadePanel = nullptr; //フェード用

    Texture2D* m_FadeL = nullptr;   // 左の障子
    Texture2D* m_FadeR = nullptr;   // 右の障子
    Texture2D* m_FadeLL = nullptr;   // 左の障子
    Texture2D* m_FadeRR = nullptr;   // 右の障子

    float m_shojiTimer = 0.0f;      // 0.0(全開) ～ 1.0(全閉)

    float m_curWakuScale = 500.0f;
   
    float m_fadeAlpha = 0.0f;    // 0.0(透明) ～ 1.0(真っ黒)
    bool m_isStarting = false;   // 決定ボタンを押したか

    bool m_HasChangedScene = false;//遷移したか,初回決定時にチュートリアルへ飛ばすのに使う

    bool IsMouseOver(Texture2D* obj);
public:
    StartScene();
    ~StartScene();

    void Init() override;
    void Update() override;
    void Uninit() override;
};