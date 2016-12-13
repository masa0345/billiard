#pragma once

#include "vector3.h"

// 四元数
class Quaternion 
{
	friend class Matrix4x3;
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const vec3f& axis, float angle);
	explicit Quaternion(const vec3f& euler);

	// X->Y->Z回転のオイラー角に変換
	//vec3f ToEuler() const;
	// 内積
	float Dot(const Quaternion& q) const;
	// 逆数
	const Quaternion Inverse() const;

	// 外積
	const Quaternion operator*(const Quaternion& rhs) const;
	Quaternion& operator*=(const Quaternion& rhs);

	// 球面線形補間
	static const Quaternion Slerp(const Quaternion& from, const Quaternion& to, float time);

private:
	vec3f v;
	float w;
};