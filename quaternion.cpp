#include "quaternion.h"
#include <cmath>
#include <cassert>

Quaternion::Quaternion() : w(1.f)
{
}

Quaternion::Quaternion(float x, float y, float z, float w) :
	w(w), v(x, y, z)
{
}

Quaternion::Quaternion(const vec3f& axis, float angle)
{
	assert(!axis.IsZero());
	angle *= 0.5f;
	w = std::cosf(angle);
	v = axis.Normalize() * std::sinf(angle);
}

vec3f Quaternion::ToEuler() const
{
	/// 間違ってる??

	vec3f result;
	float sh = 2.f * (v.x*v.z - w*v.y);
	if (std::fabsf(sh) > 0.9999f) {
		// ジンバルロック
		result.y = PI * 0.5f * sh;
		result.x = std::atan2f(v.y*v.z - w*v.x, 0.5f - v.x*v.x - v.z*v.z);
		result.z = 0.f;
	} else {
		result.y = std::asinf(sh);
		result.x = std::atan2f(-v.y*v.z - w*v.x, 0.5f - v.x*v.x - v.y*v.y);
		result.z = std::atan2f(-v.x*v.y - w*v.z, 0.5f - v.y*v.y - v.z*v.z);
	}

	return result;
}

float Quaternion::Dot(const Quaternion& q) const
{
	return w*q.w + v.Dot(q.v);
}

const Quaternion Quaternion::Inverse() const
{
	float len = std::sqrtf(w*w + v.LengthSq());
	return Quaternion(-v.x/len, -v.y/len, -v.z/len, w/len);
}

const Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
	Quaternion q;
	q.w = w*rhs.w - v.Dot(rhs.v);
	q.v = w*v + rhs.w*rhs.v + v.Cross(rhs.v);
	return q;
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
	w = w*rhs.w - v.Dot(rhs.v);
	v = w*v + rhs.w*rhs.v + v.Cross(rhs.v);
	return *this;
}

const Quaternion Quaternion::Slerp(const Quaternion& from, const Quaternion& to, float time)
{
	return Quaternion();
}
