#pragma once

#include "vector3.h"
#include <array>

class Quaternion;

// 3次元アフィン変換をするための行列
class Matrix4x3
{
	friend const vec3f operator*(const vec3f& v, const Matrix4x3& m);
	friend const Matrix4x3 operator*(const Matrix4x3& a, const Matrix4x3& b);
	friend vec3f& operator*=(vec3f& v, const Matrix4x3& m);
	friend Matrix4x3& operator*=(Matrix4x3& a, const Matrix4x3& b);
public:
	// 恒等行列
	static Matrix4x3 Identity();
	// 平行移動
	static Matrix4x3 Translation(const vec3f& v);
	// 回転
	static Matrix4x3 Rotate(const vec3f& axis, float theta);
	static Matrix4x3 Rotate(const Quaternion& q);
	static Matrix4x3 RotateX(float angle);
	static Matrix4x3 RotateY(float angle);
	static Matrix4x3 RotateZ(float angle);
	// 拡大
	static Matrix4x3 Scale(const vec3f& s);

	// 行列式
	float Determinant() const;
	// 逆行列
	Matrix4x3 Inverse() const;
	// 要素を配列で取得
	std::array<float, 16> ToArray() const;

private:
	// 外部から直接インスタンス化しない
	Matrix4x3() = default;
	
	float m11, m12, m13;
	float m21, m22, m23;
	float m31, m32, m33;
	float tx, ty, tz;
};

// 乗算
const vec3f operator*(const vec3f& v, const Matrix4x3& m);
const Matrix4x3 operator*(const Matrix4x3& a, const Matrix4x3& b);

vec3f& operator*=(vec3f& v, const Matrix4x3& m);
Matrix4x3& operator*=(Matrix4x3& a, const Matrix4x3& b);