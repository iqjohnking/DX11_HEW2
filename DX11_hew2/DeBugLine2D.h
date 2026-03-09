#pragma once
#include <cmath>
#include "Texture2D.h"
#include "Camera.h"

using DirectX::SimpleMath::Vector3;

class DeBugLine2D : public Texture2D
{
public:
    void Init() override {}
    void Update() override {}

    void Draw(Camera* cam) override
    {
        Texture2D::Draw(cam);
    }

    // p0 -> p1 の線分を設定
    void SetLine(const Vector3& p0, const Vector3& p1)
    {
        Vector3 diff = p1 - p0;
        float len = diff.Length();
        if (len <= 0.0001f) return;

        float angle = std::atan2(diff.y, diff.x);

        // 中心位置に配置する
        Vector3 center = (p0 + p1) * 0.5f;
        SetPosition(center);
        SetRotationRad(0.0f, 0.0f, angle);

        // もし Texture2D の実装が「scale.x = 長さ」ならそのまま
        // 「scale.x = 元テクスチャ幅に対する倍率」なら、補正が必要
        SetScale(len, 1.0f, 1.0f);

        // 必要ならピボットも中央に
        // SetPivot(0.5f, 0.5f, 0.0f); // Texture2D の仕様に合わせて
    }
};