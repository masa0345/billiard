#pragma once

#include <cmath>
#include <limits>
#include "range.h"

// 3次元ベクトル
template <typename T>
struct Vector3
{
	T x, y, z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
	template <typename U> Vector3(const Vector3<U>& v) {
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
	}

	bool IsZero() const {
		return x == 0 && y == 0 && z == 0;
	}
	// 長さ
	T LengthSq() const {
		return Dot(*this);
	}
	T Length() const {
		return std::sqrt(LengthSq());
	}
	// 内積
	T Dot(const Vector3<T>& v) const {
		return x*v.x + y*v.y + z*v.z;
	}
	// 外積
	const Vector3<T> Cross(const Vector3<T>& v) const {
		return Vector3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	// 正規化されたベクトルを返す
	const Vector3<T> Normalize() const {
		if (IsZero()) return Vector3<T>();
		return *this / Length();
	}
	
	// 演算子
	bool operator==(const Vector3<T>& rhs) const {
		T eps = std::numeric_limits<T>::epsilon()*10;
		Range<T> range(-eps, eps);
		return range.WithinEq(x-rhs.x) && 
				range.WithinEq(y-rhs.y) && 
				range.WithinEq(z-rhs.z);
	}
	bool operator!=(const Vector3<T>& rhs) const {
		return !(*this == rhs);
	}
	const Vector3<T> operator+(const Vector3<T>& rhs) const {
		return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	const Vector3<T> operator-(const Vector3<T>& rhs) const {
		return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	const Vector3<T> operator*(T t) const {
		return Vector3<T>(x * t, y * t, z * t);
	}
	const Vector3<T> operator/(T t) const {
		return Vector3<T>(x / t, y / t, z / t);
	}

	Vector3<T>& operator+=(const Vector3<T>& rhs) {
		x += rhs.x; y += rhs.y; z += rhs.z;
		return *this;
	}
	Vector3<T>& operator-=(const Vector3<T>& rhs) {
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
		return *this;
	}
	Vector3<T>& operator*=(T t) {
		x *= t; y *= t; z *= t;
		return *this;
	}
	Vector3<T>& operator/=(T t) {
		x /= t; y /= t; z /= t;
		return *this;
	}

	const Vector3<T> operator-() const {
		return Vector3<T>(-x, -y, -z);
	}
};

template<typename T>
const Vector3<T> operator*(T t, const Vector3<T>& rhs) {
	return rhs * t;
}

typedef Vector3<float> vec3f;


const float PI = 3.14152965f;
const float PI2 = PI * 2.f;