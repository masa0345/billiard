#pragma once

#include "vector3.h"
#include <array>

class Quaternion;

// 3�����A�t�B���ϊ������邽�߂̍s��
class Matrix4x3
{
	friend const vec3f operator*(const vec3f& v, const Matrix4x3& m);
	friend const Matrix4x3 operator*(const Matrix4x3& a, const Matrix4x3& b);
	friend vec3f& operator*=(vec3f& v, const Matrix4x3& m);
	friend Matrix4x3& operator*=(Matrix4x3& a, const Matrix4x3& b);
public:
	// �P���s��
	static Matrix4x3 Identity();
	// ���s�ړ�
	static Matrix4x3 Translation(const vec3f& v);
	// ��]
	static Matrix4x3 Rotate(const vec3f& axis, float theta);
	static Matrix4x3 Rotate(const Quaternion& q);
	static Matrix4x3 RotateX(float angle);
	static Matrix4x3 RotateY(float angle);
	static Matrix4x3 RotateZ(float angle);
	// �g��
	static Matrix4x3 Scale(const vec3f& s);

	// �s��
	float Determinant() const;
	// �t�s��
	Matrix4x3 Inverse() const;
	// �v�f��z��Ŏ擾
	std::array<float, 16> ToArray() const;

private:
	// �O�����璼�ڃC���X�^���X�����Ȃ�
	Matrix4x3() = default;
	
	float m11, m12, m13;
	float m21, m22, m23;
	float m31, m32, m33;
	float tx, ty, tz;
};

// ��Z
const vec3f operator*(const vec3f& v, const Matrix4x3& m);
const Matrix4x3 operator*(const Matrix4x3& a, const Matrix4x3& b);

vec3f& operator*=(vec3f& v, const Matrix4x3& m);
Matrix4x3& operator*=(Matrix4x3& a, const Matrix4x3& b);