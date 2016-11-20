#pragma once

#include <cmath>

template <typename T>
struct Vector2
{
	T x, y;

	Vector2() : x(0), y(0) {}
	Vector2(T x, T y) : x(x), y(y) {}
	template <typename U> Vector2(const Vector2<U>& v) {
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
	}

	// ����
	T LengthSq() const {
		return Dot(*this);
	}
	T Length() const {
		return static_cast<T>(std::sqrtf(LengthSq()));
	}
	// ����
	T Dot(const Vector2<T>& v) const {
		return x*v.x + y*v.y;
	}
	// �O��
	const Vector2<T> Cross(const Vector2<T>& v) const {
		return Vector2<T>(x*v.y - y*v.x);
	}
	// ���K�����ꂽ�x�N�g����Ԃ�
	const Vector2<T> Normalize() const {
		if (x == 0 && y == 0) return Vector2<T>();
		return *this / Length();
	}

	const Vector2<T> operator+(const Vector2<T>& rhs) const {
		return Vector2<T>(x + rhs.x, y + rhs.y);
	}
	const Vector2<T> operator-(const Vector2<T>& rhs) const {
		return Vector2<T>(x - rhs.x, y - rhs.y);
	}
	const Vector2<T> operator*(T t) const {
		return Vector2<T>(x * t, y * t);
	}
	const Vector2<T> operator/(T t) const {
		return Vector2<T>(x / t, y / t);
	}

	Vector2<T>& operator+=(const Vector2<T>& rhs) {
		x += rhs.x; y += rhs.y;
		return *this;
	}
	Vector2<T>& operator-=(const Vector2<T>& rhs) {
		x -= rhs.x; y -= rhs.y;
		return *this;
	}
	Vector2<T>& operator*=(T t) {
		x *= t; y *= t;
		return *this;
	}
	Vector2<T>& operator/=(T t) {
		x /= t; y /= t;
		return *this;
	}

	const Vector2<T> operator-() const {
		return Vector2<T>(-x, -y);
	}
};

template<typename T>
const Vector2<T> operator*(T t, const Vector2<T>& rhs) {
	return rhs * t;
}

typedef Vector2<float> vec2f;