#include "matrix4x3.h"
#include "quaternion.h"
#include <cassert>

Matrix4x3 Matrix4x3::Identity()
{
	Matrix4x3 m;
	m.m11 = m.m22 = m.m33 = 1.f;
	m.m12 = m.m13 = m.m21 = m.m23 = m.m31 = m.m32 = 0.f;
	m.tx = m.ty = m.tz = 0.f;
	return m;
}

Matrix4x3 Matrix4x3::Translation(const vec3f& v)
{
	Matrix4x3 m;
	m.m11 = m.m22 = m.m33 = 1.f;
	m.m12 = m.m13 = m.m21 = m.m23 = m.m31 = m.m32 = 0.f;
	m.tx = v.x;
	m.ty = v.y;
	m.tz = v.z;
	return m;
}

Matrix4x3 Matrix4x3::Rotate(const vec3f& axis, float theta)
{
	assert(std::fabs(axis.LengthSq() - 1.f) < 0.01f);
	float s = std::sin(theta);
	float c = std::cos(theta);
	vec3f a = (1.f - c) * axis;
	Matrix4x3 m;

	m.m11 = a.x*axis.x + c;
	m.m12 = a.y*axis.y + axis.z*s;
	m.m13 = a.z*axis.z - axis.y*s;

	m.m21 = a.y*axis.x - axis.z*s;
	m.m22 = a.y*axis.y + c;
	m.m23 = a.y*axis.z + axis.x*s;

	m.m31 = a.z*axis.x + axis.y*s;
	m.m32 = a.z*axis.y - axis.x*s;
	m.m33 = a.z*axis.z + c;

	m.tx = m.ty = m.tz = 0.f;

	return m;
}

Matrix4x3 Matrix4x3::Rotate(const Quaternion& q)
{
	float ww = 2.f * q.w;
	float xx = 2.f * q.v.x;
	float yy = 2.f * q.v.y;
	float zz = 2.f * q.v.z;
	Matrix4x3 m;

	m.m11 = 1.f - yy*q.v.y - zz*q.v.z;
	m.m12 = xx*q.v.y + ww*q.v.z;
	m.m13 = xx*q.v.z - ww*q.v.y;

	m.m21 = xx*q.v.y - ww*q.v.z;
	m.m22 = 1.f - xx*q.v.x - zz*q.v.z;
	m.m23 = yy*q.v.z + ww*q.v.x;

	m.m31 = xx*q.v.z + ww*q.v.y;
	m.m32 = yy*q.v.z - ww*q.v.x;
	m.m33 = 1.f - xx*q.v.x - yy*q.v.y;

	m.tx = m.ty = m.tz = 0.f;

	return m;
}

Matrix4x3 Matrix4x3::RotateX(float angle)
{
	Matrix4x3 m;
	float s = std::sin(angle), c = std::cos(angle);

	m.m11 = 1.f; m.m12 = 0.f; m.m13 = 0.f;
	m.m21 = 0.f; m.m22 = c;   m.m23 = s;
	m.m31 = 0.f; m.m32 = -s;  m.m33 = c;
	m.tx = m.ty = m.tz = 0.f;

	return m;
}

Matrix4x3 Matrix4x3::RotateY(float angle)
{
	Matrix4x3 m;
	float s = std::sin(angle), c = std::cos(angle);

	m.m11 = c;   m.m12 = 0.f; m.m13 = -s;
	m.m21 = 0.f; m.m22 = 1.f; m.m23 = 0.f;
	m.m31 = s;   m.m32 = 0.f; m.m33 = c;
	m.tx = m.ty = m.tz = 0.f;

	return m;
}

Matrix4x3 Matrix4x3::RotateZ(float angle)
{
	Matrix4x3 m;
	float s = std::sin(angle), c = std::cos(angle);

	m.m11 = c;   m.m12 = s;   m.m13 = 0.f;
	m.m21 = -s;  m.m22 = c;   m.m23 = 0.f;
	m.m31 = 0.f; m.m32 = 0.f; m.m33 = 1.f;
	m.tx = m.ty = m.tz = 0.f;

	return m;
}

Matrix4x3 Matrix4x3::Scale(const vec3f& s)
{
	Matrix4x3 m;
	m.m11 = s.x;
	m.m22 = s.y;
	m.m33 = s.z;
	m.m12 = m.m13 = m.m21 = m.m23 = m.m31 = m.m32 = 0.f;
	m.tx = m.ty = m.tz = 0.f;
	return m;
}

float Matrix4x3::Determinant() const
{
	return m11 * (m22*m33 - m23*m32) + 
		m12 * (m23*m31 - m21*m33) + 
		m13 * (m21*m32 - m22*m31);
}

Matrix4x3 Matrix4x3::Inverse() const
{
	float det = Determinant();
	assert(std::fabs(det) > 1e-4f);
	
	det = 1.f / det;
	Matrix4x3 r;
	r.m11 = (m22*m33 - m23*m32) * det;
	r.m12 = (m13*m32 - m12*m33) * det;
	r.m13 = (m12*m23 - m13*m22) * det;
	r.m21 = (m23*m31 - m21*m33) * det;
	r.m22 = (m11*m33 - m13*m31) * det;
	r.m23 = (m13*m21 - m11*m23) * det;
	r.m31 = (m21*m32 - m22*m31) * det;
	r.m32 = (m12*m31 - m11*m32) * det;
	r.m33 = (m11*m22 - m12*m21) * det;
	r.tx = -(tx*r.m11 + ty*r.m21 + tz*r.m31);
	r.ty = -(tx*r.m12 + ty*r.m22 + tz*r.m32);
	r.tz = -(tx*r.m13 + ty*r.m23 + tz*r.m33);

	return r;
}

std::array<float, 16> Matrix4x3::ToArray() const
{
	std::array<float, 16> a;
	a[ 0] = m11; a[ 1] = m12; a[ 2] = m13; a[ 3] = 0.f;
	a[ 4] = m21; a[ 5] = m22; a[ 6] = m23; a[ 7] = 0.f;
	a[ 8] = m31; a[ 9] = m32; a[10] = m33; a[11] = 0.f;
	a[12] = tx;  a[13] = ty;  a[14] = tz;  a[15] = 1.f;
	return a;
}

/*MATRIX Matrix4x3::ToMATRIX() const
{
	MATRIX r;
	r.m[0][0] = m11; r.m[0][1] = m12; r.m[0][2] = m13; r.m[0][3] = 0.f;
	r.m[1][0] = m21; r.m[1][1] = m22; r.m[1][2] = m23; r.m[1][3] = 0.f;
	r.m[2][0] = m31; r.m[2][1] = m32; r.m[2][2] = m33; r.m[2][3] = 0.f;
	r.m[3][0] = tx;  r.m[3][1] = ty;  r.m[3][2] = tz;  r.m[3][3] = 1.f;
	return r;
}*/



const vec3f operator*(const vec3f& v, const Matrix4x3& m)
{
	return vec3f(
		v.x*m.m11 + v.y*m.m21 + v.z*m.m31 + m.tx,
		v.x*m.m12 + v.y*m.m22 + v.z*m.m32 + m.ty,
		v.x*m.m13 + v.y*m.m23 + v.z*m.m33 + m.tz);
}

const Matrix4x3 operator*(const Matrix4x3& a, const Matrix4x3& b)
{
	Matrix4x3 r;

	r.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
	r.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
	r.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;

	r.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
	r.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
	r.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;

	r.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;
	r.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;
	r.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;

	r.tx = a.tx*b.m11 + a.ty*b.m21 + a.tz*b.m31 + b.tx;
	r.ty = a.tx*b.m12 + a.ty*b.m22 + a.tz*b.m32 + b.ty;
	r.tz = a.tx*b.m13 + a.ty*b.m23 + a.tz*b.m33 + b.tz;

	return r;
}

vec3f& operator*=(vec3f& v, const Matrix4x3& m)
{
	v = v * m;
	return v;
}

Matrix4x3& operator*=(Matrix4x3& a, const Matrix4x3& b)
{
	a = a * b;
	return a;
}
