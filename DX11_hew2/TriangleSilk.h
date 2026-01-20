#pragma once
#include <vector>
#include <SimpleMath.h>
#include "silkWall.h"

// 3本の silkWall から三角形頂点を求めるユーティリティ
// XZ 平面で線分交差を用いて三角形 ABC を構築。成功で true。
namespace TriangleSilk
{
    using DirectX::SimpleMath::Vector3;

    // m_SilkWalls の中からインデックス i1,i2,i3 を使って三角形を試行
    bool TryMakeTriangleFromWallsXY(const silkWall* w1,
        const silkWall* w2,
        const silkWall* w3,
        Vector3& A, Vector3& B, Vector3& C);

    // 点が三角形内部か（XY）
    bool IsInsideTriangleXY(const Vector3& p,
        const Vector3& A,
        const Vector3& B,
        const Vector3& C);

    // Scene 側で m_SilkWalls から組み合わせを走査し、最初に見つかった三角形を返す
    // 戻り値: 見つかったら true。A,B,C に三角形頂点を格納。
    //bool FindFirstTriangleXY(const std::vector<silkWall*>& walls,
    //    Vector3& A, Vector3& B, Vector3& C);
}
