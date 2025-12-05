//================================
// Collision.cpp　v1.0
//================================
#include "Collision.h"
#include <algorithm>

using namespace DirectX::SimpleMath;

namespace Collision
{
	//==================================
	// ■CheckHit関数
	// 線(無限の長さ)と平面(無限の大きさ)の当たり判定
	//==================================
	bool CheckHit(const Line& line, const Plane& plane)
	{
		//無限の線が無限の平面にぶつからないのは線と平面が平行な時のみ。
		//線の方向ベクトルと平面の法線ベクトルが垂直(→内積が0)かどうかをチェックする
		if (Dot((plane.point - line.point), plane.normal) == 0 || Dot(line.vec, plane.normal) != 0)
		{
			return true;
		}
		return false;
	}

	//==================================
	// ■CheckHit関数
	// 線分と平面(無限の大きさ)の当たり判定
	//==================================
	bool CheckHit(const Segment& segment, const Plane& plane)
	{
		if (Dot((segment.start - plane.point), plane.normal) * Dot((segment.end - plane.point), plane.normal) <= 0)
		{
			return true;
		}
		return false;
	}

	//==================================
	// ■CheckHit関数
	// 線(無限の長さ)とポリゴンの当たり判定
	//==================================
	bool CheckHit(const Line& line, const Polygon& polygon)
	{
		Vector3 p;
		return CheckHit(line, polygon, p);
	}
	bool CheckHit(const Line& line, const Polygon& polygon, DirectX::SimpleMath::Vector3& contact)
	{
		// 三角形の法線を計算
		Vector3 normal = GetNormal(polygon);

		// 線と法線が平行である場合
		float denom = Dot(normal, line.vec);
		if (fabs(denom) < 1e-6f) {
			return false; // 交差なし
		}

		// 線上の交点を計算
		float d = Dot(normal, polygon.p0);
		float t = (d - Dot(normal, line.point)) / denom;
		contact = line.point + t * line.vec;

		// 三角形内に交点があるかを判定
		return PointInTriangle(contact, polygon);
	}

	//==================================
	// ■CheckHit関数
	// 線分とポリゴンの当たり判定
	//==================================
	bool CheckHit(const Segment& segment, const Polygon& polygon)
	{
		Vector3 p;
		return CheckHit(segment, polygon, p);
	}
	bool CheckHit(const Segment& segment, const Polygon& polygon, Vector3& contact)
	{
		Plane plane(polygon.p0, GetNormal(polygon));

		if (CheckHit(segment, plane))
		{
			// 線分と平面の交点位置を計算
			float denom = plane.normal.Dot(segment.end - segment.start);
			float t = plane.normal.Dot(plane.point - segment.start) / denom;

			// tが0から1の範囲内であれば線分と平面が交差する
			if (t >= 0.0f && t <= 1.0f) {
				contact = segment.start + t * (segment.end - segment.start); // 交点を計算

				//交点が三角形ポリゴンの内側にあるか判定
				return PointInTriangle(contact, polygon);
			}
		}
		return false;

	}

	//==================================
	// ■CheckHit関数
	// 球体と平面の当たり判定
	//==================================
	bool CheckHit(const Sphere& sphere, const Plane& plane)
	{
		// 球体の中心から平面への距離を計算
		float distance = DistancePointToPlane(sphere.center, plane);

		// 球体の半径以内なら衝突
		return (distance <= sphere.radius);
	}

	//==================================
	// ■CheckHit関数
	// 球体とポリゴンの当たり判定
	//==================================
	bool CheckHit(const Sphere& sphere, const Polygon& polygon)
	{
		Vector3 p;
		return CheckHit(sphere, polygon, p);
	}
	bool CheckHit(const Sphere& sphere, const Polygon& polygon, Vector3& contact)
	{
		Plane plane(polygon.p0, GetNormal(polygon));

		// 球体の中心から平面への距離を計算
		float distance = DistancePointToPlane(sphere.center, plane);

		// 球体の半径以内なら
		if (distance <= sphere.radius)
		{
			//線分と平面の交点を計算
			contact = ProjectPointToPlane(sphere.center, plane);

			//交点が三角形ポリゴンの内側にあるか判定
			if (PointInTriangle(contact, polygon))
			{
				return true;
			}

			//交点が三角形ポリゴンの外側なら各辺との距離を計算
			if (DistancePointToSegment(sphere.center, { polygon.p0 , polygon.p1 }, contact) <= sphere.radius)
			{
				return true;
			}

			if (DistancePointToSegment(sphere.center, { polygon.p1 , polygon.p2 }, contact) <= sphere.radius)
			{
				return true;
			}

			if (DistancePointToSegment(sphere.center, { polygon.p2 , polygon.p0 }, contact) <= sphere.radius)
			{
				return true;
			}
		}

		return false;
	}

	//==================================
	// ■CheckHit関数
	// 球体と球体の当たり判定
	//==================================
	bool CheckHit(Sphere sphere1, Sphere sphere2)
	{
		Vector3 p;
		return CheckHit(sphere1, sphere2, p);
	}
	bool CheckHit(Sphere sphere1, Sphere sphere2, Vector3& contact)
	{
		float len2 = (sphere1.center - sphere2.center).LengthSquared();
		float r2 = (sphere1.radius + sphere2.radius) * (sphere1.radius + sphere2.radius);
		if (r2 > len2) {

			// 接触点を計算 ※sphere1がsphere2に後からぶつかって来たとしている
			Vector3 v = (sphere1.center - sphere2.center);
			v.Normalize();
			contact = sphere2.center + v * sphere2.radius;

			return true;
		}
		return false;
	}

	//==================================
	// ■CheckHit関数
	// AABBとAABBの当たり判定
	//==================================
	bool CheckHit(AABB p1, AABB p2) {

		// X座標
		if (p1.max.x < p2.min.x) {
			return false;
		}

		if (p1.min.x > p2.max.x) {
			return false;
		}

		// Y座標
		if (p1.max.y < p2.min.y) {
			return false;
		}

		if (p1.min.y > p2.max.y) {
			return false;
		}

		// Z座標
		if (p1.max.z < p2.min.z) {
			return false;
		}

		if (p1.min.z > p2.max.z) {
			return false;
		}

		return true;
	}

	//=========================================================================================
	//=========================================================================================
	//=========================================================================================
	//=========================================================================================
	//=========================================================================================



	//==================================
	// ■Dot関数
	// ベクトルの内積を求める
	//==================================
	float Dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	//==================================
	// ■Cross関数
	// ベクトルの外積を求める
	//==================================
	Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(
			v1.y * v2.z - v1.z * v2.y,	// x成分
			v1.z * v2.x - v1.x * v2.z,	// y成分
			v1.x * v2.y - v1.y * v2.x);	// z成分
	}

	//==================================
	// ■ClosestPointOnSegment関数
	// 点から線分までの最近地点
	//==================================
	Vector3 ClosestPointOnSegment(const Vector3& point, const Segment& segment)
	{
		// 線分のベクトルを計算（終点 - 始点）
		Vector3 vec = segment.end - segment.start;

		// 線分の長さの平方を計算（ベクトルの大きさの2乗）
		double r2 = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;

		// 点から線分の始点までのベクトルとの内積を計算
		double tt = -Dot(vec, (segment.start - point));

		// 点が始点よりも線分の外側
		if (tt < 0)
		{
			// 点と線分の始点の距離を返す
			return segment.start;
		}
		// 点が終点よりも線分の外側の場合
		else if (tt > r2)
		{
			// 点と線分の終点との距離を返す
			return segment.end;
		}
		// 点が線分の上に投影される場合、線分上の最短距離を計算
		else
		{
			Vector3 ab = segment.end - segment.start; // 線分のベクトル
			float lengthSq = ab.LengthSquared(); // 線分の長さの二乗

			// 線分が退化（両端が同じ点）している場合
			if (lengthSq == 0.0f) {
				return segment.start; // 線分の端点を返す
			}

			// 点が線分上に投影される位置を計算（0 <= t <= 1 の範囲）
			float t = (point - segment.start).Dot(ab) / lengthSq;
			t = std::clamp(t, 0.0f, 1.0f);

			// 線分上の最近接点を返す
			return (segment.start + t * ab);
		}
	}

	//==================================
	// ■DistanceSquaredPointToSegment関数
	// 点から線分までの距離の2乗
	//==================================
	float DistanceSquaredPointToSegment(const Vector3& point, const Segment& segment)
	{
		Vector3 p;
		return DistanceSquaredPointToSegment(point, segment, p);
	}
	float DistanceSquaredPointToSegment(const Vector3& point, const Segment& segment, Vector3& contact)
	{
		contact = ClosestPointOnSegment(point, segment);

		return (point - contact).LengthSquared();
	}

	//==================================
	// ■DistancePointToSegment関数
	// 点から線分までの距離
	//==================================
	float DistancePointToSegment(const Vector3& point, const Segment& segment)
	{
		Vector3 p;
		return DistancePointToSegment(point, segment, p);
	}
	float DistancePointToSegment(const Vector3& point, const Segment& segment, Vector3& contact)
	{
		contact = ClosestPointOnSegment(point, segment);

		return (point - contact).Length();
	}

	//==================================
	// ■DistancePointToPlane関数
	// 点から平面までの距離
	//==================================
	float DistancePointToPlane(const Vector3& point, const Plane& plane)
	{
		return fabs(Dot((point - plane.point), plane.normal) / Dot(plane.normal, plane.normal));
	}

	//==================================
	// ■ProjectPointToPlane関数
	// 点から平面に下ろした垂線の交点
	//==================================
	Vector3 ProjectPointToPlane(const Vector3& point, const Plane& plane)
	{
		double t = -Dot((point - plane.point), plane.normal) / Dot(plane.normal, plane.normal);

		// 交点の座標を計算
		return  point + (plane.normal * (float)t);
	}

	//==================================
	// ■PointInTriangle関数
	// 三角形の内部かどうかを判定する
	//==================================
	bool PointInTriangle(const Vector3& point, const Polygon& polygon)
	{
		// 3辺のベクトル
		Vector3 ab = polygon.p1 - polygon.p0;
		Vector3 bc = polygon.p2 - polygon.p1;
		Vector3 ca = polygon.p0 - polygon.p2;

		// 内部の点とのベクトル
		Vector3 ap = point - polygon.p0;
		Vector3 bp = point - polygon.p1;
		Vector3 cp = point - polygon.p2;

		// 3辺と内部の点との法線ベクトル
		Vector3	n1 = Cross(ab, ap);
		Vector3	n2 = Cross(bc, bp);
		Vector3	n3 = Cross(ca, cp);

		// 三角形の法線ベクトル
		Vector3	normal = Cross(ab, bc);

		float dot = n1.Dot(normal);
		if (dot < 0) return false; // 為す角度が鈍角

		dot = n2.Dot(normal);
		if (dot < 0) return false; // 為す角度が鈍角

		dot = n3.Dot(normal);
		if (dot < 0) return false; // 為す角度が鈍角

		return true;
	}

	//==================================
	// ■ClosestPointOnTriangle関数
	// 点と三角形間の最近接点を求める
	//==================================
	Vector3 ClosestPointOnTriangle(const Vector3& point, const Polygon& polygon)
	{
		Plane plane(polygon.p0, GetNormal(polygon));

		//線分と平面の交点を計算
		Vector3 p = ProjectPointToPlane(point, plane);

		//交点が三角形ポリゴンの内側にあるか判定
		if (PointInTriangle(p, polygon))
		{
			return p;
		}

		//交点が三角形ポリゴンの外側なら各辺との距離を計算
		Vector3 p1, p2, p3;
		float d1 = DistanceSquaredPointToSegment(point, { polygon.p0 , polygon.p1 }, p1);
		float d2 = DistanceSquaredPointToSegment(point, { polygon.p1 , polygon.p2 }, p2);
		float d3 = DistanceSquaredPointToSegment(point, { polygon.p2 , polygon.p0 }, p3);

		if (d1 < d2)
		{
			if (d1 < d3)
			{
				return p1;
			}
			else
			{
				return p3;
			}
		}
		else
		{
			if (d2 < d3)
			{
				return p2;
			}
			else
			{
				return p3;
			}
		}
	}

	//==================================
	// 法線を計算
	//==================================
	Vector3 GetNormal(const Polygon& polygon)
	{
		Vector3 n = Cross((polygon.p1 - polygon.p0), (polygon.p2 - polygon.p0));
		n.Normalize();
		return n;
	}

	//==================================
	// 球体を当たった後の地点に動かす
	//==================================
	Vector3 moveSphere(const Segment& segment, const float& radius, const Polygon& polygon, const Vector3& contact, float& distance)
	{
		// 線分の方向ベクトル
		Vector3 direction = segment.end - segment.start;
		float length = direction.Length();
		direction.Normalize();

		// 線分の長さが0の場合は計算不能
		if (length == 0.0f) return segment.start;

		// 二次方程式を計算する (a*t^2 + b*t + c = 0)
		float a = 1.0f; // directionは正規化されているのでa=1
		float b = 2.0f * Collision::Dot((segment.start - contact), direction);
		float c = (segment.start - contact).LengthSquared() - radius * radius;
		float discriminant = b * b - 4.0f * a * c;

		// 解があれば
		if (discriminant >= 0.0f) {

			// 解を求める
			float sqrt_discriminant = std::sqrt(discriminant);
			float t1 = (-b + sqrt_discriminant) / (2.0f * a);
			float t2 = (-b - sqrt_discriminant) / (2.0f * a);

			// 線分上の解を判定
			if (t1 < t2)
			{
				distance = t1;
				return  segment.start + t1 * direction;;
			}
			else
			{
				distance = t2;
				return  segment.start + t2 * direction;
			}
		}

		return segment.start;
	}
	Vector3 moveSphere(const Sphere& sphere, const Polygon& polygon, const Vector3& contact)
	{
		//Vector3 normal =  GetNormal(polygon);

		Vector3 v = sphere.center - contact;
		v.Normalize();

		return contact + v * sphere.radius;
	}

	//==================================
	// AABBを設定
	//==================================
	AABB SetAABB(Vector3 centerposition, float width, float height, float depth)
	{
		AABB aabb{};

		width = fabs(width);
		height = fabs(height);
		depth = fabs(depth);

		aabb.min.x = centerposition.x - width / 2.0f;
		aabb.min.y = centerposition.y - height / 2.0f;
		aabb.min.z = centerposition.z - depth / 2.0f;

		aabb.max.x = centerposition.x + width / 2.0f;
		aabb.max.y = centerposition.y + height / 2.0f;
		aabb.max.z = centerposition.z + depth / 2.0f;

		return aabb;
	}

}