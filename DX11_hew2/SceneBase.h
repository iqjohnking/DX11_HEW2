#pragma once
#include "Renderer.h"

class SceneBase {
public:
    virtual ~SceneBase() {}

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Uninit() = 0;
};