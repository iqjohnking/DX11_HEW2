#include "TriangleSilk.h"
#include <cmath>
#include <algorithm>

using DirectX::SimpleMath::Vector3;

//-----------------------------------------------------------------------------
// 内部関数定義
//-----------------------------------------------------------------------------

// XY平面上での2Dクロス積と向き判定
static float Cross2DXY(const Vector3& a, const Vector3& b) { return a.x * b.y - a.y * b.x; } 
// XY平面上での3点の向き判定
static float Orient2D(const Vector3& a, const Vector3& b, const Vector3& p) { return Cross2DXY(b - a, p - a); }
// 戻り値の符号の意味：
//  > 0 : b は a に対して反時計回り（左側）
//  < 0 : b は a に対して時計回り（右側）
//  = 0 : 共線（同一直線上／平行）
//=====================================================

// XZ平面上で線分同士の交差判定。交差点を outI に格納。交差すれば true。
static bool SegmentIntersectXY(const Collision::Segment& s1,
								const Collision::Segment& s2,
								Vector3& outI)
{
	Vector3 p = s1.start;                 // 線分1の始点 p
	Vector3 r = s1.end - s1.start;        // 線分1の方向ベクトル r

	Vector3 q = s2.start;                 // 線分2の始点 q
	Vector3 s = s2.end - s2.start;        // 線分2の方向ベクトル s

	// r×s（2D: XY のクロス積の z 成分相当）
	float rxs = Cross2DXY(r, s);

	// (q - p)×r（u 計算で使う）
	float qmpxr = Cross2DXY(q - p, r);

	// 平行判定（ここでは共線の重なりは「交差なし」として false）
	if (std::abs(rxs) < 1e-6f) return false;

	// 交点パラメータ t, u を求める
	// t = ((q - p)×s) / (r×s)
	// u = ((q - p)×r) / (r×s)
	float t = Cross2DXY(q - p, s) / rxs;
	float u = qmpxr / rxs;

	// 0<=t<=1 かつ 0<=u<=1 のとき、両方「線分」上で交差、それ以外は交差しない
	if (t < 0.0f || t > 1.0f || u < 0.0f || u > 1.0f) return false;

	// XY 上の交点（z は一旦無視して計算）
	outI = p + r * t;

	// z は線分1の t に合わせて線形補間して設定（XY交点に対応する高さ）
	outI.z = s1.start.z + (s1.end.z - s1.start.z) * t;

	return true;
}



//-----------------------------------------------------------------------------
// 名前空間外関数定義
//-----------------------------------------------------------------------------

// 三本の silkWall から三角形頂点を試行的に作成。成功で true。
bool TriangleSilk::TryMakeTriangleFromWallsXY(const silkWall* w1,
											  const silkWall* w2,
											  const silkWall* w3,
											  Vector3& A, Vector3& B, Vector3& C)
{
	if (!w1 || !w2 || !w3) return false;

	const auto& s1 = w1->GetSegment();
	const auto& s2 = w2->GetSegment();
	const auto& s3 = w3->GetSegment();

	Vector3 i12, i23, i31;
	bool ok12 = SegmentIntersectXY(s1, s2, i12);
	bool ok23 = SegmentIntersectXY(s2, s3, i23);
	bool ok31 = SegmentIntersectXY(s3, s1, i31);
	if (!(ok12 && ok23 && ok31)) return false;

	float area2 = std::abs(Cross2DXY(i23 - i12, i31 - i12));
	if (area2 < 1e-6f) return false;

	A = i12; B = i23; C = i31;
	if (Orient2D(A, B, C) < 0.0f) std::swap(B, C);
	return true;
}

// XZ平面上で点 p が三角形 ABC の内部にあるか判定。内部なら true。
bool TriangleSilk::IsInsideTriangleXY(const Vector3& p,
									  const Vector3& A,
									  const Vector3& B,
									  const Vector3& C)
{
	Vector3 v0 = C - A;
	Vector3 v1 = B - A;
	Vector3 v2 = p - A;

	float dot00 = v0.x * v0.x + v0.y * v0.y;
	float dot01 = v0.x * v1.x + v0.y * v1.y;
	float dot02 = v0.x * v2.x + v0.y * v2.y;
	float dot11 = v1.x * v1.x + v1.y * v1.y;
	float dot12 = v1.x * v2.x + v1.y * v2.y;

	float denom = (dot00 * dot11 - dot01 * dot01);
	if (std::abs(denom) < 1e-6f) return false;

	float invDenom = 1.0f / denom;
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}

// silkWall 群から最初に見つかった三角形頂点を作成。成功で true。
//bool TriangleSilk::FindFirstTriangleXY(const std::vector<silkWall*>& walls,
//    Vector3& A, Vector3& B, Vector3& C)
//{
//    const size_t n = walls.size();
//    if (n < 3) return false;
//    for (size_t i = 0; i < n; ++i) {
//        for (size_t j = i + 1; j < n; ++j) {
//            for (size_t k = j + 1; k < n; ++k) {
//                if (TryMakeTriangleFromWallsXY(walls[i], walls[j], walls[k], A, B, C)) {
//                    return true;
//                }
//            }
//        }
//    }
//    return false;
//}